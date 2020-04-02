//
//  FILE : NodeMCU_BT_HC06
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 5.1.2020
//  HW : NodeMCU & HC06
//
#include <string.h>
#include <ctype.h>
#include <SoftwareSerial.h>

// BT Port Set up
SoftwareSerial bluetooth(D1, D2);   //  TX =  D1, RX =  D2    VCC = UV ( test ) 5V
int oCal = 0;
int out_dust;
int bt_flag;

//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
  // BT setup
  Serial.println("BT Start");
  bluetooth.begin(9600);
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  BT_Read();
  if ( oCal == out_dust ) bt_flag = 1;   // disconnected
  else bt_flag = 0;                      // connected

  check_BT_State();
}

void check_BT_State() {
  if ( bt_flag == 1 ) Serial.println("BT Connect Fail.......");
  else Serial.println("............BT Connect Success.......");
  delay(3000);
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
        Serial.println(out_data);
        Flag = 'N';
  }
  out_dust = atoi(out_data);
}
//=========================================================================
