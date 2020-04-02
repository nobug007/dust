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

int oCal = 60;
int iCal = -115;
// BT Port Set up
SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU

int  wifi_status = 0;

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

char ssid[20] ;
char password[20];
char DevName[20];
int DelayTime;
char URL[100];


//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  

    // BT setup
  Serial.println("BT Start");
  bluetooth.begin(9600);
  init_SD();
 // WiFi Setup
  WiFi_Connect();
  if ( wifi_Flag == 0 ) Serial.println("WiFi Disconnected............");
  else Serial.println("WiFi connected.");


  display_init();
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
    if ( wifi_Flag == 1 ) {
    sendData2Server(i_data,o_data);
   }
   else {
    Serial.println("WiFi Disconnected............"); 
    WiFi_Connect();  
   }
   delay(DelayTime);
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


//=========================================================================
//    SD Card
//=========================================================================


void init_SD() {
  char c;
  char s[200];
  int i;

  File myFile;

  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {    // D8은 SD CS 핀입니다.
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("Config.txt")) {
    Serial.println("config.txt exists.");
  } else {
    Serial.println("config.txt doesn't exist.");
  }

  Serial.println("reading config.txt...");
  myFile = SD.open("Config.txt", FILE_READ);
  if (myFile) {
        Serial.println("Config.txt:");
          // read from the file until there's nothing else in it:
          i = 0;
          while (myFile.available()) {
              c = myFile.read();
              if ( c == '\n' ) { 
                Config_data(s);
                i= 0;
              } else {
                s[i++] = c;
                s[i] = NULL;
              }
          }
  // close the file:
           myFile.close();
      } else {
    // if the file didn't open, print an error:
      Serial.println("error opening config.txt");
      }
}

void Config_data(char s[200]){
  int j=0,k=0;
  char title[50];
  char s_value[100];
  char temp[10];
  char flag = 'T';
  while(s[j] != NULL ) {
     if ( s[j] == '=' ) {
        flag = 'F';
        k=0;
        j++;
     }
     if ( flag == 'T' ){
       title[k] = s[j];
       title[k+1] = NULL;
     } else {
       s_value[k] = s[j];
       s_value[k+1] = NULL;
     } 
     k++;
     j++;
  }
  
//  APN=UXI_6
//  PASS=uxinsight\^
//  Name=Nobug
//  iCal=-115
//  oCal=60
//  Time=30  
//  URL=AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp

  switch(title[0]) {
    case 'A' :
      sprintf(ssid,"%s",s_value);
      Serial.println(ssid);
      break;
    case 'P' :
      sprintf(password,"%s",s_value);
      Serial.println(password);
      break;
    case 'N' :
      sprintf(DevName,"%s",s_value);
      Serial.println(DevName);
      break;
    case 'i' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      iCal = atoi(temp);
      break;
    case 'o' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      oCal = atoi(temp);
      break;
    case 'T' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      DelayTime = atoi(temp) * 1000;  // Sec
      break;
    case 'U' :
      sprintf(URL,"%s",s_value);
      Serial.println(URL);
      break;
  }
  Serial.print("Title = ");
  Serial.print(title);
  Serial.print("       Value = ");
  Serial.println(s_value); 
}

void WiFi_Connect() {
  
  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
    WiFi.begin("UXI_6", "uxinsight\^");
  int cnt = 0;
  wifi_Flag=1;
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        cnt ++;
        if ( cnt > 100 ) {
          wifi_Flag=0;
          break;
        }
   }
}


void sendData2Server(int x, int y)
{
  const char* host = "script.google.com";
  const int httpsPort = 443;
  String GAS_ID = "AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp";  // Replace by your GAS service id

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
  String url1 = "/macros/s/";
  String url2 = "/exec?A=1";
  Serial.print("requesting URL: ");
  Serial.print(url1);
  Serial.print(URL);
  Serial.println(url2);

  client.print(String("GET ") + url1 + URL + url2 + " HTTP/1.1\r\n" +
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
