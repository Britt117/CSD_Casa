////////////////////////////////////////////////////////////////////////////////////////////
/* 
 *                         Casa Security Device Arduino Code
 *                                   Developed by:
 *                                  -Britton E Dean
 *                                  -Alexander Mitchell
 *                                  -Odera Unigwe
 *               Prototype for Senior Design Exhibit II at Mercer University.
*/
////////////////////////////////////////////////////////////////////////////////////////////

#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>  

// Global varables
const char* ssid = "**********";           // Your Wifi name, ex: NETGEAR57
const char* password = "***********";      // Password to your Wifi, ex: fluffytrumpet252
char auth[] = "************";              // Authentication code sent by Blynk, check email

// Defining ESP32 model and its pins
#define CAMERA_MODEL_AI_THINKER            // Your ESP32-Cam's model of which has PSRAM
#include "camera_pins.h"                   // This library has to be called after the ESP32's model has been defined

// Defining I/O pins
#define Motion 12                          // HC-SR501 PIR Motion Sensor controlled with pin 12
#define Sound 13                           // KY-038 AVR PIC high sensitivity sound sensor controlled with pin 13
#define Lamp 4                             // ESP32's on board flash lamp controlled with pin 4
#define PHOTO 14                           // OV2640 Camera controlled with pin 14

String local_IP;                           // Your IP string varible
void startCameraServer();                  // The camera server where the photos will be taken



void takePhoto()                           // Functionn for taking photo
{
  digitalWrite(Lamp, HIGH);                
  delay(200);
  uint32_t randomNum = random(50000);
  Serial.println("http://"+local_IP+"/capture?_cb="+ (String)randomNum); 
  Blynk.setProperty(V1, "urls", "http://"+local_IP+"/capture?_cb="+(String)randomNum);
  digitalWrite(Lamp, LOW);                 
  delay(1000);
}                                          // end takePhoto function



void setup()                               // setup function
{
  Serial.begin(115200);                    // Serial monitor set to 115200 Baud
  
  pinMode(Lamp,OUTPUT);                    // Setting flash lamp as an output
  pinMode(Motion,INPUT);                   // Setting motion sensor as an input
  pinMode(Sound,INPUT);                    // Setting sound sensor as an input
  
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

  sensor_t * s = esp_camera_sensor_get();       // Get pointer to a frame buffer and return the pointer to the frame buffer
  
  // Initial sensors are flipped vertically and colors are a little saturated as well as dim thus we correct it via the following lines
  if (s->id.PID == OV2640_PID)
  {
    s->set_vflip(s, 1);                         // Fliping the image 
    s->set_brightness(s, 1);                    // Raising the brightness 
    s->set_saturation(s, -2);                   // Lowering the saturation
  }
  
  s->set_framesize(s, FRAMESIZE_QVGA);          // Drop down frame size for higher initial frame rate

  WiFi.begin(ssid, password);                   // Getting WiFi your info

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");                          
  }
  Serial.println("");
  Serial.println("WiFi connected");             // WiFi succefully connected

  startCameraServer();                          // Calling startCameraServer function 

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());                 // Getting your IP
  local_IP = WiFi.localIP().toString();         // Assigning your IP to the local_IP variable
  Serial.println("' to connect");
  
  Blynk.begin(auth, ssid, password);            // Getting Blynk connected

  Serial.println("Setup Complete");
}                                               // end setup function



void loop()                                     // loop function
{
  // Main code which runs continuously:
  
  Blynk.run();                                  // Running Blynk software connection

  // Function for the sensors for once tripped the device will take a photo
  if ( (digitalRead(Motion) == HIGH) || (digitalRead(Sound) == HIGH) )
  {
  Serial.println("Send Notification");          
  Blynk.notify("Alert: Disturbance Detected."); // Blynk notification
  Serial.println("Capture Photo");              
  takePhoto();                                  // Calling takePhoto function
  delay(5000);                                  // Delays for 5 seconds
  }

  // Function for interacting with the button in Blynk where the user can make the device take a photo
  if(digitalRead(PHOTO) == HIGH)                
  {
  Serial.println("Capture Photo");
  takePhoto();                                  
  } 
}                                               // end loop function which constantly repeats
