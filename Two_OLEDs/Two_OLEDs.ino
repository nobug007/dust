#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
 
 
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D2, D3);  //D2=SDK  D1=SCK  As per labeling on NodeMCU
 
//=======================================================================
//                    Power on setup
//=======================================================================

 int i=0;
 
void setup() {
  delay(1000);
  Serial.begin(115200);  
  Serial.println("");
  
  Serial.println("Initializing OLED Display");
  display.init();
 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  drawFontFaceDemo();
}
 
//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
//  if ( i== 17 ) i =0;
   drawFontFaceDemo();
//  delay(1000);
  drawRectDemo();
//  delay(1000);
//  void drawCircleDemo();
  delay(1000);
}
//=========================================================================
 
 
void drawFontFaceDemo() {
  // clear the display
  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);

  display.drawRect(0, 0, display.getStringWidth("BT ON")+6, 16);
    display.drawString(3, 2, "BT ON");

    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);
  display.drawRect(122-display.getStringWidth("WiFi ON"),0,display.getStringWidth("WiFi ON")+6, 16);
    display.drawString(125,2, "WiFi ON");

    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64,2, "DUST");


     display.drawRect(0, 17, 128, 47);
     display.drawLine(0, 40, 128, 40);     //  47/2 = 23
     display.drawLine(100, 18, 100, 63);     //  47/2 = 23

  display.display();
}
 
void drawRectDemo() {
  // clear the display
//  display.clear();
   display.setTextAlignment(TEXT_ALIGN_RIGHT);
   display.setFont(ArialMT_Plain_10);
   display.drawString(124,25, "888");
   display.drawString(124,48, "000");

  for(i=0;i<100;i++) {
      display.drawLine(i,40,i,35+sin(i)*3 );
  }
   for(i=0;i<100;i++) {
      display.drawLine(i,67,i,62+cos(i)*3 );
  }
//  display.drawHorizontalLine(0,i++,128);
      // Draw a pixel at given position
//    for (int i = 0; i < 64; i++) {
//      display.setPixel(i, i);
//      display.setPixel(64 - i, i);
//    }
//display.drawCircle(32, 32, 10);
    /*
    display.drawRect(12, 12, 20, 20);
 
    // Fill the rectangle
    display.fillRect(14, 14, 17, 17);
 
    // Draw a line horizontally
    display.drawHorizontalLine(0, 40, 20);
 
    // Draw a line horizontally
    display.drawVerticalLine(40, 0, 20);
   // write the buffer to the display
   */
  display.display();
}
 
void drawCircleDemo() {
  // clear the display
  display.clear();

    display.fillRect(14, 14, 17, 17);
//  display.drawRect(12, 12, 20, 20);
//  display.setColor(WHITE);
//  display.drawCircle(32, 32, 10);
//    display.setColor(WHITE);
//  for (int i=1; i < 8; i++) {
//    display.drawCircle(32, 32, i*3);
//    if (i % 2 == 0) {
//      display.setColor(BLACK);
//    }
//    display.fillCircle(96, 32, 32 - i* 3);
 // }
 
  // write the buffer to the display
  display.display();
}
