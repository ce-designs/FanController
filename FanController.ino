/*
 * Released into the public domain. 
 *
 * Created: 13-4-2014
 * Author:	CE-Designs
 * http://ce-designs.net/
 *
 * This sketch runs two fans temperature controlled fans.
 * For more information about the schematics visit the web page below.
 * http://ce-designs.net/index.php/my-projects/other-builds/temperature-controlled-fans
 *
 * Some general notes about the usage if the sketch:
 *	- The threshold temperature values for turning the fans on or
 *	  off are by set the fanOnTemp and fanOffTemp constants. 
 *	- Because the start voltage of a fan is depending on the 
 *	  type of fan, one needs to figure out the minimal analog write
 *	  value that ensures normal operation (usually between 125 and 150).
 *	  This can be set by setting MIN_AW_VALUE.
 *
 *
 * Modified: 14-7-2014
 * Sleep mode and watchdog functionality added for low power consumption:  
 *	- The time after which the uC goes to sleep can be set by changing
 *	  the watchdogInterrupt constant.
 *	- The time after which the controller wakes up and resumes the program
 *	  can be set by changing the watchdogInterrupt wakeUpValue constant.
 * Some refactoring of duplicate code at the handleFan methods
 *
 *
 * The sleep mode and watchdog methods where borrowed from the 
 * ATtiny85_watchdog_example from InsideGadgets.
 * http://www.insidegadgets.com/2011/02/05/reduce-attiny-power-consumption-by-sleeping-with-the-watchdog-timer/
 */

#include <avr/sleep.h>  // include the sleep mode library
#include <avr/wdt.h>	// include the watch dog timer library
#include "Fan.h"		// include the fan control library
#include "LM35.h"		// include the LM35 temperature sensor library

//#define DEBUG			// uncomment for debugging

// sleep mode stuff
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// pin definitions
#define FAN1_PIN 0		// (digital) pin of fan (1)
#define FAN2_PIN 1		// (digital) pin of fan (2)
#define TEMP1_PIN 3		// (analog)  pin of LM35 (1)
#define TEMP2_PIN 2		// (analog)  pin of LM35 (2)

// fan instantiation 
#define MIN_AW_VALUE 150				// 150 is the lowest analog write value from where the fan responds normal (max = 255)
Fan fanLeft(FAN1_PIN, MIN_AW_VALUE);		
Fan fanRight(FAN2_PIN, MIN_AW_VALUE);

// temperature sensor instantiation
LM35 lm35Left(TEMP1_PIN, CELSIUS);		// or use: FAHRENHEIT
LM35 lm35Right(TEMP2_PIN, CELSIUS);		// or use: FAHRENHEIT

// constants
const unsigned long SLEEP_MILLIS = 60000L;	// time in milliseconds after which the controller goes to sleep (time of inactivity)
const unsigned long READ_MILLIS = 2500L;	// time in milliseconds between each read
const int watchdogInterrupt = 9;			// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms, 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
const int wakeUpValue = 15;					// the controller continues the program when the watchdogCounter equals this value
const float fanOnTemp = 52.0F;				// the fans are turned on when the temperature is exceeded
const float fanOffTemp = 48.0F;				// the fans are turned off when the temperature sinks below this value (must be <= fanOnTemp!)
 
// variables
volatile int watchdogCounter = 0;		// counter for the watchdog timer
unsigned long readMillis = 0L;			// for recording the time after each read;
unsigned long sleepMillis = 0L;			// for recording the time after each read;

// main entry point
void setup()
{
#ifdef DEBUG
	Serial.begin(9600);
#endif
	setup_watchdog(9);		// approximately 8 seconds sleep
	readMillis = millis();	// start recording
	sleepMillis = millis();	// start recording
} // end setup

// main loop
void loop()
{	
	// check if it's time to enter sleepMode 
	if (millis() - sleepMillis >= SLEEP_MILLIS)
	{			
		setFanPins(INPUT);		// set used ports to input to save some more power						
		
		// keep in sleep mode while the watchdogCounter smaller as the wakeUpvalue
		while (watchdogCounter < wakeUpValue) // approximately 2 minutes (15 * 8 seconds)
		{
			system_sleep();		// enter sleep mode (again)
		}
		
		setFanPins(OUTPUT);		// set used ports to output
		
		watchdogCounter = 0;	// reset the watchdog counter
		sleepMillis = millis();	// start recording again
	}	
	
	// read and compute at the set interval!
	if (millis() - readMillis >= READ_MILLIS)
	{	
		handleFan(fanLeft, lm35Left);	
		handleFan(fanRight, lm35Right);
		readMillis = millis();	// record millis			
	}	
} // end main loop

// sets the ports that are used to control the fans
void setFanPins(uint8_t value)
{
	pinMode(FAN1_PIN, value);	
	pinMode(FAN1_PIN, value);
}

// reads the temperature from the sensor and decides what to do with fan
void handleFan(Fan &fan, LM35 &lm35)
{
	float temp = lm35.read();
	#ifdef DEBUG
	Serial.print("temp: ");Serial.println(temp);
	#endif
	if (temp > fanOnTemp && fan.GetSpeed() == 0)	//  temperature exceeds the fan-on-temperature and the fan is turned off
	{
		fan.Start(fan.LowestSpeed());	// start the fan
		sleepMillis = millis();
		#ifdef DEBUG
		Serial.println("fan start");
		#endif
	}
	else if(temp > fanOnTemp && fan.GetSpeed() > 0)	//  temperature exceeds the fan-on-temperature and the fan is already turned on
	{
		int i = (int)(fan.LowestSpeed()+((temp - fanOnTemp) / 0.11));
		if (i > 255)
		{
			i = 255;
		}
		if (i != fan.GetSpeed())
		{
			fan.SetSpeed(i); // set the speed (+20 for each centigrade above 50)
		}
		sleepMillis = millis();
		#ifdef DEBUG
		Serial.print("fan speed: ");Serial.println(fan.GetSpeed());
		#endif
	}
	else if (temp <= fanOffTemp && fan.GetSpeed() > 0)		// temperature is lower or same as the fan-off-temperature
	{
		fan.Stop();	// stop the fan
		#ifdef DEBUG
		Serial.println("fan stop");
		#endif
	}
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) 
{
	byte bb;
	int ww;
	if (ii > 9 ) ii=9;
	bb=ii & 7;
	if (ii > 7) bb|= (1<<5);
	bb|= (1<<WDCE);
	ww=bb;

	MCUSR &= ~(1<<WDRF);
	// start timed sequence
	WDTCR |= (1<<WDCE) | (1<<WDE);
	// set new watchdog timeout value
	WDTCR = bb;
	WDTCR |= _BV(WDIE);
}

// set system into the sleep state
// system wakes up when watchdog is timed out
void system_sleep() 
{
	cbi(ADCSRA, ADEN);                    // switch Analog to Digital converter OFF, save power

	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
	sleep_enable();

	sleep_mode();                        // System sleeps here

	sleep_disable();                     // System continues execution here when watchdog timed out
	sbi(ADCSRA, ADEN);                    // switch Analog to Digital converter ON
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) 
{
	watchdogCounter+=1;  // raise watchdog counter
}