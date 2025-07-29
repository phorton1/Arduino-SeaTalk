# SeaTalk.ino

Communicates with E80 via SeaTalk and Teensy 4.0.
The teensy 4.0 is a logical choice for a Seatalk device, as it
natively supports 9 bits in hardware UARTS.

C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy4\HardwareSerial.h
modified as administrator to uncomment #define SERIAL_9BIT_SUPPORT near top

Seatalk is a 12V single ended 4800 baud 9 bit bi-directional protocol.
See http://www.thomasknauf.de/seatalk.htm, with thanks, for more info.
	See the docs folder for these webpages, downloaded for safety.

The circuit was prototyped and tested on a breadboard directly to an E80.

The installed circuit board also serves as a passive Seatalk junction box,
wherin the teensy and supporting circutry are not required for the boat
instruments to function properly.


### Purported E80 cable pinout:

In my live "Seatalk Junction Box" only the yellow (TX) and
black (RX) pins of the E80 are/were connected.  The black (RX)
is connected to the Seatalk bus GND, and the yellow to the
Seatalk common DATA bus line.


### Junction Box vs Active Circuit?

The breadboarded, direct to E80 circuit, and current code, allow me to
"drive" the E80.

Theoretically I could use this as a Seatalk "interpreter" to/from the
instruments and E80.

GPS
ST50 Log
ST50 Wind
ST7000 Autopilot

However, I haven't yet insterfaced any other instruments to this circuit
(or the "desk" E80) as it is difficult to provide the specialized sensor
input and/or get the GPS out of the boat to the apartment.

In the old existing system, thes instruments were all serial/bussed to the
"seatalk junction" connector, where the E80 received them and theoretically
can also talk to the auto pilot.




