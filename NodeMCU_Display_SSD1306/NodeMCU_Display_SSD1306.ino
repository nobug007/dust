//
//  FILE : NodeMCU_SSD1306
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 4.1.2020
//  HW : NodeMCU & SSD1306
//

#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU


//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
  display_init();
}

void display_init() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  drawStatus();  //  BT check & WiFi Check
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
}

//=========================================================================
//
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


/*
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

*/
