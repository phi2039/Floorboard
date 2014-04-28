EESchema Schematic File Version 2
LIBS:power
LIBS:74xgxx
LIBS:ac-dc
LIBS:adc-dac
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:brooktre
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:conn
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:linear
LIBS:logo
LIBS:memory
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip
LIBS:microchip1
LIBS:microcontrollers
LIBS:motorola
LIBS:msp430
LIBS:nxp_armmcu
LIBS:opto
LIBS:phi2039
LIBS:philips
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:Resonator_3pins
LIBS:sensors
LIBS:siliconi
LIBS:special
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:texas
LIBS:transf
LIBS:transistors
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:xilinx
LIBS:Symbols_DCDC-ACDC-Converter_RevC_20Jul2012
LIBS:Symbols_EN60617_13Mar2013
LIBS:Symbols_ICs-Diskrete_RevD6
LIBS:Symbols_ICs-Opto_RevB_29Dez2012
LIBS:Symbols_Socket-DIN41612_RevA
LIBS:Symbols_Transformer-Diskrete_RevA
LIBS:SymbolsSimilarEN60617+oldDIN617-RevE8
LIBS:modified
LIBS:74xx
LIBS:Floorboard-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L GND #PWR042
U 1 1 535D55D8
P 1900 4100
F 0 "#PWR042" H 1900 4100 30  0001 C CNN
F 1 "GND" H 1900 4030 30  0001 C CNN
F 2 "" H 1900 4100 60  0000 C CNN
F 3 "" H 1900 4100 60  0000 C CNN
	1    1900 4100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR043
U 1 1 535D5743
P 1900 1600
F 0 "#PWR043" H 1900 1690 20  0001 C CNN
F 1 "+5V" H 1900 1690 30  0000 C CNN
F 2 "" H 1900 1600 60  0000 C CNN
F 3 "" H 1900 1600 60  0000 C CNN
	1    1900 1600
	1    0    0    -1  
$EndComp
Text HLabel 4050 3500 2    60   BiDi ~ 0
D2
Text HLabel 4050 3600 2    60   BiDi ~ 0
D3
Text HLabel 4050 3700 2    60   BiDi ~ 0
D4
Text HLabel 4050 3800 2    60   BiDi ~ 0
D5
Text HLabel 4050 1800 2    60   BiDi ~ 0
D9
Text HLabel 4050 1900 2    60   BiDi ~ 0
D10
Text HLabel 6250 2000 2    60   BiDi ~ 0
D11
Text HLabel 6250 2100 2    60   BiDi ~ 0
D12
Text HLabel 6250 2200 2    60   BiDi ~ 0
D13
Text HLabel 4050 2550 2    60   Input ~ 0
A0
$Comp
L CONN_3X2 P24
U 1 1 535D873D
P 5300 2650
F 0 "P24" H 5300 2900 50  0000 C CNN
F 1 "CONN_3X2" V 5300 2700 40  0000 C CNN
F 2 "" H 5300 2650 60  0000 C CNN
F 3 "" H 5300 2650 60  0000 C CNN
	1    5300 2650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR044
U 1 1 535D874F
P 5800 2400
F 0 "#PWR044" H 5800 2490 20  0001 C CNN
F 1 "+5V" H 5800 2490 30  0000 C CNN
F 2 "" H 5800 2400 60  0000 C CNN
F 3 "" H 5800 2400 60  0000 C CNN
	1    5800 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR045
U 1 1 535D875A
P 5800 2800
F 0 "#PWR045" H 5800 2800 30  0001 C CNN
F 1 "GND" H 5800 2730 30  0001 C CNN
F 2 "" H 5800 2800 60  0000 C CNN
F 3 "" H 5800 2800 60  0000 C CNN
	1    5800 2800
	1    0    0    -1  
$EndComp
Text HLabel 1950 2300 0    60   Output ~ 0
AREF
$Comp
L CONN_6 P25
U 1 1 535D8E6D
P 5850 3350
F 0 "P25" V 5800 3350 60  0000 C CNN
F 1 "CONN_6" V 5900 3350 60  0000 C CNN
F 2 "" H 5850 3350 60  0000 C CNN
F 3 "" H 5850 3350 60  0000 C CNN
	1    5850 3350
	1    0    0    -1  
$EndComp
Text Notes 5950 3750 2    60   ~ 0
FTDI
$Comp
L GND #PWR046
U 1 1 535D90FA
P 4900 3700
F 0 "#PWR046" H 4900 3700 30  0001 C CNN
F 1 "GND" H 4900 3630 30  0001 C CNN
F 2 "" H 4900 3700 60  0000 C CNN
F 3 "" H 4900 3700 60  0000 C CNN
	1    4900 3700
	1    0    0    -1  
$EndComp
Text Label 5500 3400 2    60   ~ 0
TX
Text Label 5500 3500 2    60   ~ 0
RX
Text Label 5500 3600 2    60   ~ 0
RTS
Text Notes 5400 2850 2    60   ~ 0
ICSP
$Comp
L ATMEGA328P-P IC1
U 1 1 535D55C4
P 2950 2800
F 0 "IC1" H 2200 4050 40  0000 L BNN
F 1 "ATMEGA328P-P" H 3350 1400 40  0000 L BNN
F 2 "DIL28" H 2950 2800 30  0000 C CIN
F 3 "" H 2950 2800 60  0000 C CNN
	1    2950 2800
	1    0    0    -1  
$EndComp
$Comp
L Capacitor-General_EN60617-4_04-02-01_Date20121108 C7
U 1 1 535DAAEE
P 5150 1500
F 0 "C7" H 5200 1600 50  0000 L CNN
F 1 "22pf" H 4950 1600 50  0000 L CNN
F 2 "" H 5150 1500 60  0000 C CNN
F 3 "" H 5150 1500 60  0000 C CNN
	1    5150 1500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR047
U 1 1 535DABD0
P 5350 1700
F 0 "#PWR047" H 5350 1700 30  0001 C CNN
F 1 "GND" H 5350 1630 30  0001 C CNN
F 2 "" H 5350 1700 60  0000 C CNN
F 3 "" H 5350 1700 60  0000 C CNN
	1    5350 1700
	1    0    0    -1  
$EndComp
$Comp
L Capacitor-General_EN60617-4_04-02-01_Date20121108 C5
U 1 1 535DB1BA
P 4600 3600
F 0 "C5" V 4550 3700 50  0000 L CNN
F 1 "0.1uF" V 4550 3350 50  0000 L CNN
F 2 "" H 4600 3600 60  0000 C CNN
F 3 "" H 4600 3600 60  0000 C CNN
	1    4600 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2050 4000 1900 4000
Wire Wire Line
	1900 3900 1900 4100
Wire Wire Line
	1900 3900 2050 3900
Connection ~ 1900 4000
Wire Wire Line
	1900 1600 1900 2000
Wire Wire Line
	1900 1700 2050 1700
Wire Wire Line
	1900 2000 2050 2000
Connection ~ 1900 1700
Wire Wire Line
	5800 2800 5800 2700
Wire Wire Line
	5800 2700 5700 2700
Wire Wire Line
	5700 2500 5800 2500
Wire Wire Line
	5800 2500 5800 2400
Wire Wire Line
	3950 2000 6250 2000
Wire Wire Line
	3950 2100 6250 2100
Wire Wire Line
	3950 2200 6250 2200
Wire Wire Line
	4900 2500 4900 2100
Connection ~ 4900 2100
Wire Wire Line
	5700 2600 5900 2600
Wire Wire Line
	5900 2600 5900 2000
Connection ~ 5900 2000
Wire Wire Line
	4900 2600 4800 2600
Wire Wire Line
	4800 2600 4800 2200
Connection ~ 4800 2200
Wire Wire Line
	4800 3150 4800 2700
Wire Wire Line
	4800 2700 4900 2700
Wire Wire Line
	5500 3100 4900 3100
Wire Wire Line
	4900 3100 4900 3700
Wire Wire Line
	5500 3200 4900 3200
Connection ~ 4900 3200
Wire Wire Line
	3950 3300 4800 3300
Wire Wire Line
	4800 3300 4800 3500
Wire Wire Line
	4800 3500 5500 3500
Wire Wire Line
	3950 3400 5500 3400
Wire Wire Line
	3950 3150 4800 3150
Wire Wire Line
	3950 2300 4600 2300
Wire Wire Line
	4600 900  4950 900 
Wire Wire Line
	4700 1500 4950 1500
Wire Wire Line
	4700 1500 4700 2400
Wire Wire Line
	4700 2400 3950 2400
Connection ~ 4850 900 
Connection ~ 4850 1500
Wire Wire Line
	4600 2300 4600 900 
Connection ~ 5350 1500
Wire Wire Line
	4400 3150 4400 3800
Connection ~ 4400 3150
Wire Wire Line
	4800 3600 5500 3600
$Comp
L SW_PUSH SW2
U 1 1 535DB610
P 4400 4100
F 0 "SW2" H 4550 4210 50  0000 C CNN
F 1 "SW_PUSH" H 4400 4020 50  0000 C CNN
F 2 "" H 4400 4100 60  0000 C CNN
F 3 "" H 4400 4100 60  0000 C CNN
	1    4400 4100
	0    1    1    0   
$EndComp
Connection ~ 4400 3600
$Comp
L GND #PWR048
U 1 1 535DB671
P 4400 4500
F 0 "#PWR048" H 4400 4500 30  0001 C CNN
F 1 "GND" H 4400 4430 30  0001 C CNN
F 2 "" H 4400 4500 60  0000 C CNN
F 3 "" H 4400 4500 60  0000 C CNN
	1    4400 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 4500 4400 4400
$Comp
L R R10
U 1 1 535DB823
P 4300 2900
F 0 "R10" V 4380 2900 40  0000 C CNN
F 1 "10K" V 4307 2901 40  0000 C CNN
F 2 "" V 4230 2900 30  0000 C CNN
F 3 "" H 4300 2900 30  0000 C CNN
	1    4300 2900
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR049
U 1 1 535DB86E
P 4450 2550
F 0 "#PWR049" H 4450 2640 20  0001 C CNN
F 1 "+5V" H 4450 2640 30  0000 C CNN
F 2 "" H 4450 2550 60  0000 C CNN
F 3 "" H 4450 2550 60  0000 C CNN
	1    4450 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2550 4450 2650
Text HLabel 4050 2650 2    60   Input ~ 0
A1
Text HLabel 4050 2750 2    60   Input ~ 0
A2
Text HLabel 4050 2850 2    60   Input ~ 0
A3
Text HLabel 4050 2950 2    60   Input ~ 0
A4
Text HLabel 4050 3050 2    60   Input ~ 0
A5
Text HLabel 4050 3900 2    60   BiDi ~ 0
D6
Text HLabel 4050 4000 2    60   BiDi ~ 0
D7
Text HLabel 4050 1700 2    60   BiDi ~ 0
D8
Connection ~ 4300 3150
Wire Wire Line
	3950 2650 4050 2650
Wire Wire Line
	4050 2550 3950 2550
Wire Wire Line
	3950 2750 4050 2750
Wire Wire Line
	4050 2850 3950 2850
Wire Wire Line
	3950 2950 4050 2950
Wire Wire Line
	4050 3050 3950 3050
Wire Wire Line
	3950 3500 4050 3500
Wire Wire Line
	4050 3600 3950 3600
Wire Wire Line
	3950 3700 4050 3700
Wire Wire Line
	4050 3800 3950 3800
Wire Wire Line
	3950 3900 4050 3900
Wire Wire Line
	4050 4000 3950 4000
Wire Wire Line
	4050 1700 3950 1700
Wire Wire Line
	3950 1800 4050 1800
Wire Wire Line
	4050 1900 3950 1900
Wire Wire Line
	1950 2300 2050 2300
Text HLabel 5350 3300 0    60   Output ~ 0
USB_5V
Wire Wire Line
	5500 3300 5350 3300
Text Label 5500 3200 2    60   ~ 0
CTS
$Comp
L R R11
U 1 1 535E3E3E
P 6100 2450
F 0 "R11" V 6180 2450 40  0000 C CNN
F 1 "1K" V 6107 2451 40  0000 C CNN
F 2 "" V 6030 2450 30  0000 C CNN
F 3 "" H 6100 2450 30  0000 C CNN
	1    6100 2450
	1    0    0    -1  
$EndComp
$Comp
L LED D3
U 1 1 535E3E55
P 6400 2700
F 0 "D3" H 6400 2800 50  0000 C CNN
F 1 "L" H 6400 2600 50  0000 C CNN
F 2 "" H 6400 2700 60  0000 C CNN
F 3 "" H 6400 2700 60  0000 C CNN
	1    6400 2700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR050
U 1 1 535E3E67
P 6700 2800
F 0 "#PWR050" H 6700 2800 30  0001 C CNN
F 1 "GND" H 6700 2730 30  0001 C CNN
F 2 "" H 6700 2800 60  0000 C CNN
F 3 "" H 6700 2800 60  0000 C CNN
	1    6700 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2700 6700 2700
Wire Wire Line
	6700 2700 6700 2800
Wire Wire Line
	6100 2700 6200 2700
Connection ~ 6100 2200
Wire Wire Line
	5350 900  5350 1700
$Comp
L Capacitor-General_EN60617-4_04-02-01_Date20121108 C6
U 1 1 535DAAC1
P 5150 900
F 0 "C6" H 5200 1000 50  0000 L CNN
F 1 "22pf" H 4950 1000 50  0000 L CNN
F 2 "" H 5150 900 60  0000 C CNN
F 3 "" H 5150 900 60  0000 C CNN
	1    5150 900 
	0    1    1    0   
$EndComp
$Comp
L DIODE D8
U 1 1 535FA04B
P 4600 2850
F 0 "D8" H 4600 2950 40  0000 C CNN
F 1 "1N4001" H 4600 2750 40  0000 C CNN
F 2 "" H 4600 2850 60  0000 C CNN
F 3 "" H 4600 2850 60  0000 C CNN
	1    4600 2850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4600 3050 4600 3150
Connection ~ 4600 3150
Wire Wire Line
	4300 2650 4600 2650
Connection ~ 4450 2650
$Comp
L CRYSTAL X1
U 1 1 535FCF6A
P 4850 1200
F 0 "X1" H 4850 1350 60  0000 C CNN
F 1 "16.00MHz" H 4850 1050 60  0000 C CNN
F 2 "" H 4850 1200 60  0000 C CNN
F 3 "" H 4850 1200 60  0000 C CNN
	1    4850 1200
	0    1    1    0   
$EndComp
$EndSCHEMATC