/*
 * Engines.c
 *
 * Created: 5/5/2015 1:06:35 AM
 *  Author: NMs
 */
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "avr/interrupt.h"
#include "Engines.h"
#include "../Constants.c"
#include "../BTProtocol/BTProtocol.h"
#include "../Timing/Timing.h"

void setEnginesSpeed(Engines, Sens, uint8_t);

void goFront(uint8_t timp, uint8_t viteza){	
	setEnginesSpeed(RightEngines, SensFata, viteza);
	setEnginesSpeed(LeftEngines, SensFata, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);	
    BTTransmitStr("Ma duc inainte!");
}
void goBack(uint8_t timp, uint8_t viteza){
	setEnginesSpeed(RightEngines, SensSpate, viteza);
	setEnginesSpeed(LeftEngines, SensSpate, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    BTTransmitStr("Ma duc inapoi!");
}

void goFrontLeft(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensFata, viteza);
	setEnginesSpeed(LeftEngines, SensFata, 10);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    BTTransmitStr("Ma duc inainte stanga!");
}
void goFrontRight(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensFata, 10);
	setEnginesSpeed(LeftEngines, SensFata, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    BTTransmitStr("Ma duc inainte dreapta!");
}

void goBackLeft(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensSpate, viteza);
	setEnginesSpeed(LeftEngines, SensSpate, 10);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    BTTransmitStr("Ma duc inapoi stanga!");
}
void goBackRight(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensSpate, 10);
	setEnginesSpeed(LeftEngines, SensSpate, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    BTTransmitStr("Ma duc inapoi dreapta!");
}
void rotirePeLoc(uint8_t timp, uint8_t viteza,  uint8_t engines){
	
	if(engines == RightEngines){
		setEnginesSpeed(LeftEngines, SensFata, viteza);
		setEnginesSpeed(RightEngines, SensSpate, viteza);
		BTTransmitStr("ma rotesc spre dreapta");
	}
	else {
		setEnginesSpeed(LeftEngines, SensSpate,  viteza);
		setEnginesSpeed(RightEngines, SensFata, viteza);
		BTTransmitStr("ma rotesc spre stanga");
	}
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
}

extern volatile uint32_t time;
void stopEngines(){
    BTTransmitStr("M-am oprit!");
	
	PORTC &=~ 1<<PINC1;
	PORTC &=~ 1<<PINC0;	
	
	OCR0A = 255;
	OCR2A = 255;
	//setEnginesSpeed(RightEngines, 1, 0);
	//setEnginesSpeed(LeftEngines, 1, 0);
}
void checkFreeParallelParkingPlace(){
	
    BTTransmitStr("Start Free P P P!");
}
void completeEnclosedContour(){
	
	
    BTTransmitStr("Start complete enclosed contour!");
}


void initEngines()
{
	
	OCR0A = 255;
	OCR2A = 255;
	
	PINB|=(1<<PB3);
	TCCR0A |=3;
	TCCR0A |= (1 << COM0A0);
	TCCR0A |= (1 << COM0A1);
	TCCR0B |= (1 << CS00);
	DDRB |=1<<PINB3;
	// motor 2
	DDRD |= (1 << PD7);
	TCCR2A|=3;
	TCCR2A |= (1 << COM2A0);
	TCCR2A |= (1 << COM2A1);
	TCCR2B |= (1 << CS20);
	//sens
	DDRC |= 1<<PINC0;
	DDRC |= 1<<PINC1;
	
	
	PORTC &=~ 1<<PINC1;
	PORTC &=~ 1<<PINC0;	
	
		/*	PORTC |= 1<<PINC1;	
			PORTC |= 1<<PINC0;	*/
}


void setEnginesSpeed(Engines engine, Sens sens, uint8_t viteza)
{
	if(sens == SensFata)
		viteza = 255 - viteza;
		
	if(engine==RightEngines){
		OCR2A = viteza;
		if(sens == SensFata){	
			PORTC &=~ 1<<PINC0;
		}			
		else{
			PORTC |= 1<<PINC0;	
		}			
	}
	else
	{
		OCR0A = viteza;
		if(sens == SensFata){	
			PORTC &=~ 1<<PINC1;
		}			
		else{			
			PORTC |= 1<<PINC1;			
		}			
	}		
}	