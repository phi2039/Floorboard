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
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 6400 750  1200 1600
U 5358284F
F0 "IO" 50
F1 "IO_Board.sch" 50
F2 "A0" I L 6400 850 60 
F3 "A1" I L 6400 950 60 
F4 "A2" I L 6400 1050 60 
F5 "A3" I L 6400 1150 60 
F6 "ADDR_EN" I L 6400 1250 60 
F7 "SCK" I L 6400 1750 60 
F8 "MOSI" I L 6400 1550 60 
F9 "IN_LD" I L 6400 1450 60 
F10 "MISO" O L 6400 1650 60 
F11 "ANA_IN" O L 6400 1950 60 
F12 "ANA_REF" I L 6400 2050 60 
$EndSheet
$Comp
L PWR_FLAG #FLG01
U 1 1 53584928
P 600 750
F 0 "#FLG01" H 600 845 30  0001 C CNN
F 1 "PWR_FLAG" H 600 930 30  0000 C CNN
F 2 "" H 600 750 60  0000 C CNN
F 3 "" H 600 750 60  0000 C CNN
	1    600  750 
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG02
U 1 1 5358495E
P 1100 750
F 0 "#FLG02" H 1100 845 30  0001 C CNN
F 1 "PWR_FLAG" H 1100 930 30  0000 C CNN
F 2 "" H 1100 750 60  0000 C CNN
F 3 "" H 1100 750 60  0000 C CNN
	1    1100 750 
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR03
U 1 1 53584A9C
P 600 650
F 0 "#PWR03" H 600 740 20  0001 C CNN
F 1 "+5V" H 600 740 30  0000 C CNN
F 2 "" H 600 650 60  0000 C CNN
F 3 "" H 600 650 60  0000 C CNN
	1    600  650 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 53584AF8
P 1100 850
F 0 "#PWR04" H 1100 850 30  0001 C CNN
F 1 "GND" H 1100 780 30  0001 C CNN
F 2 "" H 1100 850 60  0000 C CNN
F 3 "" H 1100 850 60  0000 C CNN
	1    1100 850 
	1    0    0    -1  
$EndComp
NoConn ~ 3850 850 
NoConn ~ 3850 950 
NoConn ~ 3850 1050
NoConn ~ 3850 1150
NoConn ~ 3850 1250
NoConn ~ 3850 1450
NoConn ~ 3850 1550
NoConn ~ 3850 1650
$Comp
L Capacitor-General_EN60617-4_04-02-01_Date20121108 C4
U 1 1 535D8C8E
P 5150 3150
F 0 "C4" H 5200 3250 50  0000 L CNN
F 1 "0.1uF" H 5200 3050 50  0000 L CNN
F 2 "" H 5150 3150 60  0000 C CNN
F 3 "" H 5150 3150 60  0000 C CNN
	1    5150 3150
	1    0    0    -1  
$EndComp
$Comp
L Capacitor-General_EN60617-4_04-02-01_Date20121108 C1
U 1 1 535DA1CA
P 3700 3150
F 0 "C1" H 3750 3250 50  0000 L CNN
F 1 "0.1uF" H 3750 3050 50  0000 L CNN
F 2 "" H 3700 3150 60  0000 C CNN
F 3 "" H 3700 3150 60  0000 C CNN
	1    3700 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 535DAA2A
P 4200 3550
F 0 "#PWR05" H 4200 3550 30  0001 C CNN
F 1 "GND" H 4200 3480 30  0001 C CNN
F 2 "" H 4200 3550 60  0000 C CNN
F 3 "" H 4200 3550 60  0000 C CNN
	1    4200 3550
	1    0    0    -1  
$EndComp
$Comp
L +9V #PWR06
U 1 1 535DBF09
P 3300 2750
F 0 "#PWR06" H 3300 2720 20  0001 C CNN
F 1 "+9V" H 3300 2860 30  0000 C CNN
F 2 "" H 3300 2750 60  0000 C CNN
F 3 "" H 3300 2750 60  0000 C CNN
	1    3300 2750
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 535DC791
P 6850 2750
F 0 "D2" H 6850 2850 50  0000 C CNN
F 1 "LED" H 6850 2650 50  0000 C CNN
F 2 "" H 6850 2750 60  0000 C CNN
F 3 "" H 6850 2750 60  0000 C CNN
	1    6850 2750
	1    0    0    -1  
$EndComp
$Comp
L SWITCH_INV SW1
U 1 1 535DC859
P 5950 2750
F 0 "SW1" H 5750 2900 50  0000 C CNN
F 1 "POWER_SEL" H 5800 2600 50  0000 C CNN
F 2 "" H 5950 2750 60  0000 C CNN
F 3 "" H 5950 2750 60  0000 C CNN
	1    5950 2750
	-1   0    0    1   
$EndComp
$Sheet
S 3850 750  1350 1600
U 535D5480
F0 "MCU" 50
F1 "Arduino_UNO.sch" 50
F2 "D2" O R 5200 850 60 
F3 "D3" O R 5200 950 60 
F4 "D4" O R 5200 1050 60 
F5 "D5" O R 5200 1150 60 
F6 "D9" O R 5200 1250 60 
F7 "D10" O R 5200 1450 60 
F8 "D11" O R 5200 1550 60 
F9 "D12" I R 5200 1650 60 
F10 "D13" O R 5200 1750 60 
F11 "A0" I R 5200 1950 60 
F12 "AREF" O R 5200 2050 60 
F13 "A1" I L 3850 850 60 
F14 "A2" I L 3850 950 60 
F15 "A3" I L 3850 1050 60 
F16 "A4" I L 3850 1150 60 
F17 "A5" I L 3850 1250 60 
F18 "D6" B L 3850 1450 60 
F19 "D7" B L 3850 1550 60 
F20 "D8" B L 3850 1650 60 
F21 "USB_5V" O R 5200 2250 60 
$EndSheet
$Comp
L R R1
U 1 1 535E0AA7
P 7150 3100
F 0 "R1" V 7230 3100 40  0000 C CNN
F 1 "1K" V 7157 3101 40  0000 C CNN
F 2 "" V 7080 3100 30  0000 C CNN
F 3 "" H 7150 3100 30  0000 C CNN
	1    7150 3100
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG07
U 1 1 535E1608
P 850 750
F 0 "#FLG07" H 850 845 30  0001 C CNN
F 1 "PWR_FLAG" H 850 930 30  0000 C CNN
F 2 "" H 850 750 60  0000 C CNN
F 3 "" H 850 750 60  0000 C CNN
	1    850  750 
	-1   0    0    1   
$EndComp
$Comp
L +9V #PWR08
U 1 1 535E17E6
P 850 650
F 0 "#PWR08" H 850 620 20  0001 C CNN
F 1 "+9V" H 850 760 30  0000 C CNN
F 2 "" H 850 650 60  0000 C CNN
F 3 "" H 850 650 60  0000 C CNN
	1    850  650 
	1    0    0    -1  
$EndComp
$Comp
L LM7805CT U1
U 1 1 535D944A
P 4200 2900
F 0 "U1" H 4000 3100 40  0000 C CNN
F 1 "LM7805CT" H 4200 3100 40  0000 L CNN
F 2 "TO-220" H 4200 3000 30  0000 C CIN
F 3 "" H 4200 2900 60  0000 C CNN
	1    4200 2900
	1    0    0    -1  
$EndComp
$Comp
L DIODE D6
U 1 1 535E246D
P 2400 3050
F 0 "D6" H 2400 2950 40  0000 C CNN
F 1 "1N4007" H 2400 3150 40  0000 C CNN
F 2 "" H 2400 3050 60  0000 C CNN
F 3 "" H 2400 3050 60  0000 C CNN
	1    2400 3050
	0    -1   -1   0   
$EndComp
$Comp
L DIODE D4
U 1 1 535E2594
P 2200 2850
F 0 "D4" H 2200 2750 40  0000 C CNN
F 1 "1N4007" H 2200 2950 40  0000 C CNN
F 2 "" H 2200 2850 60  0000 C CNN
F 3 "" H 2200 2850 60  0000 C CNN
	1    2200 2850
	1    0    0    -1  
$EndComp
$Comp
L DIODE D5
U 1 1 535E26BB
P 2200 3250
F 0 "D5" H 2200 3150 40  0000 C CNN
F 1 "1N4007" H 2200 3350 40  0000 C CNN
F 2 "" H 2200 3250 60  0000 C CNN
F 3 "" H 2200 3250 60  0000 C CNN
	1    2200 3250
	1    0    0    -1  
$EndComp
Text Label 1800 3550 2    60   ~ 0
VIN_0
$Comp
L DIODE D1
U 1 1 535E2E1E
P 2000 3050
F 0 "D1" H 2000 2950 40  0000 C CNN
F 1 "1N4007" H 2000 3150 40  0000 C CNN
F 2 "" H 2000 3050 60  0000 C CNN
F 3 "" H 2000 3050 60  0000 C CNN
	1    2000 3050
	0    -1   -1   0   
$EndComp
Text Label 1550 2850 0    60   ~ 0
VIN_1
Text Label 2750 3450 2    60   ~ 0
GND
Text Label 2750 2850 2    60   ~ 0
VOUT
$Comp
L Capacitor-Polarized_EN60617-4_04-02-05_Date20121108 C2
U 1 1 535E4DC5
P 2900 3150
F 0 "C2" H 2930 3245 50  0000 L CNN
F 1 "1000uF/25V" H 2950 3050 50  0000 L CNN
F 2 "" H 2900 3150 60  0000 C CNN
F 3 "" H 2900 3150 60  0000 C CNN
	1    2900 3150
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P26
U 1 1 535E7200
P 1000 2950
F 0 "P26" V 950 2950 40  0000 C CNN
F 1 "CONN_2" V 1050 2950 40  0000 C CNN
F 2 "" H 1000 2950 60  0000 C CNN
F 3 "" H 1000 2950 60  0000 C CNN
	1    1000 2950
	-1   0    0    1   
$EndComp
$Comp
L Capacitor-Polarized_EN60617-4_04-02-05_Date20121108 C3
U 1 1 535D8ABF
P 4700 3150
F 0 "C3" H 4730 3245 50  0000 L CNN
F 1 "100uF/6V" H 4700 3050 50  0000 L CNN
F 2 "" H 4700 3150 60  0000 C CNN
F 3 "" H 4700 3150 60  0000 C CNN
	1    4700 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 535E0DAA
P 7150 3450
F 0 "#PWR09" H 7150 3450 30  0001 C CNN
F 1 "GND" H 7150 3380 30  0001 C CNN
F 2 "" H 7150 3450 60  0000 C CNN
F 3 "" H 7150 3450 60  0000 C CNN
	1    7150 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 850  6400 850 
Wire Wire Line
	6400 950  5200 950 
Wire Wire Line
	5200 1050 6400 1050
Wire Wire Line
	5200 1150 6400 1150
Wire Wire Line
	5200 1250 6400 1250
Wire Wire Line
	5200 1550 6400 1550
Wire Wire Line
	6400 1650 5200 1650
Wire Wire Line
	5200 1750 6400 1750
Wire Wire Line
	5200 1950 6400 1950
Wire Wire Line
	5200 1450 6400 1450
Wire Wire Line
	1100 850  1100 750 
Wire Wire Line
	600  750  600  650 
Wire Wire Line
	5200 2050 6400 2050
Wire Wire Line
	4200 3150 4200 3550
Connection ~ 4200 3450
Wire Wire Line
	4600 2850 5450 2850
Wire Wire Line
	5150 2850 5150 2950
Wire Wire Line
	4700 2950 4700 2850
Connection ~ 4700 2850
Wire Wire Line
	3700 2850 3700 2950
Connection ~ 3700 2850
Wire Wire Line
	6450 2750 6650 2750
Connection ~ 5150 2850
Wire Wire Line
	5450 2650 5350 2650
Wire Wire Line
	5350 2650 5350 2250
Wire Wire Line
	5350 2250 5200 2250
Wire Wire Line
	7150 3450 7150 3350
Wire Wire Line
	7150 2850 7150 2750
Wire Wire Line
	7150 2750 7050 2750
Wire Wire Line
	850  650  850  750 
Wire Wire Line
	2400 3250 2400 3550
Wire Wire Line
	1350 2850 2000 2850
Wire Wire Line
	2000 3250 2000 3250
Wire Wire Line
	2400 3550 1450 3550
Wire Wire Line
	2000 3250 2000 3450
Wire Wire Line
	2900 2950 2900 2850
Wire Wire Line
	2900 3350 2900 3450
Wire Wire Line
	1450 3550 1450 3050
Wire Wire Line
	1450 3050 1350 3050
Wire Wire Line
	5150 3450 5150 3350
Wire Wire Line
	4700 3450 4700 3350
Connection ~ 4700 3450
Wire Wire Line
	3700 3450 3700 3350
Connection ~ 2400 2850
Connection ~ 2000 3250
Wire Wire Line
	3300 2850 3300 2750
Wire Wire Line
	2400 2850 3800 2850
Connection ~ 2900 2850
Connection ~ 3300 2850
Wire Wire Line
	2000 3450 5150 3450
Connection ~ 2900 3450
Connection ~ 3700 3450
$EndSCHEMATC
