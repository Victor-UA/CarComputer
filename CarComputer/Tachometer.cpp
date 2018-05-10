// 
// 
// 

#include "Tachometer.h"
#define Max_Millis 4294967295

byte _hallSensorMagnets = 1;
byte _hallsensor = 2;

unsigned int _turnCounter = 0;
unsigned long _turnTimeCounter = 0;
unsigned long _lastTurnTime = 0;

//hallSensor range: [2, 3]
void TachometerClass::init(byte hallSensor, byte hallSensorMagnets)
{
	if (hallSensor >= 2 && hallSensor <= 3)
	{
		_hallsensor = hallSensor;
		pinMode(_hallsensor, INPUT);
		attachInterrupt(digitalPinToInterrupt(hallSensor), this->HallSensorHandler, RISING);
		_hallSensorMagnets = hallSensorMagnets;		
	}
	else
	{
		Serial.println("Error!\nHall sensor has to be plugged to D2 or D3.");
	}
}

unsigned int TachometerClass::getRPM() 
{
	unsigned int turnSpeed = (float)_turnCounter / _turnTimeCounter / _hallSensorMagnets * 1000 * 60;
	_turnCounter = 0;	
	_turnTimeCounter = 0;	
	return turnSpeed;
}

void TachometerClass::HallSensorHandler() {
	if (_lastTurnTime != 0)
	{
		_turnCounter++;
		_turnTimeCounter += this->ElapsedTime(_lastTurnTime);
	}
	_lastTurnTime = millis();
}

unsigned long TachometerClass::ElapsedTime(unsigned long lastTime) {
	return lastTime <= millis() ? millis() - lastTime : Max_Millis - lastTime + millis();
}


TachometerClass Tachometer;