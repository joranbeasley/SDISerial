SDISerial 
=========

An SDI-12 Serial Library for Arduino.

This library was designed around filling a niche that was lacking and allow Arduino boards to communicate freely with SDI-12 Serial peripherals.  It is largely based off of the Arduino SoftwareSerial Library, modified to support the SDI12-Serial Specifications.

Works flawlessly with an assortment of [Decagon Devices Soil Moisture Sensors.](http://www.decagon.com/products/soils/)

Verified for the following boards: 
   - [Arduino UNO](http://arduino.cc/en/Main/arduinoBoardUno)
   - [Arduino MEGA2560](http://arduino.cc/en/Main/ArduinoBoardMega2560)
   
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

You instansiate the SDISerial class as follows
> ####SDISerial::SDISerial(uint8_t DATAPIN,uint8_t INVERTED = true)	
> Arguments:<br/>&nbsp;&nbsp;&nbsp;  **DATAPIN(_integer_)** : the pin on the arduino that is connected to the data line of the sensor <br/>
> &nbsp;&nbsp;&nbsp;**INVERTED(_integer_)**: this should **always** be true for standard SDI-12 Communications<br/>
> Returns  : SDISerial Object <br/><br/>
> Example  : `SDISerial communicator(2,1); // attach SDISerial object to PIN 2, invert the signal`
> **NOTE IN ORDER TO RECIEVE DATA THE _DATAPIN_ MUST HAVE INTERRUPTS ENABLED <br/>_(SEE http://arduino.cc/en/Reference/attachInterrupt)_**<br/>


Once instanciated you must initialize it once the Arduino has done its stuff, typically you will initialize it in the `void setup()` method of arduino sketches

> ####SDISerial::begin()
> Arguments : NONE <br/>
> Returns     : NONE <br/>
> _Simply initializes our data pin and gets our library prepared to communicate_ <br/><b/>
> Example     :  `void setup(){ communicator.begin(); }`
    
Last, but certainly not least is the communication with a sensor

> ####SDISerial::sdi_cmd(char* command)
> Arguments: **command(_char*_)** : the command to send the device<br/>
> Returns    : NONE <br/>
> _this method simply  sends a command to the device, use this method when you are not expecting a response back from the sensor_<br/><br/>
> Example : `communicator.sdi_cmd("0A1"); //change address of sensor from 0 to 1`
    
    
> ####SDISerial::sdi_query(char* command,uint32_t timeout_ms)
> Arguments:<br/>&nbsp;&nbsp;&nbsp;&nbsp;**command(_char*)** : _see sdi_cmd documentation_<br/>
> &nbsp;&nbsp;&nbsp;&nbsp;**timeout_ms(uint32_t)** :  How long to wait (maximum) for a response, expectes miliseconds.<br/>
> Returns   : **result(_char*_)** : if there is no result or an empty result, it returns NULL<br/><br/>
> Example  :  `char* result = communicator.sdi_query("?R0!",1000) // get measurement, wait up to a second for the result`


thats it... pretty simple interface.

here is a very simple example (make sure you have connected your data, power, and ground lines as illustrated above)


	#include <SDISerial.h>
	#define DATA_PIN 2
	SDISerrial connection(DATA_PIN);
	char output_buffer[125]; // just for uart prints
	//initialize variables
	void setup(){
	      connection.begin();
	      Serial.begin(9600);//so we can print to standard uart
	}
	//main loop
	void loop(){
	    //print to uart
	    Serial.println("Begin Command: ?R0!");
	    char* resp = connection.sdi_query("?R0!",1000);//1 second timeout
	    sprintf(output_buffer,"RECV: %s"",resp?resp:"No Response Recieved!!");
	    Serial.println(output_buffer);
	    delay(10000);//sleep for 10 seconds before the next read
	}