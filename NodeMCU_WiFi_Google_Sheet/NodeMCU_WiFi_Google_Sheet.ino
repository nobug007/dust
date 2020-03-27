//
//  FILE : NodeMCU_WiFi_Google_Sheet
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 4.1.2020
//  HW : NodeMCU & Google ID
//
#include <string.h>
#include <ctype.h>
#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SoftwareSerial.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <WiFiClientSecure.h>



////  WiFI connection

WiFiClientSecure client;



//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
 // WiFi Setup
  WiFi_Connect();
}

void WiFi_Connect() {
  char ssid[20] = "UXI_5";
  char password[20] = "uxinsight\%";
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
   }
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
   sendData2Server(10, 11);
   delay(1000);
}
//=========================================================================


void sendData2Server(int x, int y)
{
  const char* host = "script.google.com";
  const int httpsPort = 443;
  char DevName[20]= "Nobug_Home";
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
  String url = "/macros/s/AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp/exec?A=1";
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
