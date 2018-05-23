// 
// 
// 

#include "Tachometer.h"
#define Max_Millis 4294967295

//hallSensor range: [2, 3]
void Tachometer::init(byte hallSensorMagnets = 1)
{
	this->_hallSensorMagnets = hallSensorMagnets;				
}

unsigned int Tachometer::getRPM() 
{
	unsigned int turnSpeed = (float)_turnCounter / _turnTimeCounter / _hallSensorMagnets * 1000 * 60;
	_turnCounter = 0;	
	_turnTimeCounter = 0;
	return turnSpeed;
}

void Tachometer::HallSensorHandler() {
	if (_lastTurnTime != 0)
	{
		_turnCounter++;
		_turnTimeCounter += ElapsedTime(_lastTurnTime);
	}
	_lastTurnTime = millis();
}

unsigned long Tachometer::ElapsedTime(unsigned long lastTime) {
	return lastTime <= millis() ? millis() - lastTime : Max_Millis - lastTime + millis();
}


Tachometer tachometer;

