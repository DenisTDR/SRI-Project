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

uint8_t blinkNeons();

void initLights(){	
	DDRA |= _BV(PINA4);
	DDRA |= _BV(PINA5);
	PORTA |= _BV(PINA4);
	PORTA |= _BV(PINA5);
	
	
	DDRA |= _BV(PINA6);
	DDRA |= _BV(PINA7);
	PORTA |= _BV(PINA7);
	
	addEntryIfNotExists(&blinkNeons, 250UL*1000UL, Periodic);
}

uint8_t blinkLeftLeds(){
	PORTA ^= _BV(PINA5);
	return NO;
}

uint8_t blinkRightLeds(){
	PORTA ^= _BV(PINA4);
	return NO;
}

uint8_t blinkNeonsVar = 0;
uint8_t blinkNeons(){
	if(blinkNeonsVar % 7 == 0 || blinkNeonsVar % 7 == 2){
		PORTA &= ~_BV(PINA7);
	}
	else if(blinkNeonsVar % 7 == 1 || blinkNeonsVar % 7 == 3){
		PORTA |= _BV(PINA7);
	}
	blinkNeonsVar ++;
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