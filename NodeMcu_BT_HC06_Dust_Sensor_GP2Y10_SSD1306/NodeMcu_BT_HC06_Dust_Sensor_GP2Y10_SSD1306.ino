//
//  FILE : NodeMcu_BT_HC06_Dust_Sensor_GP2Y10_SSD1306
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 26.3.2020
//  HW : NodeMCU & GP2Y10 & HC06 & SSD1306
//

#include <string.h>
#include <ctype.h>
#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

int oCal = 50;
int iCal = -120;

//=======================================================================
// gp2y10 dust sensor GPIO
//=======================================================================

int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5

int iTemp;
int oTemp;
char cTemp[10];
  int dust = 0;

//=======================================================================
// BT Port Set up
//=======================================================================

SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V

//=======================================================================
// Initialize the OLED display using Wire library
//=======================================================================

SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU


#define graphMax 100

//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);

 // Display Init
  display_init();
  
  // BT setup
  Serial.println("BT Start");
  bluetooth.begin(9600);
  
  // Dust Sensor Setup
  pinMode(ledPower,OUTPUT);  // dust LED pin

}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
        BT_Read();
        get_inner_dust();
        drawGraph();
//        delay(1000);
}


//=======================================================================
//                    Display
//=======================================================================

void display_init() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  drawStatus();  //  BT check & WiFi Check
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

   int i = 0, j = 0;
   int iDust[graphMax];
   int oDust[graphMax];
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


//=======================================================================
//                    Bluetooth Read
//=======================================================================

void BT_Read() {
  char out_data[10];
  int out_i=0;
  char Flag = 'N';

  while (bluetooth.available()) {
      Serial.print(".");
    // 수신 받은 데이터 저장
    out_data[out_i] = (char)bluetooth.read();
    out_data[++out_i] = NULL;
    Flag = 'Y';
    delay(100);
  }
  
  if ( Flag == 'Y' ) {
       // 수신된 데이터 시리얼 모니터로 출력
        Serial.print("BT Read Out Data :  ");
        Serial.print(out_data);
        Serial.print("      ");
        oTemp = atoi(out_data)+oCal;
        if ( oTemp < 0 ) oTemp = 0;
        Serial.println(oTemp);
        Flag = 'N';
  }
}

//=======================================================================
//                    Get Dust inform
//=======================================================================

void get_inner_dust() {


  
  for (int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0 + iCal;
  if ( dust < 0 ) dust = 0;
  Serial.print("   -  5 times inner Dust density :  ");
  Serial.println(dust);
}


float dust_check() {

  int samplingTime = 280;
  int deltaTime = 40;
  int sleepTime = 9680;

  float voMeasured = 0.0;
  float calcVoltage = 0.0;
  float dustDensity = 0.0;

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
