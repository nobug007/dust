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

int i=0;
float dust = 0.0;


SoftwareSerial btSerial(rxBT,txBT);

String btString;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  btSerial.begin(9600); 
}
void loop() {
  get_inner_dust();
  Serial_Print();
  BT_Send();
  delay(2500);
}


void get_inner_dust() {
  dust = 0;
  for (i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
  Serial.print("   -   Dust density :  ");
  Serial.println(dust);
}

float dust_check() {
  digitalWrite(ledPower,LOW );
  delayMicroseconds(samplingTime);

  voMeasured = analogRead( measurePin );

  delayMicroseconds( deltaTime );
  digitalWrite( ledPower , HIGH );
  delayMicroseconds(sleepTime );

  calcVoltage = voMeasured * ( 5.0 / 1024.0 );

  dustDensity = (0.17 * calcVoltage /* -1 */ - 0.07 )* 1000 ;
  delay(500);

  return dustDensity;
}


void Serial_Print() {
  
  Serial.print("Raw signal Valure :  " );
  Serial.print(voMeasured);

  Serial.print("   -   votage :  ");
  Serial.print(calcVoltage);
  
  Serial.print("   -   Dust density :  ");
  Serial.println(dust);

}

void BT_Send() {
      btString = dust;
      Serial.println(btString);

      for( i=0;i<btString.length();i++) {
         btSerial.write(btString[i]);
         delay(5);  
      }
     btSerial.write('\n');
      
}
