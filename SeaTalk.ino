//---------------------------------------------
// SeaTalk.ino
//---------------------------------------------
// Communicate with E80 via SeaTalk and Teensy 4.0.
// The teensy 4.0 is a logical choice for a Seatalk device, as it
// 		natively supports 9 bits in hardware UARTS.
// C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy4\HardwareSerial.h
//		modified as administrator to uncomment #define SERIAL_9BIT_SUPPORT near top
//
// See /docs/readme.md for more information.
// See /docs/kicad/breadboard for the schematic
// See http://www.thomasknauf.de/seatalk.htm, with thanks, for
//		Seatalk technical info
// See https://forum.openmarine.net/showthread.php?tid=4617 for
//		the optoisolated circuit used as basis for my schematic.

#include <myDebug.h>	// https://github.com/phorton1/Arduino-libraries-myDebug
#include "SeaTalk.h"

#define ALIVE_LED		13
#define ALIVE_OFF_TIME	980
#define ALIVE_ON_TIME	20


bool show_input 		= 0;	// set to 1 to call showDatagram() on incoming datagrams
bool show_every_char 	= 0;	// set to one to see every incoming seatalk character

uint32_t last_rcv_time = 0;		// ms time of last received seatalk byte



void setup()
{
	#if ALIVE_LED
		pinMode(ALIVE_LED,OUTPUT);
		digitalWrite(ALIVE_LED,1);
	#endif

	Serial.begin(115200);

	display(0,"SeaTalk.ino setup() started",0);

	SEATALK_PORT.begin(4800, SERIAL_9N1);
		// Requires #define SERIAL_9BIT_SUPPORT in HardwareSerial.h
		// Uses "normal" data when using the opto-isolater as wired!
		// Note that there is also SERIAL_9N1_RXINV_TXINV which *might*
		// work with inverted signal (different circuit).

	display(0,"SeaTalk.ino setup() completed",0);

	#if ALIVE_LED
		digitalWrite(ALIVE_LED,0);
	#endif
}



void loop()
{
	#if ALIVE_LED
		static bool alive_on = 0;
		static uint32_t last_alive_time = 0;
		uint32_t alive_now = millis();
		uint32_t alive_delay = alive_on ? ALIVE_ON_TIME : ALIVE_OFF_TIME;
		if (alive_now - last_alive_time >= alive_delay)
		{
			alive_on = !alive_on;
			digitalWrite(ALIVE_LED,alive_on);
			last_alive_time = alive_now;
		}
	#endif

	// handle commands/simulation, etc

	process();
	
	// receive datagrams

	while (SEATALK_PORT.available())
	{
		int c = SEATALK_PORT.read();
		last_rcv_time = millis();

		// the 9th bit is set on the first 'byte' of a sequence
		// the low nibble of the 2nd byte + 3 is the total number
		// 		of bytes, so all messages are at least 3 bytes
		//		the high nibble may be data.
		//	data[n+3];, implying a maximum datagram size of 19

		if (show_every_char)
			display(0,"got 0x%02x '%c'",c,(c>32 && c<128)?c:' ');
			
		static uint8_t datagram[20];
		static int outp = 0;
		static int dlen = 0;

		if (c > 0xff)
		{
			if (outp)
			{
				my_error("Dropped datagram ",0);
				outp = 0;
			}
			datagram[outp++] = c;
		}
		else if (outp == 1)
		{
			dlen = (c & 0x0f) + 3;
			datagram[outp++] = c;
		}
		else if (outp < dlen)
		{
			datagram[outp++] = c;
			if (outp == dlen)
			{
				if (show_input)
					showDatagram(datagram);
				outp = 0;
				dlen = 0;
			}
		}
		else
		{
			my_error("unexpected byte 0x%02x '%c'",c,(c>32 && c<128)?c:' ');
		}

	}	// receiving datagrams
}	// loop


// end of SeaTalk.ino
