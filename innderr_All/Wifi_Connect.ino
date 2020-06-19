void WiFi_Connect() {
  wifi_Flag = 1;
  WiFi.mode(WIFI_STA);

//  WiFi.begin(ssid, password);
    WiFi.begin("UXI_6" , "uxinsight\^");
  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        cnt ++;
        if ( cnt > 100 ) {
          break;
        } 
   }
}


void sendData2Server(int x, int y)
{
  const char* host = "script.google.com";
  const int httpsPort = 443;
//  String GAS_ID = "AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp";  // Replace by your GAS service id

  Serial.print("connecting to ");
  Serial.println(host);
  client.setInsecure();
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    wifi_Flag=1;
    WiFi_Connect();
    return;
  }
    wifi_Flag=0;
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
  String url2 = "/exec?iDust=" + string_x + "&oDust=" + string_y +  "&name="+DevName ;
  Serial.print("requesting URL: ");
  Serial.print(url1);
  Serial.print(URL);
  Serial.println(url2);

  client.print(String("GET ") + url1 + URL + url2 +" HTTP/1.1\r\n" +
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
