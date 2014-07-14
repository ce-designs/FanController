/* 
* Fan.cpp
*
* Created:	13-4-2014 13:28:31
* Author:	CE-Designs
* Version:	1.01 
*
* Modified: 13-7-2014
* Reason:	The pinMode was not set for the fan control pin
*/

#include "Fan.h"

// default constructor
Fan::Fan(int pwmControlPin, int minValue)
{
	FanControlPin = pwmControlPin;
	pinMode(pwmControlPin, OUTPUT);
	MinValue = minValue;
} //Fan


// Sets the current value, thus sets the fan speed
void Fan::SetSpeed(int value)
{
	CurrentValue = value;
	analogWrite(FanControlPin, CurrentValue);
}

// gets the current value
int Fan::GetSpeed()
{
	return CurrentValue;
}

// Starts the fan at full speed to be sure
// that it start correctly. After 1500 ms the fan
// speed is set to the last known value or at the minimal
// speed value of the last known value is zero 
void Fan::Start()
{
	SetSpeed(255);
	delay(1500);
	if (CurrentValue == 0)
	{
		CurrentValue = MinValue;
	}
	SetSpeed(CurrentValue);
}

// Starts the fan at full speed to be sure
// that it start correctly. After 1500 ms the fan
// speed is set to match the integer value
void Fan::Start(int value)
{
	SetSpeed(255);
	delay(1500);
	SetSpeed(value);
}

// Starts the fan at full speed
void Fan::StartAtFullSpeed()
{
	SetSpeed(255);
}

// stops the fan completely
void Fan::Stop()
{
	SetSpeed(0);
}

// gets lowest possible speeds where the fan still respond correctly
int Fan::LowestSpeed()
{
	return MinValue;
}

// gets highest possible fan speed
int Fan::HighestSpeed()
{
	return 255;
}