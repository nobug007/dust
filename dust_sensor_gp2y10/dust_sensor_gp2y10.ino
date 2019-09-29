int measurePin = A0;      // Connect dust sensor to arduino A0 pin
int ledPower = 2;         // Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0.0;
float calcVoltage = 0.0;
float dustDensity = 0.0;

float dust;

void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
}
void loop() {
  dust = 0;
  for(int i=0;i<5;i++) {
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

  dustDensity = (0.17 * calcVoltage -0.1)* 1000 ;// -0.1;
  delay(1000);

  return dustDensity;
/*
  Serial.print("Raw signal Valure :  " );
  Serial.print(voMeasured);

  Serial.print("   -   votage :  ");
  Serial.print(calcVoltage);
  
  Serial.print("   -   Dust density :  ");
  Serial.println(dustDensity);
*/
}
