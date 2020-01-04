//
//  FILE : NodeMcu_Dust_Sensor_GP2Y10
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 4.1.2020
//  HW : NodeMCU & GP2Y10
//
#include <SoftwareSerial.h>


// gp2y10 dust sensor GPIO

int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5



//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);

  // Dust Sensor Setup
  pinMode(ledPower,OUTPUT);  // dust LED pin

}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
        get_inner_dust();
        delay(1000);
}

//=======================================================================
//                    Get Dust inform
//=======================================================================

void get_inner_dust() {

  int dust = 0;
  
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

  dustDensity = (0.17 * calcVoltage + 0.0005 )* 1000 ; // Cal 0.1
  delay(500);

  return dustDensity;
}
