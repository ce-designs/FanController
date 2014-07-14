/* 
* Fan.h
*
* Created: 13-4-2014 13:28:32
* Author: CE-Designs
* Version: 1.0.0.0 
*/


#ifndef __FAN_H__
#define __FAN_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Fan
{
//variables
public:	
	
protected:

private:

	int MinValue;		// the lowest value where the fan still responds properly
	int FanControlPin;	// PWM pin that controls the fan
	int CurrentValue;	// holds the current value of the fan, which relates to the fan speed
	
	
//functions
public:
	Fan(int pwmControlPin, int minValue);
	
	void SetSpeed(int value);
	int GetSpeed();
	void Start();
	void Start(int value);
	void StartAtFullSpeed();
	void Stop();
	
	int LowestSpeed();
	int HighestSpeed();
	
protected:
private:
	

}; //Fan

#endif //__FAN_H__
