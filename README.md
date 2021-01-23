# CSD_Casa
Casa Security Device - School Project for Mercer University. The device centers around the ESP32-cam with the HC-SR501 PIR motion sensor and KY-038 AVR PIC high sensitivity sound sensor. When the device detects motion or sound within a certain threshold, the device will take a picture. The device works with the Blynk app which will notify the user of the disturbance along with the image.

Hardware used:
-ESP32-cam AI-Thinker model with OV2640 camera and 32GB SD card
-FTDI232 programming board
-HC-SR501 PIR motion sensor
-KY-038 AVR PIC high sensitivity sound sensor
-One bread board
-15 male to male wires

Note about the sound sensor:
-Turn the senstivity calibrater counter clock wise till the led2 on the sound sensor turns off, leaving only the led1 on. This makes the sound sensor active high.

Librarys needed:
-blynk version 0.6.1 by Volodymyr Shymanskyy

Board manager needed:
-esp32 version 1.0.4 by Espressif Systems

Tool specifications:
-Board: "ESP32 Wrover Module"
-Upload Speed: "115200"
-Flash Frequency: "40MHz"
-Partition Scheme: "HUGE APP..."
-Core Debug Level: "None"
