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
> ####SDISerial(uint8_t DATAPIN,uint8_t INVERTED)	
> **NOTE IN ORDER TO RECIEVE DATA THE _DATAPIN_ MUST HAVE INTERRUPTS ENABLED <br/>_(SEE http://arduino.cc/en/Reference/attachInterrupt)_**<br/>
    Arguments:<br/>&nbsp;&nbsp;&nbsp;  **DATAPIN(_integer_)** : the pin on the arduino that is connected to the data line of the sensor <br/>
                &nbsp;&nbsp;&nbsp;**INVERTED(_integer_)**: this should **always** be 1 for SDI-12<br/>
    Returns  : SDISerial Object <br/><br/>
    Example  : `SDISerial communicator(2,1); // attach SDISerial object to PIN 2, invert the signal`

