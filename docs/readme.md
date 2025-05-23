# SeaTalk.ino

Communicate with E80 via SeaTalk and Teensy 4.0.
The teensy 4.0 is a logical choice for a Seatalk device, as it
natively supports 9 bits in hardware UARTS.

C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy4\HardwareSerial.h
modified as administrator to uncomment #define SERIAL_9BIT_SUPPORT near top

After much messing around and very little joy trying with ESP32 to
	communicate with the E80 directly using NMEA2000 with ST2551 CANBUS
	modules (months of work) and in_esp32/NMEA0183 with MAX485 RS485,
	RS232, and FT232 modules, and messing around with the (probably
	broken) SR161 AIS Reciever, this program was needed so that I can
	"drive" the E80 on my desk to do further work on NMEA0183/2000 code.

Seatalk is a 12V single ended 4800 baud 9 bit bi-directional protocol.
See http://www.thomasknauf.de/seatalk.htm, with thanks, for more info.
	See the docs folder for these webpages, downloaded for safety.


### Purported cable pinout:

The E80 cable diagram purports to have distinct "Seatalk In" and "Seatalk Out"
pins on the Seatalk Alarm connector, but that is not actually the case.

pin1 is left of notch facing PLUG, and they go clockwise

- pin1: GND (not)					shield
- pin2: +12V (not)				red
- pin3: Seatalk RX (not)			black		Seatalk bus GND
- pin4: Seatalk TX (not)			yellow		Seatalk bus DATA
- pin5: Alarm out (untested)		white
- pin6: Alarm return (untested) 	brown

In my live "Seatalk Junction Box" only the yellow (TX) and
black (RX) pins of the E80 are/were connected.  The black (RX)
is connected to the Seatalk bus GND, and the yellow to the
Seatalk common DATA bus line.

I believe the pinout description for this cable is wrong, or
misleading at best. I am certain the yellow "TX" line is,
in fact the bi-direction general Seatalk bus, and as I have
it working, the black, NOT the shield, is the GND.

There is NOT 12V on the red wire, nor is the shield connected to
the power supply ground.  In fact these are floating values,
that measure significantly differently than expected and
I found no good use for them.

### Experiments

Initially I thought all that was needed for RX was a (10K/3.3K)
voltage divider between the shield and yellow pins, but I was wrong.

Started with the program doing nothing.

Added the logic analyzer and got a signal of sorts,
but I could not make heads or tails of it.

Pulled out the oscilliscope and got really confused.
Think I "sort of" know how the oscilloscope works.
I get what looks like 0V for "high", getting pulled
down to about -10V for "low" volts, but not a good
looking digital data signal, having transients that are
too short to be bits.

Saw that I need a 2.2K pullup to 12V on the ref html circut,
so added it, and got not much difference.  That's when
it started to get weird.

The "red" pin2 on the ST cable from the E80 is NOT at +12V.
relative to the "shield" pin1. Nada on the multimeter.
Then I measured red relative to the GND on
the power supply.  It measured nominal, like 0.2V
And, weirdly, then I measured relative to the supply
+12V, and I got something like -0.4V, as if it is
floating with a higher impedence than the multimeter.

Then revisiting my live Seatalk Junction Box diagram, I see
that I was connecting the battery ground to the "black"
ground (but NOT the battery 12V to the "red" which is
unattached ONLY for the E80 ... all the other ST's have
their reds connected to the battery +12V).

So, I'm like, OK, the E80 is sort of passive ST device,
using just the shield and yellow wires. Then, FFS,
I look at the actual JXN box wiring, and the BLACK,
yikes, which is THE E80 RX IS CONNECTED TO THE
BATTERY GROUND but the SHIELD IS NOT CONNECTED.
Cannot find ANYTHING that references the idea that the
E80 will use a separate ST "RX" channel.
PLUS the only effing way the E80 could show the
ST50 wind is that it is READING it from that yellow
black/pair.

### Optoisolated circuit

At https://forum.openmarine.net/showthread.php?tid=4617 I found
an optoisolated circuit said to be able to listen to the E80.
Its even a 3.3V circuit.  After building it, and connecting
the black and yellow from the E80 as described, IT WORKED!

Bolstered by my success, I quickly designed, implemented, and
tested a similar optoisolated TX circuit.
See /docs/kicad for the actual schematic.

### Output

I "recorded" a number of datagrams sent by the E80 in Simulation
mode (with "Send Sim=ON" buried in the E80 Seatalk Diagnostics
menu) and when I then sent them to the E80 with the
Simulation Off, the E80 SUCCESFULLY, FINALLY, showed some
data.

So now I can "drive" the E80 and test it's NEMA0183/2000 IO
capabilities in  my other code.


