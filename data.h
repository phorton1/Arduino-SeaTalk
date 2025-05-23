//---------------------------------------------
// data.h
//---------------------------------------------
// Constants and encoding methods for selected
// Seatalk sentences

#pragma once

extern int dbg_data;

#define ST_DEPTH		0x100		//     out Depth Below Transducer
#define ST_RPM			0x105		//     out Engine RPM and default PITCH; rpm may be negative
#define ST_WIND_ANGLE	0x110		//     out Apparent Wind Angle
#define ST_WIND_SPEED	0x111		//     out Apparent Wind Speed
#define ST_WATER_SPEED	0x120		//     out Speed Through Water
#define ST_SOG			0x152		//     out Speed over Ground
#define ST_COG			0x153		//     out Course over Ground
#define ST_TIME			0x154		//     out GMT Time
#define ST_DATE			0x156		//     out YYMDD
#define ST_LATLON		0x158		//     out Position
#define ST_HEADING		0x189		//     out ST40 compass heading


extern void stDepth(uint16_t *dg, float depth);
extern void stRPM(uint16_t *dg, int rpm);
extern void stWindAngle(uint16_t *dg, float angle);
extern void stWindSpeed(uint16_t *dg, float speed);
extern void stWaterSpeed(uint16_t *dg, float speed);
extern void stSOG(uint16_t *dg, float speed);
extern void stCOG(uint16_t *dg, float degrees);
extern void stTime(uint16_t *dg);
extern void stDate(uint16_t *dg);
extern void stLatLon(uint16_t *dg, float lat, float lon);
extern void stHeading(uint16_t *dg, float degrees);


// end of data.h
