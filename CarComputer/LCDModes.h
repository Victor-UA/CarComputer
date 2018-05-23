// LCDModes.h

#ifndef _LCDModes_h
#define _LCDModes_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum LCDModes
{
	Mode_Tachometer,
	Mode_Temperature_FrontInterior
};

#endif


