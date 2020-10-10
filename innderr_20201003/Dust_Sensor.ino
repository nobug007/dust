//=======================================================================
//                    Get Dust inform
//=======================================================================

int get_inner_dust() {

  int dust = 0;
  
  for (int i=0;i<5;i++) {
    dust += dust_check();
  }
  dust = dust / 5.0;
//  Serial.print("   -  5 times inner Dust density :  ");
//  Serial.println(dust);
  return (int)dust;
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
  delay(3000);

  return dustDensity;
}
