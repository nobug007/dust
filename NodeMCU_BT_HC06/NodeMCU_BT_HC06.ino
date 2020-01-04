//
//  FILE : NodeMCU_BT_HC06
//  AUTHOR : nobug (nobug007@gmail.com)
//  CREATED : 4.1.2020
//  HW : NodeMCU & HC06
//
#include <string.h>
#include <ctype.h>
#include <SoftwareSerial.h>

// BT Port Set up
SoftwareSerial bluetooth(D1, D2);


//=======================================================================
//                    Power on setup
//=======================================================================


void setup() {
  delay(1000);
  Serial.begin(9600);
  // BT setup
  bluetooth.begin(9600);
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  char out_data[10];
  int out_i=0;
  char Flag = 'N';

  while (bluetooth.available()) {
    // 수신 받은 데이터 저장
    out_data[out_i] = (char)bluetooth.read();
    out_data[++out_i] = NULL;
    // 수신된 데이터 시리얼 모니터로 출력
    Flag = 'Y';
  }
  if ( Flag == 'Y' ) {
        Serial.print("BT Read Out Data :  ");
        Serial.println(out_data);
  }
}
//=========================================================================
