//
//  FILE : Outter Dust Sensor
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 10.16.2019
//  HW : Arduino Nano & GP2Y10 & HC-06
//

#include <SoftwareSerial.h>


////////////    HC-06    //////////////////////

#define rxBT 6
#define txBT 7

int i=0;
float dust = 0.0;


SoftwareSerial btSerial(rxBT,txBT);

String btString;

///////////////////////    dust sensor   GP2Y10   /////////////////
int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = 13;         // Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;

////////////////////////////////////////////////////////



void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  btSerial.begin(9600); 
}

void loop() {
  get_out_dust();
  Serial_Print();
  BT_Send();
  delay(2500);  // send cycle 
}


void get_out_dust() {
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

  dustDensity = (0.17 * calcVoltage /* -1 */ -0.06 )* 1000 ; // cal == 0.1
  delay(500);
 // if ( dustDensity < 0 ) dustDensity = 0;
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
