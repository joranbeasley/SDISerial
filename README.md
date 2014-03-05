SDISerial 
=========

An SDI-12 Serial Library for Arduino.

This library was designed around filling a niche that was lacking and allow Arduino boards to communicate freely with SDI-12 Serial peripherals.  It is largely based off of the Arduino SoftwareSerial Library, modified to support the SDI12-Serial Specifications.

Works flawlessly with an assortment of [Decagon Devices Soil Moisture Sensors.](http://www.decagon.com/products/soils/)

Verified for the following boards: 
   - [Arduino UNO](http://arduino.cc/en/Main/arduinoBoardUno)
   - [Arduino MEGA2560](http://arduino.cc/en/Main/ArduinoBoardMega2560)

This is currently untested with more than one device on the serial bus

INSTALLATION
------------
###Option 1: Git checkout###
1. Open your terminal and cd to your root arduino folder
	- I use `cd C:\Arduino\arduino-1.0\`
	- you will know your in the right place because it will contain "arduino.exe"
2. Change to your libraries folder `cd libraries`
3. Clone the git repository `git clone git@github.com:joranbeasley/SDISerial.git`
	
###Option 2: Zip File###
1. Download Zipfile from link on the right
2. Unzip to the libraries folder listed above
3. Rename the extracted folder from `SDISerial-Master` to `SDISerial`
	
INSTALATION VERIFICATION
------------------------
1. Open The Arduino IDE by running `arduino.exe` in the <Arduino_Root> folder.
2. Goto File > Examples > SDISerial > SDISerialExample

The wiring for the example program is 
![Wiring Diagram](examples/ATMEGA_SDI_HOOKUP.png)

DOCUMENTATION
-------------
###[READ THE DOCS](https://dl.dropboxusercontent.com/u/18004504/Arduino/SDISerial/index.html)

SIMPLE EXAMPLE
--------------

```c++

#include <SDISerial.h>
#include <string.h>
#define DATA_PIN 2
SDISerial connection(DATA_PIN);
char output_buffer[125]; // just for uart prints

char sensor_info[]
//initialize variables
void setup(){
      connection.begin();
      Serial.begin(9600);//so we can print to standard uart
      //small delay to let the sensor do its startup stuff
      delay(3000);//3 seconds should be more than enough
	  
	  //Simple Query  
	  char* sensor_info = connection.sdi_query("0I!"); // get the info for the sensor at address 0 
	  //if you dont know the address you could send connection.sdi_query("?I!");
	  sprintf(output_buffer,"SENSOR INFO: %s",sensor_info?sensor_info:"No Response Recieved!!");
      Serial.println(output_buffer);
}

//main loop
void loop(){
    //print to uart
    Serial.println("Begin Command: ?R0!");
    
    //send measurement query 
    char* measurement_data = connection.service_request("0M!","0D0!");//get measurement data from sensor 0
    
    sprintf(output_buffer,"RECV: %s",measurement_data?measurement_data:"No Response Recieved!!");
    Serial.println(output_buffer);
    delay(10000);//sleep for 10 seconds before the next read
}
```