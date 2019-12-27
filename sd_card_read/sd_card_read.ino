
#include <SPI.h>
#include <SD.h>

void setup() {
  Serial.begin(9600);
  init_SD();
}
void init_SD() {
  char c;
  char s[100];
  int i;

  File myFile;

  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {    // 10은 SD CS 핀입니다.
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("Test.txt")) {
    Serial.println("config.txt exists.");
  } else {
    Serial.println("config.txt doesn't exist.");
  }

  Serial.println("writing config.txt...");
  myFile = SD.open("Test.txt", FILE_READ);
  if (myFile) {
        Serial.println("test.txt:");
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
      Serial.println("error opening test.txt");
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
