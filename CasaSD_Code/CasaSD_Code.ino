//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
 *                          Casa Security Device Prototybe Code with Blynk
 *              Developed by the CSD Project Team for senior design at Mercer University
 *                                  
 *                    ESP32 connects to user's WiFi and listens to its sensors.
 *                     When the sensors go high the ESP32 will take a picture.
 *            The picture is uploaded to the webserver and is displayed in the Blynk app.
 *                 The Blynk will send notification to the user's phone and email.
 *  The user will be able to press a button in the Blynk app that will make the device take a picture.
 *                  
 *                    *******************IMPORTANT*************************
 *                                  Tool specifications:
 *                               Board: "ESP32 Wrover Module"
 *                               Upload Speed: "115200"
 *                               Flash Frequency: "40MHz"
 *                               Partition Scheme: "HUGE APP..."
 *                               Core Debug Level: "None"
 *                             
 *                                     Board Manager: 
 *                           esp32 version 1.0.4 by Espressif Systems
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                                            // SD Card ESP32
#include "SD_MMC.h"                                        // SD Card ESP32
#include "soc/soc.h"                                       // Disable brownout problems
#include "soc/rtc_cntl_reg.h"                              // Disable brownout problems
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>                            

// Global varables
const char* ssid = "********";                             // Your Wifi name, ex: NETGEAR57
const char* password = "********";                         // Password to your Wifi, ex: fluffytrumpet252
char auth[] = "********";                                  // Authentication code sent by Blynk, check email
//const char* myEmail = "********";                        // Input your prefered email for Blynk notifications via email

// Defining ESP32 model and its pins
#define CAMERA_MODEL_AI_THINKER                            // Your ESP32-Cam's model of which has PSRAM
#include "camera_pins.h"                                   // This library has to be called after the ESP32's model has been defined

// Defining I/O pins
#define Motion 12                                          // HC-SR501 PIR Motion Sensor controlled with pin 12
#define Sound 13                                           // KY-038 AVR PIC high sensitivity sound sensor controlled with pin 13
#define Lamp 4                                             // ESP32's on board flash lamp controlled with pin 4
#define PHOTO 14                                           // OV2640 Camera controlled with pin 14

String local_IP;                                           // Your IP string varible
void startCameraServer();                                  // The camera server where the photos will be taken



void takePhoto()                                           // Functionn for taking photo
{
  digitalWrite(Lamp, HIGH);                
  delay(200);
  uint32_t randomNum = random(50000);
  
  // Stating where the image is being captured
  Serial.println("http://"+local_IP+"/capture?_cb="+ (String)randomNum); 

  // Setting image propeertites and passing image to the Blynk app
  Blynk.setProperty(V1, "urls", "http://"+local_IP+"/capture?_cb="+(String)randomNum);
  
  digitalWrite(Lamp, LOW);                 
  delay(1000);
}                                                          // End takePhoto function



void setup()                                               // Setup function
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);               // Disable brown out detector
  
  Serial.begin(115200);                                    // Serial monitor set to 115200 Baud
  
  pinMode(Lamp,OUTPUT);                                    // Setting flash lamp as an output
  pinMode(PHOTO,OUTPUT);                                   // Setting Camera as an Output
  pinMode(Motion,INPUT);                                   // Setting motion sensor as an input
  pinMode(Sound,INPUT);                                    // Setting sound sensor as an input
  
  Serial.setDebugOutput(true);
  Serial.println();

  // Configuring the ESP32 pins
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
  
  // If PSRAM IC present, init with UXGA resolution and higher JPEG quality
  // for larger pre-allocated frame buffer.
  if(psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } 
  else 
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Camera initilizing and error catching
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();                   // Get pointer to a frame buffer and return the pointer to the frame buffer
  
  // Initial sensors are flipped vertically and colors are a little saturated and dim,
  // thus we correct it via the following lines
  if (s->id.PID == OV2640_PID)
  {
    s->set_vflip(s, 1);                                     // Fliping the image 
    s->set_brightness(s, 1);                                // Raising the brightness 
    s->set_saturation(s, -2);                               // Lowering the saturation
  }
  
  s->set_framesize(s, FRAMESIZE_QVGA);                      // Drop down frame size for higher initial frame rate

  WiFi.begin(ssid, password);                               // Getting WiFi your info

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");                          
  }
  Serial.println("");
  Serial.println("WiFi connected");                        // WiFi succefully connected

  startCameraServer();                                     // Calling startCameraServer function 

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());                            // Getting your IP
  local_IP = WiFi.localIP().toString();                    // Assigning your IP to the local_IP variable
  Serial.println("' to connect");
  Serial.println("");
  
  Blynk.begin(auth, ssid, password);                       // Getting Blynk connected

  Serial.println("Setup Complete");
  Serial.println("");
}                                                          // End setup function



void loop()                                                // Loop function
{
  // Main code which runs continuously:
  //local_IP = WiFi.localIP().toString();
  
  Blynk.run();                                             // Running Blynk software connection

  // Function for the sensors for once tripped the device will take a photo
  if ( (digitalRead(Motion) == HIGH) || (digitalRead(Sound) == HIGH) )
  {
  Serial.println("Send Notification");     

  // Blynk notification sent to user's phone
  Blynk.notify("Alert: Disturbance Detected.");        
     
  // Blynk notification sent to user's email
  //Blynk.email(myEmail, "CSD Alert", "Disturbance detected. Visit http://"+local_IP+"', for image capture"); 
  
  Serial.println("Capture Photo");     
  Serial.println("");         
  takePhoto();                                             // Calling takePhoto function
  delay(5000);                                             // Delays for 5 seconds
  }

  // Function for interacting with the button in Blynk where the user can make the device take a photo
  if(digitalRead(PHOTO) == HIGH)                
  {
  Serial.println("Manual Capture Photo");
  Serial.println("");
  takePhoto();                                  
  } 
}                                                         // End loop function which constantly repeats
