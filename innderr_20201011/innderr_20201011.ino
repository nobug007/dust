//
//  FILE : NodeMCU_BT_HC06_SSD1306_Dust_Sensor_GP2Y10_SD_CARD_WiFi_Google_Sheet
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 01.04.2020
//  HW : NodeMCU & SSD1306 & HC06 & GP2Y10 & SD_Card
//
#include <string.h>
#include <ctype.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <WiFiClientSecure.h>
#include <SPI.h>   // SD Card  Header 
#include <SD.h>    
#define  graphMax 100


// gp2y10 dust sensor GPIO
int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5

int oCal = 65;
int iCal = 5;
// BT Port Set up
SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU

//int  wifi_status = 0;

int iDust_Arr[graphMax];
int oDust_Arr[graphMax];
int iArr = 0;


//   SD Card  Pin 
//
//   VCC  -> UV  ( 5V )
//   CS   -> D8 ( Changable )
//   SCK  -> D5
//   MOSI -> D7
//   MISO -> D6

////  WiFI connection

WiFiClientSecure client;
int wifi_Flag = 0;

char ssid[20] = "nobug_home";
char password[20] = "bang8813";
char DevName[20]= "nobug";
int DelayTime = 30000;
char URL[100]= "AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp";


int bluetooth_flag = 0;

//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  

    // BT setup
  Serial.println("BT Start");
  bluetooth.begin(9600);
  display_init();
  
//  WiFi Setup
  WiFi_Connect();
  if ( wifi_Flag == 0 ) Serial.println("WiFi Disconnected............");
  else Serial.println("WiFi connected.");
}
