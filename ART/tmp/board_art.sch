EESchema Schematic File Version 4
LIBS:power
LIBS:74xx
EELAYER 26 0
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
L Device:R R1
U 1 1 5B91BA71
P 4510 2300
F 0 "R1" V 4303 2300 50  0000 C CNN
F 1 "R" V 4394 2300 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4440 2300 50  0001 C CNN
F 3 "~" H 4510 2300 50  0001 C CNN
	1    4510 2300
	0    1    1    0   
$EndComp
$Comp
L Device:LED D1
U 1 1 5B91BB2D
P 4810 2460
F 0 "D1" V 4848 2343 50  0000 R CNN
F 1 "LED" V 4757 2343 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric" H 4810 2460 50  0001 C CNN
F 3 "~" H 4810 2460 50  0001 C CNN
	1    4810 2460
	0    -1   -1   0   
$EndComp
$Comp
L Device:Battery_Cell BT1
U 1 1 5B91BC16
P 4050 2510
F 0 "BT1" H 4168 2606 50  0000 L CNN
F 1 "Battery_Cell" H 4168 2515 50  0000 L CNN
F 2 "Battery:BatteryHolder_Keystone_106_1x20mm" V 4050 2570 50  0001 C CNN
F 3 "~" V 4050 2570 50  0001 C CNN
	1    4050 2510
	1    0    0    -1  
$EndComp
Wire Wire Line
	4360 2300 4050 2300
Wire Wire Line
	4050 2300 4050 2310
Wire Wire Line
	4660 2300 4810 2300
Wire Wire Line
	4810 2300 4810 2310
Wire Wire Line
	4810 2610 4050 2610
Text Label 4200 2610 0    50   ~ 0
GND
$EndSCHEMATC
