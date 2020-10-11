//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
    int o_data = 0, i_data = 0;

    o_data = BT_Read()+oCal;
    if ( oCal == o_data ) { 
      bluetooth_flag = 0;
    } else {
      bluetooth_flag = 1;
    }

    i_data =  get_inner_dust()+iCal;
    drawGraph(i_data,o_data);
      Serial.print("Out Data : ");
    Serial.print(o_data);
       Serial.print("              In Data : ");
    Serial.println(i_data);
    

    
   if ( wifi_Flag == 1 ) {
    sendData2Server(i_data,o_data);
   }
   else {
    Serial.println("WiFi Disconnected............"); 
    wifi_Flag = 0;
    WiFi_Connect();  
   }
   o_data = 0;
   delay(20000);
}
