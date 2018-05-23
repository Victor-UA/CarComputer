// Termometer_DHT11.h

#ifndef _TERMOMETER_DHT11_h
#define _TERMOMETER_DHT11_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Termometer_DHT11Class
{
 protected:


 public:
	void init();
};

extern Termometer_DHT11Class Termometer_DHT11;

#endif


