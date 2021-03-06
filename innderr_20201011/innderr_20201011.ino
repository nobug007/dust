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

int iCal = 0;
int oCal = 65;
char ssid[20] = "shin";
char password[20] = "!shin123";
char DevName[20]= "Shin";
int DelayTime = 20000;
char URL[100]= "AKfycbwsoCrKOusQSGTYzd9k8YdLg34fZ-wGVCwa0rHd1CXGe6vRW0O5";

/* 방효식   AKfycbzW348NaBBoLuQ1thJqiROx-Yzwuxc4DujicWcieHvbav67tpc
 * 우문영   AKfycby_HUOgx8mllQOmr_hTdM6OVoaVwjIFaZ78t63msrKMkKTTGpo
 * 박지수   AKfycbyoSPgZDf11k48PasuJuL6U53sPlOn1FhAOPx3CjFOrloSc7tQx    65 /  5   -   파란색 9
 * 신제용   AKfycbwsoCrKOusQSGTYzd9k8YdLg34fZ-wGVCwa0rHd1CXGe6vRW0O5     0  /  65  -
 * 조해수   AKfycbxwR6Gqkt1vN50iJlSyI5KM5SwikVma9qIAr1YrhoGw7Yehnao5
 * 류정민   AKfycbymvNaZ-PxsMzwL-t7iZ9JhUlIwkbehCNxjmTVdA0_oiNCi3Nk        
 */

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

  init_SD();
  display_init();

  
//  WiFi Setup
  WiFi_Connect();
  if ( wifi_Flag == 0 ) Serial.println("WiFi Disconnected............");
  else Serial.println("WiFi connected.");
}
