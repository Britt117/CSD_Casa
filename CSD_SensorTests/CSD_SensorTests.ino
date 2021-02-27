////////////////////////////////////////////////////////////////////////////////////////////
/* 
 *                    Casa Security Device Motion and Sound Sensor Test
 *          Developed by the CSD Project Team for senior design at Mercer University
 *                                  
 *                    Testing the motion and sound sensor with the ESP32
 * When the sensors go high, the esp32's flash lamp will go high, indicating a disturbance detected.
 *                  
 *                  *******************IMPORTANT*************************
 *                                 Tool specifications:
 *                             Board: "ESP32 Wrover Module"
 *                             Upload Speed: "115200"
 *                             Flash Frequency: "40MHz"
 *                             Partition Scheme: "HUGE APP..."
 *                             Core Debug Level: "None"
 *                             
 *                                Preferences needed:
 *                 https://dl.espressif.com/dl/package_esp32_index.json
 *                                  
 *                                  Board Manager: 
 *                       esp32 version 1.0.4 by Espressif Systems
*/
////////////////////////////////////////////////////////////////////////////////////////////

#include "esp_camera.h"

// Defining ESP32 model and its pins
#define CAMERA_MODEL_AI_THINKER              // Your ESP32-Cam's model of which has PSRAM
#include "camera_pins.h"

// Defining I/O pins
#define Motion 12                            // HC-SR501 PIR Motion Sensor controlled with pin 12
#define Sound 13                             // KY-038 AVR PIC high sensitivity sound sensor controlled with pin 13
#define Lamp 4                               // ESP32's on board flash lamp controlled with pin 4



void setup() 
{
  Serial.begin(115200);                      // Serial monitor set to 115200 Baud
  
  pinMode(Lamp,OUTPUT);                      // Setting flash lamp as an output
  pinMode(Motion,INPUT);                     // Setting motion sensor as an input
  pinMode(Sound,INPUT);                      // Setting sound sensor as an input
  
  Serial.setDebugOutput(true);
  Serial.println();
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  Serial.println("Setup Complete");
}



void loop() 
{
  /* 
   *  Comment out the if statment test you do not wish to perform and
   *  uncomment the if statment test you do wish to perform
  */
  
  //if ( digitalRead(Motion) == HIGH )                                       // Tests only the motion sensor
  //if ( digitalRead(Sound) == HIGH )                                        // Tests only the sound sensor
  if ( (digitalRead(Motion) == HIGH) || (digitalRead(Sound) == HIGH) )       // Tests both sensors together
  {
    Serial.println("Disturbance Detected");
    digitalWrite(Lamp, HIGH);
    delay(1000);
    digitalWrite(Lamp, LOW);
  }
}
