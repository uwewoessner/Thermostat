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
L RF_Module:ESP32-WROOM-32D U?
U 1 1 61F6C243
P 4700 2950
F 0 "U?" H 4700 4531 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 4700 4440 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 4700 1450 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 4400 3000 50  0001 C CNN
	1    4700 2950
	1    0    0    -1  
$EndComp
Text GLabel 5300 1850 2    50   Input ~ 0
USART_TX
Text GLabel 5300 2050 2    50   Input ~ 0
USART_RX
$Comp
L power:GND #PWR?
U 1 1 61F6D36B
P 4700 4650
F 0 "#PWR?" H 4700 4400 50  0001 C CNN
F 1 "GND" H 4705 4477 50  0000 C CNN
F 2 "" H 4700 4650 50  0001 C CNN
F 3 "" H 4700 4650 50  0001 C CNN
	1    4700 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61F6DEC9
P 4700 1000
F 0 "#PWR?" H 4700 850 50  0001 C CNN
F 1 "+3.3V" H 4715 1173 50  0000 C CNN
F 2 "" H 4700 1000 50  0001 C CNN
F 3 "" H 4700 1000 50  0001 C CNN
	1    4700 1000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
