EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x06 J?
U 1 1 681E59E9
P 7350 2950
F 0 "J?" H 7268 2425 50  0001 C CNN
F 1 "Seatalk Alarm Cable from E80" H 8150 2500 50  0000 C CNN
F 2 "" H 7350 2950 50  0001 C CNN
F 3 "~" H 7350 2950 50  0001 C CNN
	1    7350 2950
	1    0    0    1   
$EndComp
Text Notes 7600 3200 0    63   ~ 0
brown\nwhite\nyellow\nblack\nred\nshield
Text Notes 8000 3200 0    63   ~ 0
unused alarm return\nunused alarm\nseatalk TX\nseatalk RX (except it's not)\n12V (except it's not)\nGND (excpt it's not)
$Comp
L power:+12V #PWR?
U 1 1 681E9B5C
P 7150 2150
F 0 "#PWR?" H 7150 2000 50  0001 C CNN
F 1 "+12V" H 7165 2323 50  0000 C CNN
F 2 "" H 7150 2150 50  0001 C CNN
F 3 "" H 7150 2150 50  0001 C CNN
	1    7150 2150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 681EA407
P 5100 2200
F 0 "#PWR?" H 5100 2050 50  0001 C CNN
F 1 "+3V3" H 5115 2373 50  0000 C CNN
F 2 "" H 5100 2200 50  0001 C CNN
F 3 "" H 5100 2200 50  0001 C CNN
	1    5100 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 681EA805
P 6950 4700
F 0 "#PWR?" H 6950 4450 50  0001 C CNN
F 1 "GND" H 6955 4527 50  0000 C CNN
F 2 "" H 6950 4700 50  0001 C CNN
F 3 "" H 6950 4700 50  0001 C CNN
	1    6950 4700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 681EAC8B
P 7350 2150
F 0 "J?" H 7430 2142 50  0001 L CNN
F 1 "Power Supply" H 7430 2096 50  0000 L CNN
F 2 "" H 7350 2150 50  0001 C CNN
F 3 "~" H 7350 2150 50  0001 C CNN
	1    7350 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 681F1D31
P 7150 2250
F 0 "#PWR?" H 7150 2000 50  0001 C CNN
F 1 "GND" H 7155 2077 50  0000 C CNN
F 2 "" H 7150 2250 50  0001 C CNN
F 3 "" H 7150 2250 50  0001 C CNN
	1    7150 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 681F5B40
P 2550 1250
F 0 "#PWR?" H 2550 1000 50  0001 C CNN
F 1 "GND" V 2550 1050 50  0000 C CNN
F 2 "" H 2550 1250 50  0001 C CNN
F 3 "" H 2550 1250 50  0001 C CNN
	1    2550 1250
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 681F6BDE
P 2650 1250
F 0 "#PWR?" H 2650 1100 50  0001 C CNN
F 1 "+3V3" V 2650 1450 50  0000 C CNN
F 2 "" H 2650 1250 50  0001 C CNN
F 3 "" H 2650 1250 50  0001 C CNN
	1    2650 1250
	1    0    0    -1  
$EndComp
Text GLabel 3550 1250 1    63   Input ~ 0
RXD
Text GLabel 3650 1250 1    63   Input ~ 0
TXD
$Comp
L 0_my_teensy:myTeensy4.0 U?
U 1 1 681E4C83
P 3000 2250
F 0 "U?" H 3828 2253 60  0001 L CNN
F 1 "myTeensy4.0" H 2850 2150 60  0000 L CNN
F 2 "" V 3900 2300 60  0000 C CNN
F 3 "" V 3900 2300 60  0000 C CNN
	1    3000 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 2950 6900 2950
Wire Wire Line
	6900 2950 6900 3050
$Comp
L power:+12V #PWR?
U 1 1 6820AAC2
P 6000 2200
F 0 "#PWR?" H 6000 2050 50  0001 C CNN
F 1 "+12V" H 6015 2373 50  0000 C CNN
F 2 "" H 6000 2200 50  0001 C CNN
F 3 "" H 6000 2200 50  0001 C CNN
	1    6000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2850 5850 2850
Wire Wire Line
	5850 2650 6000 2650
Wire Wire Line
	5250 2850 5100 2850
$Comp
L Device:R R?
U 1 1 6822159F
P 5100 2350
F 0 "R?" H 5170 2396 50  0001 L CNN
F 1 "4.7K" V 5100 2350 50  0000 C CNN
F 2 "" V 5030 2350 50  0001 C CNN
F 3 "~" H 5100 2350 50  0001 C CNN
	1    5100 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2650 5100 2650
Wire Wire Line
	5100 2650 5100 2500
Text GLabel 4750 2650 0    63   Input ~ 0
RXD
Wire Wire Line
	4750 2650 5100 2650
Connection ~ 5100 2650
$Comp
L Isolator:TLP291 U?
U 1 1 6822E37B
P 5550 2750
F 0 "U?" H 5550 3075 50  0001 C CNN
F 1 "TLP521-1" H 5550 2983 50  0000 C CNN
F 2 "Package_SO:SOIC-4_4.55x2.6mm_P1.27mm" H 5350 2550 50  0001 L CIN
F 3 "https://toshiba.semicon-storage.com/info/docget.jsp?did=12884&prodName=TLP291" H 5550 2750 50  0001 L CNN
	1    5550 2750
	-1   0    0    -1  
$EndComp
$Comp
L Isolator:TLP291 U?
U 1 1 68235388
P 5550 3450
F 0 "U?" H 5550 3775 50  0001 C CNN
F 1 "TLP521-1" H 5550 3683 50  0000 C CNN
F 2 "Package_SO:SOIC-4_4.55x2.6mm_P1.27mm" H 5350 3250 50  0001 L CIN
F 3 "https://toshiba.semicon-storage.com/info/docget.jsp?did=12884&prodName=TLP291" H 5550 3450 50  0001 L CNN
	1    5550 3450
	1    0    0    -1  
$EndComp
Text GLabel 4750 3550 0    63   Input ~ 0
TXD
$Comp
L Device:R R?
U 1 1 68236F3F
P 5000 3350
F 0 "R?" H 5070 3396 50  0001 L CNN
F 1 "220" V 5000 3350 50  0000 C CNN
F 2 "" V 4930 3350 50  0001 C CNN
F 3 "~" H 5000 3350 50  0001 C CNN
	1    5000 3350
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 68244B74
P 4750 3250
F 0 "#PWR?" H 4750 3100 50  0001 C CNN
F 1 "+3V3" H 4765 3423 50  0000 C CNN
F 2 "" H 4750 3250 50  0001 C CNN
F 3 "" H 4750 3250 50  0001 C CNN
	1    4750 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 68211A3C
P 6150 2850
F 0 "R?" H 6220 2896 50  0001 L CNN
F 1 "4.7K" V 6150 2850 50  0000 C CNN
F 2 "" V 6080 2850 50  0001 C CNN
F 3 "~" H 6150 2850 50  0001 C CNN
	1    6150 2850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5250 3350 5150 3350
Wire Wire Line
	4850 3350 4750 3350
Wire Wire Line
	4750 3350 4750 3250
Wire Wire Line
	6300 2850 6600 2850
Wire Wire Line
	6000 2650 6000 2200
Wire Wire Line
	5100 2850 5100 3050
Wire Wire Line
	5100 3050 6900 3050
Connection ~ 6900 3050
Wire Wire Line
	6900 3050 6900 3550
Wire Wire Line
	5850 3550 6900 3550
Connection ~ 6900 3550
Wire Wire Line
	6900 3550 6900 3750
Wire Wire Line
	4750 3550 5250 3550
Wire Wire Line
	5850 3350 6600 3350
Wire Wire Line
	6600 3350 6600 2850
Connection ~ 6600 2850
Wire Wire Line
	6600 2850 7150 2850
Text Notes 4800 1300 0    50   ~ 0
RECEIVE\nThe yellow (bus) is pulled up to 12V (idle) thru 4.7K ohms.  All devices pull the bus up. \nIf the bus is high (idle), no current flows through opto1_diode, and so RXD is pulled\nup to 3.3V through the 4.7K ohm resistor.   When another device pulls the bus low,\nthe diode lights up and RXD is pulled to ground through opto1.
Text Notes 4800 1800 0    50   ~ 0
TRANSMIT\nTXD is normally high, and the other side is pulled up to 3.3v thru 220 ohms,\nso no current flows through opto2_diode, and thus, the yellow (bus) is NOT \npulled down to ground.   When we transmit data, TXD goes low, lights the diode\nand the yellow (bus) is pulled down to ground through opto2.\n
$Comp
L Transistor_BJT:BC547 Q?
U 1 1 68890C7D
P 6850 4500
F 0 "Q?" H 7041 4546 50  0000 L CNN
F 1 "BC547" H 7041 4455 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7050 4425 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 6850 4500 50  0001 L CNN
	1    6850 4500
	1    0    0    -1  
$EndComp
Text Notes 7500 4650 0    50   ~ 0
ST50 SPEED LOG PULSE CIRCUIT\nOnly needs the GREEN (log pulse pin 3) pin and common Seatalk ground.\nThe GREEN is pulled high by the instrument.  Grounding it constitutes a "pulse".\nWe output a square wave on pin 11 at various frequencies.
Text GLabel 6100 4100 0    50   Input ~ 0
GREEN
Text GLabel 6250 4500 0    50   Input ~ 0
PULSE_OUT
$Comp
L Device:R R?
U 1 1 688AFEB1
P 6500 4500
F 0 "R?" H 6570 4546 50  0001 L CNN
F 1 "100" V 6500 4500 50  0000 C CNN
F 2 "" V 6430 4500 50  0001 C CNN
F 3 "~" H 6500 4500 50  0001 C CNN
	1    6500 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	6100 4100 6950 4100
Wire Wire Line
	6950 4100 6950 4300
Wire Wire Line
	6250 4500 6350 4500
Text GLabel 3650 3350 3    50   Input ~ 0
PULSE_OUT
Text Notes 1150 4450 0    50   ~ 0
ST50 Wind Manual Test\nFor speed pulse, pull up to 5V through 220 ohm resistor and \nattach yellow to that
$Comp
L power:GND #PWR?
U 1 1 68894369
P 2350 5300
F 0 "#PWR?" H 2350 5050 50  0001 C CNN
F 1 "GND" H 2355 5127 50  0000 C CNN
F 2 "" H 2350 5300 50  0001 C CNN
F 3 "" H 2350 5300 50  0001 C CNN
	1    2350 5300
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q?
U 1 1 6889436F
P 2250 5100
F 0 "Q?" H 2441 5146 50  0000 L CNN
F 1 "BC547" H 2441 5055 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 2450 5025 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 2250 5100 50  0001 L CNN
	1    2250 5100
	1    0    0    -1  
$EndComp
Text Notes 2900 5250 0    50   ~ 0
ST50 SPEED LOG PULSE CIRCUIT\nOnly needs the GREEN (log pulse pin 3) pin and common Seatalk ground.\nThe GREEN is pulled high by the instrument.  Grounding it constitutes a "pulse".\nWe output a square wave on pin 11 at various frequencies.
Text GLabel 1650 5100 0    50   Input ~ 0
PULSE_OUT
$Comp
L Device:R R?
U 1 1 68894377
P 1900 5100
F 0 "R?" H 1970 5146 50  0001 L CNN
F 1 "100" V 1900 5100 50  0000 C CNN
F 2 "" V 1830 5100 50  0001 C CNN
F 3 "~" H 1900 5100 50  0001 C CNN
	1    1900 5100
	0    1    1    0   
$EndComp
Wire Wire Line
	2350 4700 2350 4900
Wire Wire Line
	1650 5100 1750 5100
$Comp
L power:+5V #PWR?
U 1 1 68895D9D
P 1650 4700
F 0 "#PWR?" H 1650 4550 50  0001 C CNN
F 1 "+5V" V 1665 4828 50  0000 L CNN
F 2 "" H 1650 4700 50  0001 C CNN
F 3 "" H 1650 4700 50  0001 C CNN
	1    1650 4700
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 68897211
P 2000 4700
F 0 "R?" H 2070 4746 50  0001 L CNN
F 1 "220" V 2000 4700 50  0000 C CNN
F 2 "" V 1930 4700 50  0001 C CNN
F 3 "~" H 2000 4700 50  0001 C CNN
	1    2000 4700
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 4700 1850 4700
Wire Wire Line
	2150 4700 2350 4700
Text Notes 1200 5750 0    50   ~ 0
The direction test is done by connecting the 8V (red) through two pots and a 3.3K ohm divider network and manually twidling the pots
$Comp
L Device:R_POT RV?
U 1 1 6889AADC
P 2000 6450
F 0 "RV?" H 1931 6496 50  0001 R CNN
F 1 "10K" V 2000 6450 50  0000 C CNN
F 2 "" H 2000 6450 50  0001 C CNN
F 3 "~" H 2000 6450 50  0001 C CNN
	1    2000 6450
	1    0    0    -1  
$EndComp
Text GLabel 1700 6100 0    50   Input ~ 0
RED_(8V)
$Comp
L power:GND #PWR?
U 1 1 688B4437
P 2000 6900
F 0 "#PWR?" H 2000 6650 50  0001 C CNN
F 1 "GND" H 2005 6727 50  0000 C CNN
F 2 "" H 2000 6900 50  0001 C CNN
F 3 "" H 2000 6900 50  0001 C CNN
	1    2000 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 688B50E1
P 2000 6750
F 0 "R?" H 2070 6796 50  0001 L CNN
F 1 "3.3K" V 2000 6750 50  0000 C CNN
F 2 "" V 1930 6750 50  0001 C CNN
F 3 "~" H 2000 6750 50  0001 C CNN
	1    2000 6750
	1    0    0    -1  
$EndComp
Text GLabel 2150 6450 2    50   Input ~ 0
GREEN_(Sine)
$Comp
L Device:R_POT RV?
U 1 1 688B7216
P 3250 6450
F 0 "RV?" H 3181 6496 50  0001 R CNN
F 1 "10K" V 3250 6450 50  0000 C CNN
F 2 "" H 3250 6450 50  0001 C CNN
F 3 "~" H 3250 6450 50  0001 C CNN
	1    3250 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 688B721C
P 3250 6900
F 0 "#PWR?" H 3250 6650 50  0001 C CNN
F 1 "GND" H 3255 6727 50  0000 C CNN
F 2 "" H 3250 6900 50  0001 C CNN
F 3 "" H 3250 6900 50  0001 C CNN
	1    3250 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 688B7222
P 3250 6750
F 0 "R?" H 3320 6796 50  0001 L CNN
F 1 "3.3K" V 3250 6750 50  0000 C CNN
F 2 "" V 3180 6750 50  0001 C CNN
F 3 "~" H 3250 6750 50  0001 C CNN
	1    3250 6750
	1    0    0    -1  
$EndComp
Text GLabel 3400 6450 2    50   Input ~ 0
BLUE_(Cosine)
Wire Wire Line
	1700 6100 2000 6100
Wire Wire Line
	3250 6100 3250 6300
Wire Wire Line
	2000 6300 2000 6100
Connection ~ 2000 6100
Wire Wire Line
	2000 6100 3250 6100
Wire Wire Line
	2350 4700 2650 4700
Connection ~ 2350 4700
Text GLabel 2650 4700 2    50   Input ~ 0
YELLOW_(WindSpeedPulse)
$EndSCHEMATC
