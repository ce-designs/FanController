/*
* LM35.cpp
*
* Created:	13-4-2014 16:31:40
* Author:	CE-Designs
* Version:	1.01
*
* Modified: 13-7-2014
* Reason:	To ensure more accurate reading the sensor is
*			now being read 4 times and the average of the
*			measurements is returned
*			Also changed the way that the analog reference
*			is being set
*/


#ifndef __LM35_H__
#define __LM35_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define CELSIUS 1
#define FAHRENHEIT 2

#define DEFAULT_RESOLUTION 4
#define HIGH_RESOLUTION 8
//
class LM35
{
//variables
public:
	
protected:
		
private:

	int TempPin;
	uint8_t Resolution;
	uint8_t UnitOfMeasurement;

//functions
public:

	LM35(int tempPin, uint8_t unitOfMeasurement);
	LM35(int tempPin, uint8_t unitOfMeasurement, uint8_t resolution);
	
	void begin();
	float read();	
	void useHighResolution();
	void useDefaultResolution();
	
protected:

private:

	float measureFoutTimes();
	float readCelcius(int reading);
	float readFahrenheit(int reading);
	float readCelciusHighRes(int reading);
	float readFahrenheitHighRes(int reading);
	void applyResolution();

}; //LM35

#endif //__LM35_H__
