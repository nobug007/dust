//=========================================================================
//    WiFi connect
//=========================================================================

void WiFi_Connect() {
  WiFi.mode(WIFI_STA);

  Serial.print(" SSID =  ");  Serial.print(ssid);  Serial.print("     passwd = ");  Serial.println(password);

  WiFi.begin(ssid, password);
  int cnt = 0;
  wifi_Flag = 1;
  while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
        cnt ++;
        if ( cnt > 100 ) {
          wifi_Flag = 0;
          break;
        }
   }
}


void sendData2Server(int x, int y)
{
  const char* host = "script.google.com";
  const int httpsPort = 443;

  client.setInsecure();
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    WiFi_Connect();
  }
  
  String string_x     =  String(x, DEC);
  String string_y     =  String(y, DEC);
//  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_x + "&humidity=" + string_y;
  String url1 = "/macros/s/";
  String url2 = "/exec?iDust=" + string_x + "&oDust=" + string_y +  "&name="+DevName ;

  Serial.print("requesting URL: ");  Serial.print(url1);  Serial.print(URL);  Serial.println(url2);

  client.print(String("GET ") + url1 + URL + url2 +" HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break;
  }
  
  String line = client.readStringUntil('\n');
}
