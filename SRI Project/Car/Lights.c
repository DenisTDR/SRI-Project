/*
 * Lights.c
 *
 * Created: 6/3/2015 8:37:50 PM
 *  Author: NMs
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../BTProtocol/BTProtocol.h"
#include "../Timing//Timing.h"
#include "../Constants.c"
#include "../utile.h"

void initLights(){	
	DDRA |= _BV(PINA4);
	DDRA |= _BV(PINA5);
	PORTA |= _BV(PINA4);
	PORTA |= _BV(PINA5);
}

uint8_t blinkLeftLeds(){
	PORTA ^= _BV(PINA5);
	return NO;
}

uint8_t blinkRightLeds(){
	PORTA ^= _BV(PINA4);
	return NO;
}

void doBlinkLeds(uint32_t perioadaStanga, uint32_t perioadaDreapta){
	if(perioadaStanga == 0){
		removeEntryFromTimerQueue(&blinkLeftLeds);
		PORTA &=~ _BV(PINA4);
	}
	else{
		addEntryIfNotExists(&blinkLeftLeds, perioadaStanga, Periodic);
	}
	if(perioadaDreapta == 0){
		removeEntryFromTimerQueue(&blinkRightLeds);
		PORTA &=~ _BV(PINA5);
	}
	else{
		addEntryIfNotExists(&blinkRightLeds, perioadaDreapta, Periodic);
	}
}

void turnBlinkingOff(){
	removeEntryFromTimerQueue(&blinkLeftLeds);
	removeEntryFromTimerQueue(&blinkRightLeds);
	PORTA &=~ _BV(PINA5);
	PORTA &=~ _BV(PINA4);		
}

/*

uint8_t blinkLeds(){
	PORTA ^= _BV(PINA4);
	//PORTA ^= _BV(PINA5);
	return NO;
}

*/