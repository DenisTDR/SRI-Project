/*
 * SRI_Project.c
 *
 * Created: 5/5/2015 12:49:42 AM
 *  Author: NMs
 */ 
#define F_CPU 8000000UL // 8 MHz

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Car/Sensors.h"
#include "Car/Engines.h"
#include "Car/Encoders.h"
#include "Car/Lights.h"
#include "BTProtocol/BTProtocol.h"
#include "BTProtocol/Serializer.h"
#include "Timing/Timing.h"
#include "utile.h"
#include "Settings.h"

volatile uint8_t sradc0, shouldBlink=0;

uint8_t blinkLeds(){
	PORTA ^= _BV(PINA4);
	PORTA ^= _BV(PINA5);
	return NO;
}


int main(void)
{
	initBTProtocol();
	initTiming();
	initEngines();
	initLights();
	initSensors();
	initEncoders();
	sei();
	
	
	setDebugging(1);
	setReadingSensors(1);
	
	//addEntryIfNotExists(&blinkLeds, 500*1000UL, Periodic);
	//addEntryIfNotExists(&blinkA1, 500UL*1000UL, Periodic);
	BTTransmitStr("  >>>main start<<<  ");	
	
    while(1)
    {
		
		checkTimeQueue();
		//BTTransmitStr("-main loop-");
    }
}