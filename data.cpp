//---------------------------------------------
// data.cpp
//---------------------------------------------
// Encoding methods for selected Seatalk sentences

#include <myDebug.h>
#include "data.h"

int dbg_data = 1;


void stDepth(uint16_t *dg, float depth)
{
	display(dbg_data,"stDepth(%0.1f)",depth);
	uint16_t d10;
	if (depth > 999)
		d10 = 9990;
	else
		d10 = depth * 10.0;

	dg[0] = ST_DEPTH;
	dg[1] = 0x02;
	dg[2] = 0;	// flags
	dg[3] = d10 & 0xff;
	dg[4] = (d10 >> 8) & 0xff;
}


void stRPM(uint16_t *dg, int rpm)
{
	display(dbg_data,"stRPM(%d)",rpm);
	if (rpm > 4000)
		rpm = 4000;
	dg[0] = ST_RPM;
	dg[1] = 0x03;
	dg[2] = 0;	// engine number
	dg[3] = rpm / 256;
	dg[4] = rpm % 256;
	dg[5] = 0x08;	// default 8 degree pitch

}


void stWindAngle(uint16_t *dg, float angle)
{
	display(dbg_data,"stWindAngle(%0.1f)",angle);
	if (angle > 359.5)
		angle = 359.5;
	int a2 = angle * 2;
	dg[0] = ST_WIND_ANGLE;
	dg[1] = 0x01;
	dg[2] = (a2 >> 8) & 0xff;
	dg[3] = a2 & 0xff;
}


void stWindSpeed(uint16_t *dg, float speed)
{
	if (speed > 127.9)
		speed = 127.9;
		
	int tenths = (speed + 0.05) * 10.0;
		// round float to nearest 0.1 for math
	int ispeed = tenths / 10;
	tenths = tenths % 10;

	display(dbg_data,"stWindSpeed(%0.1f) ispeed=%d tenths=%d",speed,ispeed,tenths);

	dg[0] = ST_WIND_SPEED;
	dg[1] = 0x01;
	dg[2] = (ispeed & 0x7f);		// high order bit 0=knots; 1 would be fathoms
	dg[3] = tenths;

	// even though I am sending 12.7, the E80 is showing 12.6
}


void stWaterSpeed(uint16_t *dg, float speed)
{
	display(dbg_data,"stWaterSpeed(%0.1f)",speed);
	if (speed > 99.9)
		speed = 99.9;
	dg[0] = ST_WATER_SPEED;
	int ispeed = (speed + 0.05) * 10;
	dg[1] = 01;
	dg[2] = ispeed & 0xff;
	dg[3] = (ispeed >> 8) & 0xff;
}



void stSOG(uint16_t *dg, float speed)
{
	display(dbg_data,"stSOG(%0.1f)",speed);
	if (speed > 99.9)
		speed = 99.9;
	int ispeed = speed * 10;
		
	dg[0] = ST_SOG;
	dg[1] = 0x01;
	dg[2] = ispeed & 0xff;
	dg[3] = (ispeed >> 8) & 0xff;
}


void stCOG(uint16_t *dg, float degrees)
{
	if (degrees > 359.5)
		degrees = 359.5;

	// what a weird encoding
	// .... ........
	// HH99   222222

	int idegrees = degrees;
	int nineties = idegrees / 90;
	idegrees = idegrees - (nineties * 90);
	int twos = idegrees / 2;
	idegrees = idegrees - (twos * 2);
	int halfs = idegrees * 2;

	display(dbg_data,"stCOG(%0.1f) = nineties(%d) twos(%d) halfs(%d)",degrees,nineties,twos,halfs);

	dg[0] = ST_COG;
	dg[1] = 0 | (nineties << 4) | (halfs<<6);
	dg[2] = twos;

}


void stTime(uint16_t *dg)	// GMT
{
	int secs = 12*3600 + millis()/1000;
	int hour = secs / 3600;
	int minute = (secs - hour * 3600) / 60;
	secs = secs % 60;

	if (hour > 23)
		hour = 0;

	uint16_t RST = (minute << 6) | secs;

	display(dbg_data,"stTime(%02d:%02d:%02d)",hour,minute,secs);

	dg[0] = ST_TIME;
	dg[1] = 0x01 | ((RST & 0x3) << 4);
	dg[2] = RST >> 2;
	dg[3] = hour;
}


void stDate(uint16_t *dg)
{
	int year = 25;
	int month = 5;
	int day = 10;

	display(dbg_data,"stDate(%02d/%02d/%02d)",month,day,year);
	dg[0] = ST_DATE;
	dg[1] = 0x01 | (month << 4);
	dg[2] = day;
	dg[3] = year;

}


void stLatLon(uint16_t *dg, float lat, float lon)
{
	display(dbg_data,"stLatLon(%0.6f,%0.6f)",lat,lon);
	uint8_t Z1 = 0;
	uint8_t Z2 = 0x20;
	if (lat < 0)
	{
		lat = abs(lat);
		Z1 = 0x10;
	}
	if (lon < 0)
	{
		lon = abs(lon);
		Z2 = 0x0;
	}

	// integer portions
	uint16_t i_lat = lat;
	uint16_t i_lon = lon;

	// right of decimal point
	float frac_lat = lat - i_lat;
	float frac_lon = lon - i_lon;

	// converted to minutes
	float min_lat = frac_lat * 60.0;
	float min_lon = frac_lon * 60.0;

	// times 1000 into integers
	int imin_lat = min_lat * 1000;
	int imin_lon = min_lon * 1000;

	proc_entry();
	display(dbg_data,"i_lat(%d) frac_lat(%0.6f) min_lat(%0.6f) imin_lat(%d)",i_lat,frac_lat,min_lat,imin_lat);
	display(dbg_data,"i_lon(%d) frac_lon(%0.6f) min_lon(%0.6f) imin_lon(%d)",i_lon,frac_lon,min_lon,imin_lon);
	proc_leave();

	dg[0] = ST_LATLON;
	dg[1] = 0x5 | Z1 | Z2;

	dg[2] = i_lat;
	dg[3] = (imin_lat >> 8) & 0xff;
	dg[4] = imin_lat & 0xff;

	dg[5] = i_lon;
	dg[6] = (imin_lon >> 8) & 0xff;
	dg[7] = imin_lon & 0xff;
}


void stHeading(uint16_t *dg, float degrees)
	// as sent by ST40
{
	if (degrees > 359.5)
		degrees = 359.5;

	// what a weird encoding
	// .... ........
	// HH99   222222

	int idegrees = degrees;
	int nineties = idegrees / 90;
	idegrees = idegrees - (nineties * 90);
	int twos = idegrees / 2;
	idegrees = idegrees - (twos * 2);
	int halfs = idegrees * 2;

	display(dbg_data,"stHeading(%0.1f) = nineties(%d) twos(%d) halfs(%d)",degrees,nineties,twos,halfs);

	dg[0] = ST_HEADING;
	dg[1] = 2 | (nineties << 4) | (halfs<<6);
	dg[2] = twos;
	dg[3] = 0x00;	// unuaed by me at this time
	dg[4] = 0x20;	// unuaed by me at this time
}



// end of data.cpp
