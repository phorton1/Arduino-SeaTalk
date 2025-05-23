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
P 7350 2650
F 0 "J?" H 7268 2125 50  0001 C CNN
F 1 "Seatalk Alarm Cable from E80" H 8150 2200 50  0000 C CNN
F 2 "" H 7350 2650 50  0001 C CNN
F 3 "~" H 7350 2650 50  0001 C CNN
	1    7350 2650
	1    0    0    1   
$EndComp
Text Notes 7600 2900 0    63   ~ 0
brown\nwhite\nyellow\nblack\nred\nshield
Text Notes 8000 2900 0    63   ~ 0
unused alarm return\nunused alarm\nseatalk TX\nseatalk RX (except it's not)\n12V (except it's not)\nGND (excpt it's not)
$Comp
L power:+12V #PWR?
U 1 1 681E9B5C
P 7150 1850
F 0 "#PWR?" H 7150 1700 50  0001 C CNN
F 1 "+12V" H 7165 2023 50  0000 C CNN
F 2 "" H 7150 1850 50  0001 C CNN
F 3 "" H 7150 1850 50  0001 C CNN
	1    7150 1850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 681EA407
P 5100 1900
F 0 "#PWR?" H 5100 1750 50  0001 C CNN
F 1 "+3V3" H 5115 2073 50  0000 C CNN
F 2 "" H 5100 1900 50  0001 C CNN
F 3 "" H 5100 1900 50  0001 C CNN
	1    5100 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 681EA805
P 6900 3450
F 0 "#PWR?" H 6900 3200 50  0001 C CNN
F 1 "GND" H 6905 3277 50  0000 C CNN
F 2 "" H 6900 3450 50  0001 C CNN
F 3 "" H 6900 3450 50  0001 C CNN
	1    6900 3450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 681EAC8B
P 7350 1850
F 0 "J?" H 7430 1842 50  0001 L CNN
F 1 "Power Supply" H 7430 1796 50  0000 L CNN
F 2 "" H 7350 1850 50  0001 C CNN
F 3 "~" H 7350 1850 50  0001 C CNN
	1    7350 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 681F1D31
P 7150 1950
F 0 "#PWR?" H 7150 1700 50  0001 C CNN
F 1 "GND" H 7155 1777 50  0000 C CNN
F 2 "" H 7150 1950 50  0001 C CNN
F 3 "" H 7150 1950 50  0001 C CNN
	1    7150 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 681F5B40
P 2450 1700
F 0 "#PWR?" H 2450 1450 50  0001 C CNN
F 1 "GND" V 2450 1500 50  0000 C CNN
F 2 "" H 2450 1700 50  0001 C CNN
F 3 "" H 2450 1700 50  0001 C CNN
	1    2450 1700
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 681F6BDE
P 2550 1700
F 0 "#PWR?" H 2550 1550 50  0001 C CNN
F 1 "+3V3" V 2550 1900 50  0000 C CNN
F 2 "" H 2550 1700 50  0001 C CNN
F 3 "" H 2550 1700 50  0001 C CNN
	1    2550 1700
	1    0    0    -1  
$EndComp
Text GLabel 3450 1700 1    63   Input ~ 0
RXD
Text GLabel 3550 1700 1    63   Input ~ 0
TXD
$Comp
L 0_my_teensy:myTeensy4.0 U?
U 1 1 681E4C83
P 2900 2700
F 0 "U?" H 3728 2703 60  0001 L CNN
F 1 "myTeensy4.0" H 2750 2600 60  0000 L CNN
F 2 "" V 3800 2750 60  0000 C CNN
F 3 "" V 3800 2750 60  0000 C CNN
	1    2900 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 2650 6900 2650
Wire Wire Line
	6900 2650 6900 2750
$Comp
L power:+12V #PWR?
U 1 1 6820AAC2
P 6000 1900
F 0 "#PWR?" H 6000 1750 50  0001 C CNN
F 1 "+12V" H 6015 2073 50  0000 C CNN
F 2 "" H 6000 1900 50  0001 C CNN
F 3 "" H 6000 1900 50  0001 C CNN
	1    6000 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2550 5850 2550
Wire Wire Line
	5850 2350 6000 2350
Wire Wire Line
	5250 2550 5100 2550
$Comp
L Device:R R?
U 1 1 6822159F
P 5100 2050
F 0 "R?" H 5170 2096 50  0001 L CNN
F 1 "4.7K" V 5100 2050 50  0000 C CNN
F 2 "" V 5030 2050 50  0001 C CNN
F 3 "~" H 5100 2050 50  0001 C CNN
	1    5100 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2350 5100 2350
Wire Wire Line
	5100 2350 5100 2200
Text GLabel 4750 2350 0    63   Input ~ 0
RXD
Wire Wire Line
	4750 2350 5100 2350
Connection ~ 5100 2350
$Comp
L Isolator:TLP291 U?
U 1 1 6822E37B
P 5550 2450
F 0 "U?" H 5550 2775 50  0001 C CNN
F 1 "TLP521-1" H 5550 2683 50  0000 C CNN
F 2 "Package_SO:SOIC-4_4.55x2.6mm_P1.27mm" H 5350 2250 50  0001 L CIN
F 3 "https://toshiba.semicon-storage.com/info/docget.jsp?did=12884&prodName=TLP291" H 5550 2450 50  0001 L CNN
	1    5550 2450
	-1   0    0    -1  
$EndComp
$Comp
L Isolator:TLP291 U?
U 1 1 68235388
P 5550 3150
F 0 "U?" H 5550 3475 50  0001 C CNN
F 1 "TLP521-1" H 5550 3383 50  0000 C CNN
F 2 "Package_SO:SOIC-4_4.55x2.6mm_P1.27mm" H 5350 2950 50  0001 L CIN
F 3 "https://toshiba.semicon-storage.com/info/docget.jsp?did=12884&prodName=TLP291" H 5550 3150 50  0001 L CNN
	1    5550 3150
	1    0    0    -1  
$EndComp
Text GLabel 4750 3250 0    63   Input ~ 0
TXD
$Comp
L Device:R R?
U 1 1 68236F3F
P 5000 3050
F 0 "R?" H 5070 3096 50  0001 L CNN
F 1 "220" V 5000 3050 50  0000 C CNN
F 2 "" V 4930 3050 50  0001 C CNN
F 3 "~" H 5000 3050 50  0001 C CNN
	1    5000 3050
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 68244B74
P 4750 2950
F 0 "#PWR?" H 4750 2800 50  0001 C CNN
F 1 "+3V3" H 4765 3123 50  0000 C CNN
F 2 "" H 4750 2950 50  0001 C CNN
F 3 "" H 4750 2950 50  0001 C CNN
	1    4750 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 68211A3C
P 6150 2550
F 0 "R?" H 6220 2596 50  0001 L CNN
F 1 "4.7K" V 6150 2550 50  0000 C CNN
F 2 "" V 6080 2550 50  0001 C CNN
F 3 "~" H 6150 2550 50  0001 C CNN
	1    6150 2550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5250 3050 5150 3050
Wire Wire Line
	4850 3050 4750 3050
Wire Wire Line
	4750 3050 4750 2950
Wire Wire Line
	6300 2550 6600 2550
Wire Wire Line
	6000 2350 6000 1900
Wire Wire Line
	5100 2550 5100 2750
Wire Wire Line
	5100 2750 6900 2750
Connection ~ 6900 2750
Wire Wire Line
	6900 2750 6900 3250
Wire Wire Line
	5850 3250 6900 3250
Connection ~ 6900 3250
Wire Wire Line
	6900 3250 6900 3450
Wire Wire Line
	4750 3250 5250 3250
Wire Wire Line
	5850 3050 6600 3050
Wire Wire Line
	6600 3050 6600 2550
Connection ~ 6600 2550
Wire Wire Line
	6600 2550 7150 2550
$EndSCHEMATC
