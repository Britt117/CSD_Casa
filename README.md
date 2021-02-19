# CSD_Casa
Casa Security Device - School Project for Mercer University by students from the CSD project team: Britton, Alexander and Odera. The device centers around the ESP32-cam with the HC-SR501 PIR motion sensor and KY-037 AVR PIC high sensitivity sound sensor. When the device detects motion or sound within a certain threshold, the device will take a picture. The device works with the Blynk app which will notify the user of the disturbance along with the image.

Hardware used:
-ESP32-cam AI-Thinker model with OV2640 camera and 32GB SD card
-FTDI232 USB mini adapter programming board
-HC-SR501 PIR motion sensor
-KY-037 AVR PIC high sensitivity sound sensor
-One bread board
-15 male to male wires

Note about the sound sensor:
-Turn the potentiometer clockwise untill the led2 on the sound sensor turns off and then turn it counter clockwise where it led2 comes back on, leaving both led1 and led2 on. This makes the sound sensor at its most sensitve. Furthermore, the analog pin is being used not the digital pin on the sound sensor. 

Arduino IDE needed: 
-https://www.arduino.cc/en/Main.Software

Preferences needed:
-https://dl.espressif.com/dl/package_esp32_index.json

Libraries needed:
-blynk version 0.6.1 by Volodymyr Shymanskyy

Board manager needed:
-esp32 version 1.0.4 by Espressif Systems

Tool specifications:
-Board: "ESP32 Wrover Module"
-Upload Speed: "115200"
-Flash Frequency: "40MHz"
-Partition Scheme: "HUGE APP..."
-Core Debug Level: "None"


