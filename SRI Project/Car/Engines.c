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
#include "../Settings.h"
#include "Encoders.h"

void setEnginesSpeed(Engines, Sens, uint8_t);
void countSeconds();

void goFront(uint8_t timp, uint8_t viteza){	
	setEnginesSpeed(RightEngines, SensFata, viteza);
	setEnginesSpeed(LeftEngines, SensFata, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
	if(DEBUGGING)
		BTTransmitStr("Ma duc inainte!");
}
void goBack(uint8_t timp, uint8_t viteza){
	setEnginesSpeed(RightEngines, SensSpate, viteza);
	setEnginesSpeed(LeftEngines, SensSpate, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    if(DEBUGGING)
		BTTransmitStr("Ma duc inapoi!");
}

void goFrontLeft(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensFata, viteza);
	setEnginesSpeed(LeftEngines, SensFata, 10);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    if(DEBUGGING)
		BTTransmitStr("Ma duc inainte stanga!");
}
void goFrontRight(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensFata, 10);
	setEnginesSpeed(LeftEngines, SensFata, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    if(DEBUGGING)
		BTTransmitStr("Ma duc inainte dreapta!");
}

void goBackLeft(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensSpate, viteza);
	setEnginesSpeed(LeftEngines, SensSpate, 10);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    if(DEBUGGING)
		BTTransmitStr("Ma duc inapoi stanga!");
}
void goBackRight(uint8_t timp, uint8_t viteza){
	
	setEnginesSpeed(RightEngines, SensSpate, 10);
	setEnginesSpeed(LeftEngines, SensSpate, viteza);
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
    if(DEBUGGING)
		BTTransmitStr("Ma duc inapoi dreapta!");
}
void rotirePeLoc(uint8_t timp, uint8_t viteza,  uint8_t engines){
	
	if(engines == RightEngines){
		setEnginesSpeed(LeftEngines, SensSpate,  viteza);
		setEnginesSpeed(RightEngines, SensFata, viteza);
		if(DEBUGGING)
			BTTransmitStr("ma rotesc spre dreapta");
	}
	else {
		setEnginesSpeed(LeftEngines, SensFata, viteza);
		setEnginesSpeed(RightEngines, SensSpate, viteza);
		if(DEBUGGING)
			BTTransmitStr("ma rotesc spre stanga");
	}
	removeEntryFromTimerQueue(&stopEngines);
	addEntryToTimerQueue(&stopEngines, 1000UL*1000UL*timp, Once);
}


void stopEngines(){
	OCR0A = 0;
	OCR2A = 0;
	
	PORTD &= ~ 1<<PIND2;
	PORTD &= ~ 1<<PIND4;
	PORTD &= ~ 1<<PIND3;
	PORTD &= ~ 1<<PIND5;
	
	removeEntryFromTimerQueue(&countSecondsForEncoders);
	
	//if(DEBUGGING)	
		//BTTransmitStr("M-am oprit!");
	
	//setEnginesSpeed(RightEngines, 1, 0);
	//setEnginesSpeed(LeftEngines, 1, 0);
}
void checkFreeParallelParkingPlace(){
    if(DEBUGGING)
		BTTransmitStr("Start Free P P P!");
}
void completeEnclosedContour(){
    if(DEBUGGING)
		BTTransmitStr("Start complete enclosed contour!");
}



void initEngines()
{
	
	OCR0A = 0;
	OCR2A = 0;
	
	//PINB|=(1<<PB3);
	
	
	DDRB |=1<<PINB3;
	TCCR0A |=3;
	TCCR0A |= (1 << COM0A0);
	TCCR0A |= (1 << COM0A1);
	TCCR0B |= (1 << CS00);
	// motor 2
	
	DDRD |= (1 << PD7);
	TCCR2A|=3;
	TCCR2A |= (1 << COM2A0);
	TCCR2A |= (1 << COM2A1);
	TCCR2B |= (1 << CS20);
	//sens
	//DDRC |= 1<<PINC0;
	//DDRC |= 1<<PINC1;
	
	
	//PORTC &=~ 1<<PINC1;
	//PORTC &=~ 1<<PINC0;	
	
		/*	PORTC |= 1<<PINC1;	
			PORTC |= 1<<PINC0;	*/
		
	//enable standby
	DDRB |= 1<<PINB5;
	PORTB |= 1<<PINB5;	
	DDRB |= 1<<PINB4;
	PORTB |= 1<<PINB4;
	
	
	//in1 & in2 4 left engines
	DDRD |= 1<<PIND3;
	DDRD |= 1<<PIND5;
	PORTD &= ~ 1<<PIND3;
	PORTD &= ~ 1<<PIND5;
	
	//in1 & in2 4 left engines
	DDRD |= 1<<PIND2;
	DDRD |= 1<<PIND4;
	PORTD &= ~ 1<<PIND2;
	PORTD &= ~ 1<<PIND4;
	
}


void setEnginesSpeed(Engines engine, Sens sens, uint8_t viteza)
{
	//PD2 (sens driver dreapta)
	//PD3 (sens driver stanga)
	
	//if(sens == SensFata)
	addEntryIfNotExists(&countSecondsForEncoders, 1000UL*1000UL, Periodic);
	viteza = 255 - viteza;
		
	if(engine==RightEngines){
		if(sens != SensFata){
			PORTD &= ~ (1<<PIND2);
			PORTD |=  1<<PIND4;
		}			
		else{
			PORTD |=  1<<PIND2;
			PORTD &= ~( 1<<PIND4);
		}
		OCR2A = viteza;	
	}
	else
	{
		if(sens != SensFata){	
			PORTD |=  1<<PIND3;
			PORTD &= ~ (1<<PIND5);
		}			
		else{			
			PORTD &= ~ (1<<PIND3);
			PORTD |=  1<<PIND5;		
		}
		OCR0A = viteza;
	}		
}

