/*
 SDISerial.h (formerly SoftwareSerial.h) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/
#define DEBUG 1
#define _SS_MAX_RX_BUFF 255 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif
inline void DebugPulse(uint8_t pin, uint8_t count);

//! SDISerial
/*!
   An Arduino library to facilitate simple communication with SDI-12 Devices, that
   are common with soil moisture sensors
   \author Joran Beasley
*/
class SDISerial : public Stream
{
private:
  // per object data
  uint8_t _receivePin;
  uint8_t _receiveBitMask;
  volatile uint8_t *_receivePortRegister;
  uint8_t _transmitBitMask;
  volatile uint8_t *_transmitPortRegister;

  uint16_t _rx_delay_centering;
  uint16_t _rx_delay_intrabit;
  uint16_t _rx_delay_stopbit;
  uint16_t _tx_delay;

  uint16_t _buffer_overflow:1;
  uint16_t _inverse_logic:1;
  

  //sdi12 stuff 
  char response[255];
  bool response_ready;
  
  // static data
  static char _receive_buffer[_SS_MAX_RX_BUFF]; 
  static volatile uint8_t _receive_buffer_tail;
  static volatile uint8_t _receive_buffer_head;
  static SDISerial *active_object;
  
  // private methods
  void recv();
  uint8_t rx_pin_read();
  void tx_pin_write(uint8_t pin_state);
  
  void setTX(uint8_t transmitPin);
  void setRX(uint8_t receivePin);
  // private static method for timing
  static inline void tunedDelay(uint16_t delay);
  //private methods for sdi12
    bool isListening() { return this == active_object; }
  bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
  int peek();
  void end();
  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();
  bool listen();
  using Print::write;
  uint8_t* read_buffer();
  
public:
  // public methods
  //!Constructor
  /*!
     \param dataPin : The pin you have connected to the dataline
     \param inverse_logic(optional:TRUE) : inverted signal should always be true for SDI-12
     \returns a new SDISerial Connection
     
     \warning The dataPin **MUST** be a pin that supports interrupts<br/>
              SEE [Arduino Pins With Interrupts](http://arduino.cc/en/Reference/attachInterrupt)
     
     \code{.cpp}
	  #include <SDISerial.h>
	  #define DATA_PIN 2
	  SDISerial sdi(DATA_PIN);
	  \endcode
  */
  SDISerial(uint8_t dataPin, bool inverse_logic = true);
  

  //!Initializer
  /*!
    MUST be called before attempting any communication
    
    \code{.cpp}
	  void setup(){
			sdi.begin();
	   }
	  \endcode
  */
  void begin();
  
  
  //!Send Command To SDI-12 Sensor Bus
  /*!
    \param bytes : The query string to send
    
    \code{.cpp}
	  sdi.sdi_cmd("0A1!"); // change address from 0 to 1 
	  \endcode
  */
  void sdi_cmd(const char* bytes);
  
  
  //!Read a response string from the sensor based on a command query \see  response_complete_byte
  /*!
    \param timeout_ms : the maximum time to wait for a response in miliseconds
    \returns the response string or NULL in the event of a timeout
    
    \code{.cpp}
	  sdi.sdi_cmd("0I!"); // get info from sensor 0
	  char* sensor_info = sdi.wait_for_response(1000); // wait 1 second maximum
	  
	  \endcode
  */
  char* wait_for_response(uint32_t timeout_ms);
  
  
  //! query the sensor and wait for a response. \see  response_complete_byte
  /*!
    \param bytes : The query string to send
    \param timeout_ms : the maximum time to wait for a response in miliseconds
    \returns the response string or NULL in the event of a timeout
    
    \code{.cpp}
	  char* sensor_info = sdi.sdi_query("0I!",1000); // get info from sensor 0
	  \endcode
  */
  char* sdi_query(const char* bytes,uint32_t timeout_ms);
  
  
  //! convience function that takes advantage of service requests 
  //! service requests are requests that are returned in stages  "?M!" is typically the service request command
  /*!
    \param serviceRequest : The query string to initiate the service request(typically `"?M!"`)
    \param readCommand : the command that fetches the request once the sensor has provided it (typically "?D0!")
    \returns the response string from the read_command or NULL in the event of a timeout
    
	\code{.cpp}
	  char* current_measurement = sdi.service_request("0M!","0D0!"); // get measurement from sensor 0
	\endcode
  */
  char* service_request(const char* serviceRequest,const char* readCommand);
  
  char response_complete_byte; /*!< specifies the byte that signifies sensor has finished the message 
                                    default value is '\n'

                                    \code{.cpp}
                                    sdi.response_complete_byte = '\r'; 
                                    sdi.wait_For_response(1000);
									\endcode
                               */
  //\cond HIDDEN_SYMBOLS
  ~SDISerial(){end();}
  // public only for easy access by interrupt handlers
  static inline void handle_interrupt();
  //\endcond
  
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif
