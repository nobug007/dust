//=========================================================================
//    SD Card
//=========================================================================


void init_SD() {
  char c;
  char s[200];
  int i;

  File myFile;

  Serial.print("Initializing SD card...");

  if (!SD.begin(D8)) {    // D8은 SD CS 핀입니다.
    Serial.println("initialization failed!");
 
     return;
  }
  Serial.println("initialization done.");   

  if (SD.exists("Config.txt")) {
    Serial.println("config.txt exists.");
  } else {
    Serial.println("config.txt doesn't exist.");
  }
  

  Serial.println("reading config.txt...");
  myFile = SD.open("Config.txt", FILE_READ);
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

void Config_data(char s[200]){
  int j=0,k=0;
  char title[50];
  char s_value[100];
  char temp[10];
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
  
//  APN=UXI_6
//  PASS=uxinsight\^
//  Name=Nobug
//  iCal=-115
//  oCal=60
//  Time=30  
//  URL=AKfycbyK-BAXk4EkgDBnqDUV6EcT4W72FzqcL-ez90RXHjn3wS-71Dvp

  switch(title[0]) {
    case 'A' :
      sprintf(ssid,"%s",s_value);
      Serial.println(ssid);
      break;
    case 'P' :
      sprintf(password,"%s",s_value);
      Serial.println(password);
      break;
    case 'N' :
      sprintf(DevName,"%s",s_value);
      Serial.println(DevName);
      break;
    case 'i' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      iCal = atoi(temp);
      break;
    case 'o' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      oCal = atoi(temp);
      break;
    case 'T' :
      sprintf(temp,"%s",s_value);
      Serial.println(temp);
      DelayTime = atoi(temp) * 1000;  // Sec
      break;
    case 'U' :
      sprintf(URL,"%s",s_value);
      Serial.println(URL);
      URL[strlen(URL)-1] = NULL;

      break;
  }
  Serial.print("Title = ");
  Serial.print(title);
  Serial.print("       Value = ");
  Serial.println(s_value); 
}
