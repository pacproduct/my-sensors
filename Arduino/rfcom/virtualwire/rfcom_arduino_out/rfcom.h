/* rfcom library

*/
#ifndef rfcom_H
#define rfcom_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <VirtualWire.h>

// Uncomment to enable printing out nice debug messages.
//#define rfcom_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef rfcom_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif




#ifndef RFCOM_SPEED
//bauds for VirtualWire setup
	#define RFCOM_SPEED 2000
#endif

#ifndef RFCOM_RX_PIN
//receiver pin
	#define RFCOM_RX_PIN 11
#endif

#ifndef RFCOM_TX_PIN
//transmitter pin
	#define RFCOM_TX_PIN 12
#endif

#ifndef RFCOM_MAX_MESSAGE_LENGTH
//maximum length of the message
	#define RFCOM_MAX_MESSAGE_LENGTH 64
#endif


class rfcom {
  public:
   rfcom();
   void init();
   void start_listening();
   void stop_listening();
   int send_message(String message);
   String get_message(unsigned long milliseconds);
   
  private:
    uint8_t buf_message[RFCOM_MAX_MESSAGE_LENGTH];
};

#endif
