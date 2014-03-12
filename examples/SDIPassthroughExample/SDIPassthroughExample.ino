#include <SDISerial.h>


/*
I used a few devices with no problem using a MEGA 2560 as well as an UNO.
Sketch was built with Arduino 1.0.4, however I also tested it under 1.0.0
Place the SDISerial folder in    "<ARDUINO_ROOT>/libraries"

with the 5TE 
the WHITE wire is power. 
   - I hooked it up to the arduino 5v output pin, however you could also connect it to a pin and drive power only when you wanted it
the RED wire is the DATA_LINE.
   - you must hook it up to a pin that can process interrupts (see link below)
   
the remaining wire must be connected to ground
*/

//in order to recieve data you must choose a pin that supports interupts
#define DATALINE_PIN 2
#define INVERTED 1
#define MAX_TIMEOUT 1000
//see:   http://arduino.cc/en/Reference/attachInterrupt
//for pins that support interupts (2 or 3 typically)


SDISerial connection(DATALINE_PIN, INVERTED);
char b_in[125];
char output_buffer[255];
char tmp_buffer[10];


void wait_for_message(char* buffer,char terminal){
   Serial.println("Waiting For input...");
   int i=0;
   while( true){
     if(!Serial.available())delay(500);
     else{
        buffer[i] = Serial.read();
        if(buffer[i] == terminal){
          buffer[i+1] = '\0';
          return;
        }
        i+=(buffer[i] >= 32 && buffer[i] <= 127);
     } 
   }
   
}

void setup(){
   Serial.begin(9600);
   connection.begin();
   delay(1000);
   Serial.println("Initialization Complete");
   
}
void loop(){
   
   wait_for_message(b_in,'!');
   sprintf(output_buffer,"[out]:%s",b_in?b_in:"No Output");
   Serial.println(output_buffer);
   char *response =connection.sdi_query(b_in,MAX_TIMEOUT);
   sprintf(output_buffer,"[in]:%s",response?response:"No Response");
   Serial.println(output_buffer);
   Serial.flush();
   delay(1000);
   
}
