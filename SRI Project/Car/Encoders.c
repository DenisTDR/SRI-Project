/*
 * Encoders.c
 *
 * Created: 6/1/2015 1:54:21 AM
 *  Author: NMs
 */
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../BTProtocol/BTProtocol.h"

volatile uint32_t encoder1CNT, encoder2CNT;
volatile uint8_t lastPB0, lastPB7;
volatile uint32_t secondsPassed = 0;
ISR(PCINT1_vect)
{
	if( (PINB & _BV(PB0)) ^ lastPB0){
		lastPB0 = (PINB & _BV(PB0));
		encoder1CNT++;
	}
	if( (PINB & _BV(PB7)) ^ lastPB7){
		lastPB7 = (PINB & _BV(PB7));
		encoder2CNT++;
	}
}


void initEncoders(){		
	PCMSK1 |= _BV(PCINT8);
	DDRB &= ~_BV(PINB0);	
	PCMSK1 |= _BV(PCINT15);
	DDRB &= ~_BV(PINB7);	
	PCICR |= _BV(PCIE1);
}

void getAverageSpeed(uint8_t reset){
	char strBuffer[100];
	sprintf(strBuffer, "\ne1C: %lu\ne2C: %lu ", encoder1CNT, encoder2CNT);
	BTTransmitStr(strBuffer);
	if(reset){
		encoder2CNT = encoder1CNT = 0;
		secondsPassed = 0;
	}
}
void countSecondsForEncoders(){
	secondsPassed ++;
}
void resetEncoders(){
	encoder2CNT = encoder1CNT = 0;
	lastPB7 = lastPB0 = 0;
}