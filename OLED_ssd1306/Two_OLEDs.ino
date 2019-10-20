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
   iTemp = random(20);
   oTemp = random(20);
   
   display.setTextAlignment(TEXT_ALIGN_RIGHT);

   sprintf(cTemp,"%d",iTemp);
   display.drawString(124,25, cTemp);
   sprintf(cTemp,"%d",oTemp);
   display.drawString(124,48, cTemp);

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
