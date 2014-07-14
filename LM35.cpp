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


#include "LM35.h"

// default constructor
LM35::LM35(int tempPin, uint8_t unitOfMeasurement)
{	
	this->TempPin = tempPin;
	this->UnitOfMeasurement = unitOfMeasurement;
	this->Resolution = DEFAULT_RESOLUTION;	
} //LM35

// default constructor
LM35::LM35(int tempPin, uint8_t unitOfMeasurement, uint8_t resolution)
{	
	this->UnitOfMeasurement = unitOfMeasurement;
	this->Resolution = resolution;
	this->TempPin = tempPin;	
} 

void LM35::begin()
{
	applyResolution();	
}

float LM35::read()
{	
	switch (this->UnitOfMeasurement + this->Resolution)
	{
		case 5:
		return readCelcius(measureFoutTimes());
		case 6:
		return readFahrenheit(measureFoutTimes());
		case 9:
		return readCelciusHighRes(measureFoutTimes());		
		case 10:
		return readFahrenheitHighRes(measureFoutTimes());
		default:
		return 0;
		break;
	}
}

float LM35::measureFoutTimes()
{
	float avg = 0;
	for (uint8_t i = 0; i < 4; i++)
	{
		avg += analogRead(this->TempPin);;
	}
	return avg / 4;
}

float LM35::readCelcius(int reading)
{		
	return (5.0 * reading * 100.0) / 1024.0;
}

float LM35::readCelciusHighRes(int reading)
{
	return reading / 9.31;
}

float LM35::readFahrenheit(int reading)
{	
	return (((5.0 * reading * 100.0) / 1024.0) * 9.0 / 5.0) + 32.0;
}

float LM35::readFahrenheitHighRes(int reading)
{
	return ((reading / 9.31) * 9.0 / 5.0) + 32.0;
}

void LM35::useHighResolution()
{
	analogReference(INTERNAL);
}

void LM35::useDefaultResolution()
{
	analogReference(DEFAULT);
}

void LM35::applyResolution()
{
	switch (this->Resolution)
	{
		case DEFAULT_RESOLUTION:
		analogReference(DEFAULT);
		break;
		case HIGH_RESOLUTION:
		analogReference(INTERNAL);
		break;
		default:
		analogReference(DEFAULT);
		break;
	}
}