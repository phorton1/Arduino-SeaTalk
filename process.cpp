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
#define IDLE_BUS_MS				10		// ms bus must be idle to send next datagram
#define SEND_DELAY				50		// ms between each datagram
#define NUM_RETRIES				6

#define PIN_PULSE				11

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

static int running = 0;
	// defines number of messages to send
	// hopefully the order is somewhat meaningful

static bool show_output = 0;

int loop_counter = 0;
uint32_t time_init = 0;
int time_year = 0;
int time_month = 1;
int time_day = 1;


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

static bool pulse_out;
static uint32_t pulse_ms;
static uint32_t last_pulse;


// state variables

static bool inited = 0;
static int dg_num = 0;
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
	
	display(0,"Seatalk USAGE",0);
	proc_entry();

	display(0,"? = show this help",0);
	display(0,"y = re-initialize simulator",0);
	display(0,"i = show received datagrams",0);
	display(0,"o = show sent datagrams",0);
	display(0,"xN = x0 to stop; xN to send first N datagrams",0);

	display(0,"dN, d+N, d-N = set AND SEND depth",0);
	display(0,"hN, h+N, h-N = set/increment/decrement heading",0);
	display(0,"sN, s+N, s-N = set/increment/decrement speed",0);
	display(0,"jN, j+N, j-N = jump to waypoint; next waypoint, prev waypoint",0);
	display(0,"wN, w+N, w-N = set heading to waypoint; next waypoint, prev waypoint",0);
	display(0,"gN, g+N, g-N = set ms between pulses to speed log test",0);

	display(0,"t = send time(00:00:00)",0);

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



const uint16_t *getDatagram(int num);
int sendDatagram(const uint16_t *dg);
	// forwards


static void init_sim()
{
	display(0,"INITIALIZE SIMULATOR",0);

	loop_counter = 0;
	time_init = millis();
	time_year = 25;
	time_month = 9;
	time_day = 5;
	if (0)
	{
		display(0,"TIME(0)",9);
		const uint16_t *dg = getDatagram(0);
		sendDatagram(dg);
	}
	pulse_ms = 100;
	pulse_out = 0;
	pinMode(PIN_PULSE,OUTPUT);
	digitalWrite(PIN_PULSE,pulse_out);

	
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

#define NUM_DGS   10

const uint16_t *getDatagram(int num)
{
	static uint16_t dg[20];
	memset(dg,0,20*2);
	switch (num)
	{
		case 0: stTime(dg);							break;
		case 1: stDate(dg);							break;
		case 2: stHeading(dg, heading);	        	break;
		case 3: stDepth(dg, depth);					break;
		case 4: stCOG(dg, cog);	                    break;
		case 5: stSOG(dg, sog);	                    break;
		case 6: stLatLon(dg, latitude, longitude);	break;
		case 7: stWindAngle(dg, app_wind_angle);	break;
		case 8: stWindSpeed(dg, app_wind_speed);	break;
		case 9: stWaterSpeed(dg, water_speed);		break;
		case 10: stRPM(dg, rpm);	                    break;
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
		Serial.print("]");
	}

	int len = (dg[1] & 0xf) + 3;
	for (int i=0; i<len; i++)
	{
		int out_byte = dg[i];

		if (show_output)
		{
			Serial.print(" 0x");
			Serial.print(out_byte,HEX);
		}

		SEATALK_PORT.write9bit(out_byte);
	}

	if (show_output)
	{
		Serial.println();

		// experiment to send a binary packet

		char packet[300];

		packet[0] = 0x02;		// binary indicator
		packet[1] = 0xff;		// big endian length in two bytees
		packet[2] = 0x00;

		for (int i=0; i<256; i++)
		{
			packet[i+3] = (uint8_t) i;
		}

		Serial.write(packet,258);
	}
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

	if (command == 'x')
	{
		if (val > NUM_DGS)
			val = NUM_DGS;

		if (val && !running)
			start_sim();
		else if (running && !-val)
			stop_sim();
		running = val;
	}
	else if (command == 'h')
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
	else if (command == 'd')
	{
		if (inc)
			depth += (inc * val);
		else
			depth = val;

		if (depth < 0)
			depth = 0;
		if (depth > 99)
			depth = 99;

		display(0,"DEPTH(depth)",depth);
		const uint16_t *dg = getDatagram(11);
		sendDatagram(dg);



	}
	else if (command == 'g')
	{
		if (inc)
			pulse_ms += (inc * val);
		else
			pulse_ms = val;
		if (pulse_ms > 10000)
			pulse_ms = 10000;
		if (pulse_ms < 0)
			pulse_ms = 0;

		display(0,"pulse <= %d MS",pulse_ms);
	}

}




void process()
{
	if (!inited)
	{
		init_sim();
		usage();
	}

	uint32_t now = millis();

	if (pulse_out && !pulse_ms)
	{
		pulse_out = 0;
		digitalWrite(PIN_PULSE,pulse_out);
		last_pulse = now;
	}
	else if (pulse_ms && now - last_pulse > pulse_ms)
	{
		last_pulse = now;
		pulse_out = !pulse_out;
		digitalWrite(PIN_PULSE,pulse_out);
	}


	if (running)
	{
		uint32_t diff = now - last_send_time;
		if ((!dg_num && (diff > SEND_INTERVAL)) ||
			 (dg_num && (diff > SEND_DELAY)))
		{
			if (!dg_num)
			{
				updatePosition();
				if (show_output)
					display(0,"----------------START---------------",0);
			}

			if (dg_num < running)
			{
				const uint16_t *dg = getDatagram(dg_num);
				sendDatagram(dg);
				last_send_time = millis();
				dg_num++;
			}
			else
			{
				dg_num = 0;
				last_send_time = millis();
				if (show_output)
					display(0,"----------------END-----------------",0);
				loop_counter++;
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
		else if (c == 't')
		{
			display(0,"TIME(0)",9);
			time_init = millis();
			const uint16_t *dg = getDatagram(0);
			sendDatagram(dg);
		}

		else if (c == 'h' || c == 's' || c == 'w' || c == 'j' || c == 'g' || c == 'd' || c == 'x')
		{
			in_command = c;
		}
	}
}


// end of process.cpp
