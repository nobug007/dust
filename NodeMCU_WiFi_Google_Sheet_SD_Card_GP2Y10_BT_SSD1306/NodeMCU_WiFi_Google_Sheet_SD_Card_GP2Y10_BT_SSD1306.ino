//
//  FILE : NodeMCU_WiFi_Google_Sheet_SD_Card_GP2Y10_BT_SSD1306
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 5.1.2020
//  HW : NodeMCU & Google ID  &  SD Card & GP2Y10 & HC06 & SSD1306
//
#include <string.h>
#include <ctype.h>
#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <WiFiClientSecure.h>
#include <SPI.h>   // SD Card  Header 
#include <SD.h>    

//   SD Card  Pin 
//
//   VCC  -> UV  ( 5V )
//   CS   -> D8 ( Changable )
//   SCK  -> D5
//   MOSI -> D7
//   MISO -> D6

// BT Port Set up
SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D3, D4);  //D3=SDA  D4=SCL  As per labeling on NodeMCU

////  WiFI connection

WiFiClientSecure client;

char ssid[20] ;
char password[20];
char DevName[20];
int iCal;
int oCal;

// gp2y10 dust sensor GPIO
int i=0;
int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5
int dust = 0;
int out_dust = 0; 
int iDust[128];
int oDust[128];
//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
  init_SD();
 // Dust Sensor Setup
  pinMode(ledPower,OUTPUT);  // dust LED pin
  // BT setup
  bluetooth.begin(9600);
 // WiFi Setup
  WiFi_Connect();
  display_init();
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
   get_inner_dust();
   out_dust =   BT_Read();
   sendData2Server(dust, out_dust);
   delay(1000);
}
//=========================================================================



/*   컨피그 화일이 없으면 이것을 풀어 먼저 컨피그 화일을 만들어 넣고, 시작 하세요. 
void init_SD() {
  char c;
  char s[100];
  int i;

  File myFile;

  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {    // D8은 SD CS 핀입니다.
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("Config.txt")) {
    Serial.println("Config.txt exists.");
  } else {
     myFile = SD.open("Config.txt", FILE_WRITE);

    Serial.println("Config.txt doesn't exist. It made.");
  }
    myFile.write("APN=Nobug_Home\n");
    myFile.write("PASS=12345678\n");
    myFile.write("Name=Nobug\n");
    myFile.write("iCal=0.001\n");
    myFile.write("oCal=0.002\n");
           myFile.close();
           Serial.println("finished Config.txt");
}

*/

void init_SD() {
  char c;
  char s[100];
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

void Config_data(char s[100]){
  int j=0,k=0;
  char title[50];
  char s_value[50];
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
  
//  APN=Nobug_Home     //char ssid[20] ;
//  PASS=12345678      //char password[20];
//  Name=Nobug         //char DevName[20];
//  iCal=0.001        //float iCal;
//  oCal=0.002        //float oCal;

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
      iCal = atof(temp);
      break;
    case 'o' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      oCal = atof(temp);
      break;
  }
  Serial.print("Title = ");
  Serial.print(title);
  Serial.print("       Value = ");
  Serial.println(s_value); 
}

void WiFi_Connect() {
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
   }
}


void sendData2Server(int x, int y)
{
  const char* host = "script.google.com";
  const int httpsPort = 443;
  String GAS_ID = "AKfycby1aogYleMRxnU3gLmuB9DYAU6ykKC55VfcGL-fKY85UD8Mh2c";  // Replace by your GAS service id

  Serial.print("connecting to ");
  Serial.println(host);
  client.setInsecure();
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  String string_x     =  String(x, DEC);
  String string_y     =  String(y, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?name="+ DevName + "&iDust=" + string_x + "&oDust=" + string_y;
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


//=======================================================================
//                    Get Dust inform
//=======================================================================

void get_inner_dust() {

  
  for (int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
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

  dustDensity = (0.17 * calcVoltage - 0.001 )* 1000 ; // Cal 0.1
  delay(500);

  return dustDensity;
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
 return (int)atof(out_data);
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


   char cTemp[10];
   int iTemp, oTemp;
   int graphMax = 100;
   int j;

   display.setTextAlignment(TEXT_ALIGN_RIGHT);

   sprintf(cTemp,"%d",dust);
   display.drawString(124,25, cTemp);
   sprintf(cTemp,"%d",out_dust);
   display.drawString(124,48, cTemp);

   if ( dust > 100 ) dust = 100 ;
   else if ( dust < 0 ) dust = 0;
   if ( out_dust > 100 ) out_dust = 100 ;
   else if ( out_dust < 0 ) out_dust = 0;
   iTemp = map(dust,0,100,0,20);  // dust => graph high
   oTemp = map(out_dust,0,100,0,20);  // dust => graph high


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
