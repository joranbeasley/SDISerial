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
//see:   http://arduino.cc/en/Reference/attachInterrupt
//for pins that support interupts (2 or 3 typically)


SDISerial connection(DATALINE_PIN, INVERTED);

char output_buffer[125];

void setup(){
  connection.begin(); // start our SDI connection 
  Serial.begin(9600); // start our uart
  Serial.println("OK INITIALIZED"); // startup string echo'd to our uart
  delay(3000); // startup delay to allow sensor to powerup and output its DDI serial string
  char* sensor_info = connection.sdi_query("?I!",1000); // get sensor info
  //print to uart
  sprintf(output_buffer,"Sensor Info: %s",sensor_info?sensor_info:"No Response");
  Serial.println(output_buffer);
}

int j=0;
void loop(){
  char* measurement_data = connection.service_request("?M!","?D0!"); // get measurement data
  //print to uart
  sprintf(output_buffer,"RECV:%s",measurement_data?measurement_data:"No Response!");
  Serial.println(output_buffer); //just a debug print statement to the serial port
  //sleep a while
  delay(5000);
}
