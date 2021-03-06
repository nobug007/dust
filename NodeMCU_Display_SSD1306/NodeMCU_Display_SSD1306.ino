//
//  FILE : NodeMCU_SSD1306
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 26.3.2020
//  HW : NodeMCU & SSD1306
//

#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define  graphMax 100

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU

int  wifi_status = 0;

int iDust_Arr[graphMax];
int oDust_Arr[graphMax];
int iArr = 0;
//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
  display_init();
}


//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  for(int i = 0;i<100 ; i++ ) {
    drawGraph(i,100-i);
  }
}
//=======================================================================
//                    Display SSD1306 init
//=======================================================================

void display_init() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  drawStatus();  //  BT check & WiFi Check
}

//=========================================================================
//
//=========================================================================

void drawStatus() {
  char wifi_String[15];
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
  if ( wifi_status == 1 ) {
    sprintf(wifi_String,"%S","WiFi ON");
  } else {
    sprintf(wifi_String,"%S","WiFi OFF");    
  }
  display.drawRect(122-display.getStringWidth(wifi_String),0,display.getStringWidth(wifi_String)+6, 16);
  display.drawString(125,2, wifi_String);

// Graph rectangle
   display.drawRect(0, 17, 128, 47);
   display.drawLine(0, 40, 128, 40);     //  47/2 = 23
   display.drawLine(100, 18, 100, 63);     //  47/2 = 23

  display.display();
}



void drawGraph(int iDust, int oDust) {

   int j=0;
   char cTemp[5];
   int iTemp, oTemp;

   display.setTextAlignment(TEXT_ALIGN_RIGHT);

   display.setColor(BLACK);
   display.fillRect(102, 20, 24, 20);
   display.fillRect(102, 42, 24, 20);
   
   display.setColor(WHITE);
   sprintf(cTemp,"%d",iDust);
   display.drawString(124,25, cTemp);
   sprintf(cTemp,"%d",oDust);
   display.drawString(124,48, cTemp);

   if ( iDust > 100 ) iDust = 100 ;
   else if ( iDust < 0 ) iDust = 0;
   if ( oDust > 100 ) oDust = 100 ;
   else if ( oDust < 0 ) oDust = 0;


   if ( iArr == graphMax-1) {
     for(j=0;j<graphMax-1;j++) {
       iDust_Arr[j] = iDust_Arr[j+1];
       oDust_Arr[j] = oDust_Arr[j+1];
     }
   }

   iDust_Arr[iArr] = map(iDust,0,100,0,20);  // dust => graph high
   oDust_Arr[iArr] = map(oDust,0,100,0,20);  // dust => graph high

 
   for(j=0;j<iArr;j++) {
      display.setColor(BLACK);
      display.drawLine(j+1,40,j+1,20); 
      display.drawLine(j+1,64,j+1,44); 
      display.setColor(WHITE);
      display.drawLine(j+1,40,j+1,40-iDust_Arr[j] );
      display.drawLine(j+1,64,j+1,64-oDust_Arr[j] );
   }

  display.display();
  if (iArr < graphMax-1 ) iArr++;

  Serial.println( iArr);
  delay(100);
}
