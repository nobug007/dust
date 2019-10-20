#include <SoftwareSerial.h>

SoftwareSerial bluetooth(D6,D7);
>>>>>>> 1bb5798a1476679d330e145cb8447e2990375f20
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

  bluetooth.print("AT+NAMEDust");
  delay(1000);
  while(bluetooth.available()) {
    char c = bluetooth.read();
    Serial.print(c);
  }
  Serial.println(" !");
  bluetooth.print("AT+PIN1234");
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
}
