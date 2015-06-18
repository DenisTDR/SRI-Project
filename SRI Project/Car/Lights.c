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
#include "../Settings.h"

uint8_t blinkNeons();

void initLights(){	
	DDRA |= _BV(PINA4);
	DDRA |= _BV(PINA5);
	//PORTA |= _BV(PINA4);
	//PORTA |= _BV(PINA5);
	
	
	DDRA |= _BV(PINA6);
	DDRA |= _BV(PINA7);
	
}

uint8_t blinkLeftLeds(){
	PORTA ^= _BV(PINA5);
	return NO;
}

uint8_t blinkRightLeds(){
	PORTA ^= _BV(PINA4);
	return NO;
}

uint8_t blinkNeonsState = 0;

uint8_t blinkNeonsSMF(){
	switch(NEONS_SETTING){
		case 1:
			if(blinkNeonsState == 0)
				PORTA &= ~_BV(PINA7);
			else if(blinkNeonsState > 6){
				PORTA ^= _BV(PINA7);
				if(blinkNeonsState == 12)
				blinkNeonsState = 0;
			}
		break;
		case 2:
			if(blinkNeonsState % 7 == 0 || blinkNeonsState % 7 == 2){
				PORTA &= ~_BV(PINA7);
			}
			else if(blinkNeonsState % 7 == 1 || blinkNeonsState % 7 == 3){
				PORTA |= _BV(PINA7);
			}
		break;
		case 3:
		
		break;
		case 4:
		
		break;
		default:
		break;
	}
	blinkNeonsState ++;
	return NO;
}
void setTheNeonSetting(uint8_t val){
	blinkNeonsState=0;
	PORTA &= ~_BV(PINA7);
	if(val)
		addEntryIfNotExists(&blinkNeonsSMF, 250 * 1000UL, Periodic);
	else
		removeEntryFromTimerQueue(&blinkNeonsSMF);
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