// Tachometer.h

#ifndef _TACHOMETER_h
#define _TACHOMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TachometerClass
{
 private:
	 void HallSensorHandler();
	 unsigned long ElapsedTime(unsigned long last_time);

 protected:


 public:
	void init(byte hallSensor, byte hallSensorMagnets);
	unsigned int getRPM();
};

extern TachometerClass Tachometer;

#endif