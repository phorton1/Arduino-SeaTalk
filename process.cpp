//---------------------------------------------
// process.cpp
//---------------------------------------------
// Command processor and simulator.
// Initial version just regurgitates recorded datagrams
// The E80 invalidates data after about 2 seconds and
// 		alarms on loss of position, heading, depth, etc.

#include <myDebug.h>
#include "Seatalk.h"
#include "data.h"
#include <math.h>

#define SEND_OK			0
#define SEND_BUSY 		1
#define SEND_COLLISION	2
#define SEND_ERROR		3

#define SEND_INTERVAL 			1000	// ms between sending update to E80
#define IDLE_BUS_MS				5		// ms bus must be idle to send next datagram
#define SEND_DELAY				20		// ms between each datagram
#define NUM_RETRIES				5


typedef struct
{
	const char *name;
	float lon;
	float lat;
} waypoint_t;


const waypoint_t waypoints[] =
{
	{ "Popa0", -82.242050, 9.334083 },
	{ "Popa1", -82.204624, 9.272120 },
	{ "Popa2", -82.197158, 9.255866 },
	{ "Popa3", -82.193311, 9.249720 },
	{ "Popa4", -82.180733, 9.231067 },
	{ "Popa5", -82.165517, 9.227000 },
	{ "Popa6", -82.155577, 9.208679 },
	{ "Popa7", -82.156200, 9.203134 },
	{ "Popa8", -82.143034, 9.204582 },
};

const int NUM_WAYPOINTS = 9;


// UI variables

static bool running = 0;
static bool show_output = 0;

static float depth;			// 0-999.9	depth below transducer
static int 	 rpm;			// 0-4000	negative = backwards; currently useless
static float wind_angle;	// 0-359.5  TRUE wind angle
static float wind_speed;	// 0-127.9	TRUE wind speed
static float water_speed;	// 0-99.9	speed through water
static float sog;			// 0-99.9	speed over ground, knots,
static float cog;			// 0-359.5  course over ground
static float latitude;
static float longitude;
static float heading;		// 0-359.9

static int waypoint_num;	// 0..NUM_WAYPOINTS-1


// state variables

static bool inited = 0;
static int dg_num = 0;
static int retry_num = 0;
static uint32_t last_send_time = 0;
static uint32_t last_update_ms = 0;

static float app_wind_speed;
static float app_wind_angle;


//-----------------------------------------------
// implementation
//-----------------------------------------------

static void usage()
{
	display(0,"STATE: waypoint(%d) heading(%0.1f) speed(%0.1f) show input(%d) output(%d)",
		waypoint_num,cog,sog,show_input,show_output);
	
	display(0,"USAGE",0);
	proc_entry();

	display(0,"? = show this help",0);
	display(0,"x = start/stop simulator",0);
	display(0,"y = re-initialize simulator",0);
	display(0,"i = show received datagrams",0);
	display(0,"o = show sent datagrams",0);
	display(0,"hN, h+N, h-N = set/increment/decrement heading",0);
	display(0,"sN, s+N, s-N = set/increment/decrement speed",0);
	display(0,"jN, j+N, j-N = jump to waypoint; next waypoint, prev waypoint",0);
	display(0,"wN, w+N, w-N = set heading to waypoint; next waypoint, prev waypoint",0);

	proc_leave();
}


void calculateApparentWind()
{
    float wind_angle_rad = wind_angle * M_PI / 180.0;
    float cog_rad = cog * M_PI / 180.0;

    float wind_x = wind_speed * cos(wind_angle_rad);
    float wind_y = wind_speed * sin(wind_angle_rad);
    float sog_x = sog * cos(cog_rad);
    float sog_y = sog * sin(cog_rad);

    float apparent_x = wind_x + sog_x; // Wind vector minus boat speed vector
    float apparent_y = wind_y + sog_y;

    app_wind_speed = sqrt(apparent_x * apparent_x + apparent_y * apparent_y);
    app_wind_angle = atan2(apparent_y, apparent_x) * 180.0 / M_PI;

    if (app_wind_angle < 0)
        app_wind_angle += 360;

    app_wind_angle = (app_wind_angle - cog + 360);
    app_wind_angle = fmod(app_wind_angle, 360);

	display(0,"angle,speed  boat(%0.1f,%0.1f)  true(%0.1f,%0.1f)  apparent(%0.1f,%0.1f)",
		cog,sog,
		wind_angle,wind_speed,
		app_wind_angle,app_wind_speed);
}




static void init_sim()
{
	display(0,"INITIALIZE SIMULATOR",0);

	depth = 10;
	rpm = 0;
	wind_angle = 90;
	wind_speed = 12;
	water_speed = sog = 0;
	heading = cog = 180;
	latitude = waypoints[0].lat;
	longitude = waypoints[0].lon;

	calculateApparentWind();

	waypoint_num = 0;

	dg_num = 0;
	retry_num = 0;
	last_send_time = 0;
	last_update_ms = 0;
	inited = true;
}


void updatePosition()
{
	uint32_t now = millis();
	double elapsed_secs = (now - last_update_ms) / 1000.0;
	last_update_ms = now;
	if (sog == 0)
		return;

	const double earth_radius = 6371000; 	// in meters
	const double knots_to_mps = 0.514444;
	double distance_m = sog * knots_to_mps * elapsed_secs;
	double heading_rad = heading * M_PI / 180.0;
	double delta_lat = (distance_m * cos(heading_rad)) / earth_radius;
	double delta_lon = (distance_m * sin(heading_rad)) / (earth_radius * cos(latitude * M_PI / 180.0));
	latitude += delta_lat * (180.0 / M_PI);
	longitude += delta_lon * (180.0 / M_PI);
}


void setHeadingToWaypoint(const waypoint_t *waypoint)
{
	double way_lon = waypoint->lon;
	double way_lat = waypoint->lat;
	double delta_lon = way_lon - longitude;

	double y = sin(delta_lon * M_PI / 180.0) * cos(way_lat * M_PI / 180.0);
	double x = cos(latitude * M_PI / 180.0) * sin(way_lat * M_PI / 180.0) -
		sin(latitude * M_PI / 180.0) * cos(way_lat * M_PI / 180.0) * cos(delta_lon * M_PI / 180.0);

	double heading_rad = atan2(y, x);
	double heading_deg = heading_rad * (180.0 / M_PI);
	heading_deg = fmod((heading_deg + 360.0), 360.0);

	cog = heading = heading_deg;
}



//--------------------------------------------------
// sendDatagram
//--------------------------------------------------


const uint16_t *getDatagram(int num)
{
	if (num == 11)
		return 0;
	static uint16_t dg[20];
	memset(dg,0,20*2);
	switch (num)
	{
		case 0: stRPM(dg, rpm);	                    break;
			// stDepth(dg, depth);					break;
		case 1: stRPM(dg, rpm);	                    break;
		case 2: stWindAngle(dg, app_wind_angle);	break;
		case 3: stWindSpeed(dg, app_wind_speed);	break;
		case 4: stWaterSpeed(dg, water_speed);		break;
		case 5: stSOG(dg, sog);	                    break;
		case 6: stCOG(dg, cog);	                    break;
		case 7: stTime(dg);							break;
		case 8: stDate(dg);							break;
		case 9: stLatLon(dg, latitude, longitude);	break;
		case 10: stHeading(dg, heading);	        break;
	}
	return dg;
}



int sendDatagram(const uint16_t *dg)
{
	if (millis() - last_rcv_time < IDLE_BUS_MS)
		return SEND_BUSY;	// bus busy

	#define WRITE_TIMEOUT	40

	if (show_output)
	{
		Serial.print("--> [");
		Serial.print(dg_num);
		Serial.print(",");
		Serial.print(retry_num);
		Serial.print("]");
	}

	int len = (dg[1] & 0xf) + 3;
	for (int i=0; i<len; i++)
	{
		bool ok = false;
		int out_byte = dg[i];

		if (show_output)
		{
			Serial.print(" 0x");
			Serial.print(out_byte,HEX);
		}

		uint32_t sent_time = millis();
		SEATALK_PORT.write9bit(out_byte);
		while (!ok)
		{
			if (SEATALK_PORT.available())
			{
				int in_byte = SEATALK_PORT.read();
				if (in_byte == out_byte)
				{
					ok = true;
				}
				else
				{
					if (show_output)
						Serial.println();
					warning(0,"collision",0);
					return SEND_COLLISION;
				}
			}
			else if (millis() - sent_time >= WRITE_TIMEOUT)
			{
				if (show_output)
					Serial.println();
				my_error("WRITE_TIMEOUT",0);
				return SEND_ERROR;
			}
		}
	}

	if (show_output)
		Serial.println();
	return SEND_OK;
}





//--------------------------------------------------
// process
//--------------------------------------------------

void start_sim()
{
	display(0,"STARTING SIMULATOR",0);
	running = true;
}
void stop_sim()
{
	display(0,"STOPPING SIMULATOR",0);
	running = false;
}


void handleCommand(int command, char *buf)
{
	int val = 1;
	int inc = 0;
	if (*buf == '+')
	{
		inc = 1;
		buf++;
	}
	else if (*buf == '-')
	{
		inc = -1;
		buf++;
	}
	if (*buf >= '0' && *buf <= '9')
	{
		val = atoi(buf);
	}

	display(0,"command(%c) inc(%d) val(%d)",command,inc,val);
	if (command == 'h')
	{
		if (inc)
			cog += (inc * val);
		else
			cog = val;

		if (cog < 0)
			cog += 360;
		if (cog > 360)
			cog -= 360;

		heading = cog;
		display(0,"HEADING <= %0.1f",heading);
		calculateApparentWind();
	}
	else if (command == 's')
	{
		if (inc)
			sog += (inc * val);
		else
			sog = val;
		if (sog > 99)
			sog = 99;
		if (sog < 0)
			sog = 0;

		water_speed = sog;
		display(0,"SPEED <= %0.1f",sog);
		calculateApparentWind();
	}
	else if (command == 'w')
	{
		if (inc)
			waypoint_num += (inc * val);
		else
			waypoint_num = val;

		if (waypoint_num < 0)
			waypoint_num = 0;
		if (waypoint_num > NUM_WAYPOINTS - 1)
			waypoint_num = NUM_WAYPOINTS - 1;

		const waypoint_t *wp = &waypoints[waypoint_num];
		setHeadingToWaypoint(wp);
		display(0,"WAYPOINT[%d} %s %0.6f %0.6f  heading=%0.1f",
			waypoint_num,
			wp->name,
			wp->lat,
			wp->lon,
			cog);
		calculateApparentWind();
	}
	else if (command == 'j')
	{
		if (inc)
			waypoint_num += (inc * val);
		else
			waypoint_num = val;

		if (waypoint_num < 0)
			waypoint_num = 0;
		if (waypoint_num > NUM_WAYPOINTS - 1)
			waypoint_num = NUM_WAYPOINTS - 1;

		const waypoint_t *wp = &waypoints[waypoint_num];
		latitude = wp->lat;
		longitude = wp->lon;

		display(0,"JUMP TO WAYPOINT[%d} %s %0.6f %0.6f",
			waypoint_num,
			wp->name,
			wp->lat,
			wp->lon);
	}
}




void process()
{
	if (!inited)
	{
		init_sim();
		usage();
	}

	if (running)
	{
		uint32_t now = millis();
		uint32_t diff = now - last_send_time;
		if ((!dg_num && (diff > SEND_INTERVAL)) ||
			 (dg_num && (diff > SEND_DELAY)))
		{
			updatePosition();

			if (show_output && !dg_num && !retry_num)
				display(0,"----------------START---------------",0);

			const uint16_t *dg = getDatagram(dg_num);
			if (dg && dg[0])
			{
				int rslt = sendDatagram(dg);
				last_send_time = millis();

				if (rslt == SEND_ERROR)
				{
					stop_sim();
				}
				else if (rslt == SEND_COLLISION)
				{
					retry_num++;
					if (retry_num >= NUM_RETRIES)
					{
						my_error("BUS CONGESTED; could not send datagram(%d)",dg_num);
						stop_sim();
					}
					// otherwise, try again later
				}
				else if (rslt == SEND_BUSY)
				{
					// bus busy, try again later
				}
				else // rslt == SEND_OK
				{
					dg_num++;
					retry_num = 0;
				}
			}
			else
			{
				dg_num = 0;
				retry_num = 0;
				last_send_time = millis();
				if (show_output)
					display(0,"----------------END-----------------",0);
			}
		}
	}	// if running
	
	
	// UI


	if (Serial.available())
	{
		#define MAXCOMMAND	12

		static int in_command = 0;
		static int cmd_ptr = 0;
		static char command[MAXCOMMAND+1];

		int c = Serial.read();

		if (in_command)
		{
			if (c == 0x0a || cmd_ptr==MAXCOMMAND)
			{
				command[cmd_ptr++] = 0;
				handleCommand(in_command,command);
				in_command = 0;
				cmd_ptr = 0;
			}
			else if (c != 0x0d)
			{
				command[cmd_ptr++] = c;
			}
		}
		else if (c == '?')
		{
			usage();
		}
		else if (c == 'x')
		{
			if (running)
				stop_sim();
			else
				start_sim();
		}
		else if (c == 'y')
		{
			init_sim();
		}
		else if (c == 'i')
		{
			show_input = !show_input;
			display(0,"SHOW INPUT(%d)",show_input);
		}
		else if (c == 'o')
		{
			show_output = !show_output;
			display(0,"SHOW OUTPUT(%d)",show_output);
		}

		else if (c == 'h' || c == 's' || c == 'w' || c == 'j')
		{
			in_command = c;
		}
	}
}


// end of process.cpp
