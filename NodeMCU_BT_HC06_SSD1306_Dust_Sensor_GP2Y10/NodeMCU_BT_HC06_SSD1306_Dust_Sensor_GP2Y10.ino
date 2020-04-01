//
//  FILE : NodeMCU_BT_HC06_SSD1306_Dust_Sensor_GP2Y10
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 27.3.2020
//  HW : NodeMCU & SSD1306 & HC06 & GP2Y10
//
#include <string.h>
#include <ctype.h>
#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define  graphMax 100



// gp2y10 dust sensor GPIO
int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5

int oCal = 50;
int iCal = -105;
// BT Port Set up
SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V

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
  pinMode(ledPower,OUTPUT);
  
  display_init();
    // BT setup
  Serial.println("BT Start");
  bluetooth.begin(9600);
}


//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
    int o_data, i_data;
    o_data = BT_Read()+oCal;
    i_data =  get_inner_dust()+iCal;
    drawGraph(i_data,o_data);
    Serial.print("Out Data : ");
    Serial.println(o_data);
       Serial.print("In Data : ");
    Serial.println(i_data);
    delay(10000);
}

//=======================================================================
//                    Bluetooth Read
//=======================================================================

int BT_Read() {
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
        Serial.println(out_data);
        Flag = 'N';
  }
  return atoi(out_data);
}

//=======================================================================
//                    Get Dust inform
//=======================================================================

int get_inner_dust() {

  int dust = 0;
  
  for (int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
  Serial.print("   -  5 times inner Dust density :  ");
  Serial.println(dust);
  return (int)dust;
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
      display.drawLine(j+1,63,j+1,42); 
      display.setColor(WHITE);
      display.drawLine(j+1,40,j+1,40-iDust_Arr[j] );
      display.drawLine(j+1,63,j+1,63-oDust_Arr[j] );
   }

  display.display();
  if (iArr < graphMax-1 ) iArr++;

  Serial.println( iArr);
  delay(100);
}
