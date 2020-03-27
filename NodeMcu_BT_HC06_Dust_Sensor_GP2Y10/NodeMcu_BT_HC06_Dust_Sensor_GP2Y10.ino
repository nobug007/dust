//
//  FILE : NodeMcu_BT_HC06_Dust_Sensor_GP2Y10
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 26.3.2020
//  HW : NodeMCU & GP2Y10 & HC06
//

#include <string.h>
#include <ctype.h>
#include <SoftwareSerial.h>

int oCal = 50;
int iCal = -120;

//=======================================================================
// BT Port Set up
//=======================================================================

SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V

//=======================================================================
// gp2y10 dust sensor GPIO
//=======================================================================

int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = D0;         // Connect 3 led driver pins of dust sensor to Arduino D5



//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);

  // BT setup
  Serial.println("BT Start");
  bluetooth.begin(9600);
  
  // Dust Sensor Setup
  pinMode(ledPower,OUTPUT);  // dust LED pin

}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
        BT_Read();
        get_inner_dust();
//        delay(1000);
}

//=======================================================================
//                    Bluetooth Read
//=======================================================================

void BT_Read() {
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
        Serial.print(out_data);
        Serial.print("      ");
        out_i = atoi(out_data)+oCal;
        if ( out_i < 0 ) out_i = 0;
        Serial.println(out_i);
        Flag = 'N';
  }
}

//=======================================================================
//                    Get Dust inform
//=======================================================================

void get_inner_dust() {

  int dust = 0;
  
  for (int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0 + iCal;
  if ( dust < 0 ) dust = 0;
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
