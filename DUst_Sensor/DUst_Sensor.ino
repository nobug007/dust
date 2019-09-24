
#include <SDS011.h>

float p10, p25;
int err;

SDS011 my_sds;

int rxPin = 7;
int txPin = 6;

void setup() {
  my_sds.begin(rxPin,txPin);
  Serial.begin(9600);
}

void loop() {
  err = my_sds.read(&p25, &p10);
  if (!err) {
    Serial.print("P2.5: " + String(p25));
    Serial.println("            P10:  " + String(p10));
  }
  delay(100);
}
