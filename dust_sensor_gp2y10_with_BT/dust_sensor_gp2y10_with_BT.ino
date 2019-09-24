#include <SoftwareSerial.h>

#define rxBT 6
#define txBT 7

///////////////////////    dust sensor   /////////////////
int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = 2;         // Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;
////////////////////////////////////////////////////////

SoftwareSerial btSerial(rxBT,txBT);

String btString;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  btSerial.begin(9600); 
}
void loop() {
  get_dust_density();
  Serial_Print();
  BT_Send();
  delay(600000);
}

void get_dust_density() {
  digitalWrite(ledPower,LOW );
  delayMicroseconds(samplingTime);

  voMeasured = analogRead( measurePin );

  delayMicroseconds( deltaTime );
  digitalWrite( ledPower , HIGH );
  delayMicroseconds(sleepTime );

  calcVoltage = voMeasured * ( 5.0 / 1024.0 );

  dustDensity = 0.17 * calcVoltage  * 1000 ;//  -0.1;  // 1000 is   mg => ug
}

void Serial_Print() {
  
  Serial.print("Raw signal Valure :  " );
  Serial.print(voMeasured);

  Serial.print("   -   votage :  ");
  Serial.print(calcVoltage);
  
  Serial.print("   -   Dust density :  ");
  Serial.println(dustDensity);

}

void BT_Send() {
      btString = dustDensity;
      Serial.println(btString);

      for(int i=0;i<btString.length();i++) {
         btSerial.write(btString[i]);
         delay(5);  
      }
     btSerial.write('\n');
      
}
