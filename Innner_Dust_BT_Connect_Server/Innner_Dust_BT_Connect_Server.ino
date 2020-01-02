//
//  FILE : Inner Dust Sensor
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 10.9.2019
//  HW : NodeMCU & SSD1306
//
#include <string.h>
#include <ctype.h>
#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <WiFiClientSecure.h>

////  WiFI connection 
String readString;
const char* ssid = "Usability_5";
const char* password = "usability2019%";
const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client;

String GAS_ID = "AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp";  // Replace by your GAS service id


// BT Port Set up


 SoftwareSerial bluetooth(D1, D2);

 
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU


// gp2y10 dust sensor parameter

int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;

float dust;

//=======================================================================
//                    Power on setup
//=======================================================================

int i=0,j;
int iDust[128],iTemp;
int oDust[128],oTemp;
int graphMax = 100;
char cTemp[5];
char Flag = 'N';
 
void setup() {
  delay(1000);
  Serial.begin(9600);  

  // BT setup 
  bluetooth.begin(9600); 

 // WiFi Setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
             
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
   }

  // Dust Sensor Setup 
  pinMode(ledPower,OUTPUT);  // dust LED pin 
  
  Serial.println("BT ON");
  Serial.println("Initializing OLED Display");
  display.init();
 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  drawStatus();  //  BT check & WiFi Check 
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  char out_data[10];
  int out_i=0;

  while (bluetooth.available()) { 
    // 수신 받은 데이터 저장
    out_data[out_i] = (char)bluetooth.read();
    out_data[++out_i] = NULL; 
    // 수신된 데이터 시리얼 모니터로 출력
    Flag = 'Y';
  }
  if ( Flag == 'Y' ) {
        Serial.print("BT Read Out Data :  ");
        Serial.println(out_data);
        oTemp = atof(out_data);
        get_inner_dust();
        drawStatus();
        drawGraph();
        Flag = 'N';
  } 
}
//=========================================================================


void drawStatus() {
  // clear the display
   display.clear();
   // Title Display

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64,2, "DUST");

//  if BT ON
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawRect(0, 0, display.getStringWidth("BT ON")+6, 16);
  display.drawString(3, 2, "BT ON");

//  if WiFi ON
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawRect(122-display.getStringWidth("WiFi ON"),0,display.getStringWidth("WiFi ON")+6, 16);
  display.drawString(125,2, "WiFi ON");

// Graph rectangle 
   display.drawRect(0, 17, 128, 47);
   display.drawLine(0, 40, 128, 40);     //  47/2 = 23
   display.drawLine(100, 18, 100, 63);     //  47/2 = 23

  display.display();
}
 
void drawGraph() {
   
   display.setTextAlignment(TEXT_ALIGN_RIGHT);

   sprintf(cTemp,"%d",(int)dust);
   display.drawString(124,25, cTemp);
   sprintf(cTemp,"%d",(int)oTemp);
   display.drawString(124,48, cTemp);

   if ( dust > 100 ) dust = 100 ;
   else if ( dust < 0 ) dust = 0;
   if ( oTemp > 100 ) oTemp = 100 ;
   else if ( oTemp < 0 ) oTemp = 0;
   iTemp = map(dust,0,100,0,20);  // dust => graph high
   oTemp = map(oTemp,0,100,0,20);  // dust => graph high

   
   if ( i == graphMax) {
     for(j=0;j<graphMax;j++) {
       iDust[j] = iDust[j+1];
       oDust[j] = oDust[j+1];
     }
   }
   
   iDust[i] = iTemp;
   oDust[i] = oTemp;
   
   for(j=1;j<=i;j++) {
      display.drawLine(j,40,j,40-iDust[j] );
      display.drawLine(j,64,j,64-oDust[j] );
   }
  display.display();
  if (i < graphMax ) i++;
  delay(100);
}

void get_inner_dust() {
  dust = 0;
  for (int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
  Serial.print("   -  5 times inner Dust density :  ");
  Serial.println(dust);
}


float dust_check() {
  digitalWrite(ledPower,LOW );
  delayMicroseconds(samplingTime);

  voMeasured = analogRead( measurePin );

  delayMicroseconds( deltaTime );
  digitalWrite( ledPower , HIGH );
  delayMicroseconds(sleepTime );

  calcVoltage = voMeasured * ( 5.0 / 1024.0 );

  dustDensity = (0.17 * calcVoltage + 0.0005 )* 1000 ; // Cal 0.1
  delay(500);

  return dustDensity;
}


void sendData2Server(int x, int y)
{
    Serial.print("connecting to ");
  Serial.println(host);
  client.setInsecure();
  if (!client.connect(host, httpsPort)) {

    Serial.println("connection failed");
    return;
  }
/*
  if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } else {
  Serial.println("certificate doesn't match");
  }
  */
  String string_x     =  String(x, DEC);
  String string_y     =  String(y, DEC);
//  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_x + "&humidity=" + string_y;
  String url = "/macros/s/AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp/exec?A=1";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
     Serial.println("=====111=====");
     Serial.println(line);
    Serial.println("====111======");
    Serial.println("headers received");
    break;
  }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}
