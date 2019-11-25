#include <SoftwareSerial.h>
SoftwareSerial bluetooth(6,7);

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("BT Write !");

  bluetooth.print("AT");
  delay(1000);
  while(bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  Serial.println(" !");


  bluetooth.print("AT+ROLE=M");
  delay(1000);
  while(bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  Serial.println(" !");

  bluetooth.print("AT+NAMEDust_M");
  delay(1000);
  while(bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  Serial.println(" !");
  bluetooth.print("AT+PIN123456");
  delay(1000);

  while(bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  Serial.println(" !");

  Serial.println("OK All Done !");

}
void loop()
{
  Serial.println("Bluetootn state...");
  
  bluetooth.write("AT+STATE?");

  while(bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  Serial.println(" !");
  delay(1000); 
  
}
