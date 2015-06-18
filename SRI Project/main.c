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
	setNeonSetting(0);	
	
	BTTransmitStr("  >>>main start<<<  ");	
	sendCarStartedSignal();
    while(1)
    {
		checkTimeQueue();
    }
}