// Tachometer.h

#ifndef _TACHOMETER_h
#define _TACHOMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Tachometer
{
 protected:
	 byte _hallSensorMagnets = 1;
	 unsigned int _turnCounter = 0;
	 unsigned long _turnTimeCounter = 0;
	 unsigned long _lastTurnTime = 0;


 public:
	 void HallSensorHandler();
	 void init(byte hallSensorMagnets = 1);
	 unsigned int getRPM();
	 unsigned long ElapsedTime(unsigned long lastTime);
};

extern Tachometer tachometer;

#endif