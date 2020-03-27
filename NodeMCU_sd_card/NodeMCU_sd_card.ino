
#include <SPI.h>
#include <SD.h>

//   SD Card  Pin 
//
//   VCC  -> UV  ( 5V )
//   CS   -> D8 ( Changable )
//   SCK  -> D5
//   MOSI -> D7
//   MISO -> D6



void setup() {
  Serial.begin(9600);
  init_SD();
}


/*   컨피그 화일이 없으면 이것을 풀어 먼저 컨피그 화일을 만들어 넣고, 시작 하세요. 
void init_SD() {
  char c;
  char s[100];
  int i;

  File myFile;

  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {    // D8은 SD CS 핀입니다.
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("Config.txt")) {
    Serial.println("Config.txt exists.");
  } else {
     myFile = SD.open("Config.txt", FILE_WRITE);

    Serial.println("Config.txt doesn't exist. It made.");
  }
    myFile.write("APN=Nobug_Home\n");
    myFile.write("PASS=12345678\n");
    myFile.write("Name=Nobug\n");
    myFile.write("iCal=0.001\n");
    myFile.write("oCal=0.002\n");
           myFile.close();
           Serial.println("finished Config.txt");
}

*/

void init_SD() {
  char c;
  char s[100];
  int i;

  File myFile;

  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {    // D8은 SD CS 핀입니다.
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("config.txt")) {
    Serial.println("config.txt exists.");
  } else {
    Serial.println("config.txt doesn't exist.");
  }

  Serial.println("reading config.txt...");
  myFile = SD.open("config.txt", FILE_READ);
  if (myFile) {
        Serial.println("Config.txt:");
          // read from the file until there's nothing else in it:
          i = 0;
          while (myFile.available()) {
              c = myFile.read();
              if ( c == '\n' ) { 
                Config_data(s);
                i= 0;
              } else {
                s[i++] = c;
                s[i] = NULL;
              }
          }
  // close the file:
           myFile.close();
      } else {
    // if the file didn't open, print an error:
      Serial.println("error opening config.txt");
      }
}

void Config_data(char s[100]){
  int j=0,k=0;
  char title[50];
  char s_value[50];
  char flag = 'T';
  while(s[j] != NULL ) {
     if ( s[j] == '=' ) {
        flag = 'F';
        k=0;
        j++;
     }
     if ( flag == 'T' ){
       title[k] = s[j];
       title[k+1] = NULL;
     } else {
       s_value[k] = s[j];
       s_value[k+1] = NULL;
     } 
     k++;
     j++;
  }
  Serial.print("Title = ");
  Serial.print(title);
  Serial.print("       Value = ");
  Serial.println(s_value); 
}

void loop() {
}
