//
//  FILE : Inner Dust Sensor
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 10.9.2019
//  HW : NodeMCU & SSD1306
//

#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
 
 
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D2, D3);  //D2=SDK  D3=SCK  As per labeling on NodeMCU


// gp2y10 dust sensor parameter

int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D8;         // Connect 3 led driver pins of dust sensor to Arduino D8

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
 
void setup() {
  delay(1000);
  Serial.begin(115200);  
  Serial.println("");

  pinMode(ledPower,OUTPUT);  // dust LED pin 
  
  Serial.println("Initializing OLED Display");
  display.init();
 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  
   drawStatus();  //  BT check & WiFi Check 
   drawGraph();   // Draw Graph
   sendData2Server();
   delay(1000);
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

   get_inner_dust();
   oTemp = random(20);
  
   
   display.setTextAlignment(TEXT_ALIGN_RIGHT);

   sprintf(cTemp,"%d",(int)dust);
   display.drawString(124,25, cTemp);
   Serial.println(dust);
   Serial.println(cTemp);
   sprintf(cTemp,"%d",oTemp);
   display.drawString(124,48, cTemp);

  if ( dust > 250 ) dust = 250 ;
  else if ( dust < 0 ) dust = 0;
   iTemp = map(dust,0,250,0,20);  // dust => graph high

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

void get_inner_dust() {
  dust = 0;
  for(int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
  Serial.print("   -   Dust density :  ");
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

  dustDensity = (0.17 * calcVoltage )* 1000 ;
  delay(500);

  return dustDensity;
}
