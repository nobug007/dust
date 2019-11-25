 
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


 // BT Port Set up

 SoftwareSerial bluetooth(D6, D7);

 
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D2, D3);  //D2=SDA  D3=SCL  As per labeling on NodeMCU


// gp2y10 dust sensor parameter

int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D1;         // Connect 3 led driver pins of dust sensor to Arduino D8

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
char cTemp[4];
char Flag = 'N';
 
void setup() {
  delay(1000);
  Serial.begin(9600);  
  Serial.println("");
  bluetooth.begin(9600); 
  pinMode(ledPower,OUTPUT);  // dust LED pin 
  
  Serial.println("Initializing OLED Display");
  display.init();
 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  drawStatus();  //  need BT check & WiFi Check 
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {

//   drawStatus();  //  BT check & WiFi Check 
   drawGraph();   // Draw Graph
//   sendData2Server();
   delay(1000);
//     Out_BT_Read();
}
//=========================================================================

void Out_BT_Read() {
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
        Serial.print("BT Read  :  ");
        oTemp = atof(out_data);
        Serial.println(oTemp);
        get_inner_dust();
        drawStatus();
        drawGraph();
        Flag = 'N';
  } 
  delay(100);
}

void get_inner_dust() {
  dust = 0;
  for(int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
  Serial.print("   -   Dust density :  ");
  Serial.println(iTemp);
}


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
   Serial.print("dust=");
   Serial.println( dust);
   Serial.println(cTemp);
   sprintf(cTemp,"%d",oTemp);
   display.drawString(124,48, cTemp);

  if ( dust > 100 ) dust = 100 ;
  else if ( dust < 0 ) dust = 0;
   iTemp = map(dust,0,100,0,20);  // dust => graph high

//debug
   Serial.println(iTemp);

   
   if ( i == graphMax) {
     for(j=0;j<graphMax;j++) {
       iDust[j] = iDust[j+1];
       oDust[j] = oDust[j+1];
     }
   }
   
   iDust[i] = iTemp;
   oDust[i] = oTemp;
   
   for(j=0;j<=i;j++) {
      display.drawLine(j,40,j,40-iDust[j] );
      display.drawLine(j,64,j,64-oDust[j] );
   }
  display.display();
  if (i < graphMax ) i++;
}


void sendData2Server()
{
  
}


float dust_check() {
  digitalWrite(ledPower,LOW );
  delayMicroseconds(samplingTime);

  voMeasured = analogRead( measurePin );

  delayMicroseconds( deltaTime );
  digitalWrite( ledPower , HIGH );
  delayMicroseconds(sleepTime );

  calcVoltage = voMeasured * ( 5.0 / 1024.0 );

  dustDensity = (0.17 * calcVoltage)* 1000 ; // Cal 0.1
  delay(500);
     Serial.print("dustDensity= ");

     Serial.println( dustDensity);

  return dustDensity;
}
