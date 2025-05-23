//---------------------------------------------
// SeaTalk.h
//---------------------------------------------
// Common defines and externs for SeaTalk.ino

#pragma once

#define SEATALK_PORT Serial3
	// 14 = TX3
	// 15 = RX3

extern void process();
	// in process.cpp

extern void showDatagram(const uint8_t *datagram);
	// in decode.cpp

extern bool show_input;			// call showDatagram() on incoming datagrams
extern bool show_every_char;	// show every charcter on incoming datagrams
extern uint32_t last_rcv_time;	// used for idle detection in process.cpp
	// in SeaTalk.ino,


// end of SeaTalk.h
