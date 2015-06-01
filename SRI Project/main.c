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
#include "BTProtocol/BTProtocol.h"
#include "BTProtocol/Serializer.h"
#include "Timing/Timing.h"
#include "utile.h"

volatile uint8_t sradc0, shouldBlink=0;

int main(void)
{
	
	initBTProtocol();
	initTiming();
	initLeds();
	initEngines();
	initTimeQueue();
	initSensors();
	initEncoders();
	sei();
	
	DDRA |= _BV(PINA4);
	DDRA |= _BV(PINA5);
	PORTA |= _BV(PINA4);
	
	
	BTTransmitStr("  >>>main start<<<  ");	
	
    while(1)
    {
		
		checkTimeQueue();
		//BTTransmitStr("-main loop-");
    }
}