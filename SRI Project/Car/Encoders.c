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
#include "../Timing//Timing.h"
#include "../Constants.c"
#include "../utile.h"

#define DISTANTA_PARCURSA ( (((encoder1CNT+encoder2CNT)* 21)>>2) )

volatile uint32_t encoder1CNT, encoder2CNT;
volatile uint8_t lastPB0, lastPB7;
volatile uint32_t deciSecondsPassed = 0;
volatile uint8_t encSetting = 2;
volatile uint8_t cnt4Encoders = 0;

void resetEncoders();
uint8_t countTimeForEncoders();
void toggleCountingTimeForEncoders(uint8_t on);

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
	if((encSetting & 3) == 0){
		encSetting = 5;
		cnt4Encoders = 0;
		addEntryIfNotExists(&countTimeForEncoders, 200 * 1000UL, Periodic);
	}	
	encSetting |= 3;
	cnt4Encoders = 0;
} 

void initEncoders(){		
	PCMSK1 |= _BV(PCINT8);
	DDRB &= ~_BV(PINB0);	
	PCMSK1 |= _BV(PCINT15);
	DDRB &= ~_BV(PINB7);	
	PCICR |= _BV(PCIE1);
	resetEncoders();
}
uint8_t sendDistAndTimePeriodically(void);

void getAverageSpeed(uint8_t reset){
	char strBuffer[100];
	sprintf(strBuffer, "\n   dist: %lu \n   timp: %lu",  DISTANTA_PARCURSA, deciSecondsPassed);
	BTTransmitStr(strBuffer);
	
	if(!existsEntryInTimerQueue(&sendDistAndTimePeriodically))
		addEntryToTimerQueue(&sendDistAndTimePeriodically, 1001 * 1000UL, Periodic);
	else
		removeEntryFromTimerQueue(&sendDistAndTimePeriodically);
	if(reset){
		encoder2CNT = encoder1CNT = 0;
		deciSecondsPassed = 0;
	}
}
uint8_t countTimeForEncoders(){
	if(encSetting & 1){
		deciSecondsPassed += 2;
		
		cnt4Encoders++;
		if(cnt4Encoders & 3){
			encSetting ^= 1;
		}
	}
	return 0;
}
void resetEncoders(){
	encoder2CNT = encoder1CNT = 0;
	lastPB7 = lastPB0 = 0;
}
void setSendingDistAndTime(uint8_t on){
	if(on)
		addEntryIfNotExists(&sendDistAndTimePeriodically, 500 * 1000UL, Periodic);
	else
		removeEntryFromTimerQueue(&sendDistAndTimePeriodically);
}

uint8_t sendDistAndTimePeriodically(void){
	if((encSetting & 2) == 0)
		return NO;
	encSetting ^= 2;
	
	uint32_t dist = DISTANTA_PARCURSA;
	uint32_t time = deciSecondsPassed;
	uint8_t buffer[20];
	buffer[0] = StartByte;
	buffer[1] = InfoCarStats;
	buffer[2] = 8;
	
	buffer[3] = dist >> 24;
	buffer[4] = (uint8_t)(dist >> 16);
	buffer[5] = (uint8_t)(dist >> 8);
	buffer[6] = (uint8_t)(dist);
	
	buffer[7] = time >> 24;
	buffer[8] = (uint8_t)(time >> 16);
	buffer[9] = (uint8_t)(time >> 8);
	buffer[10] = (uint8_t)(time);
	
	buffer[11] = EndByte;
	BTTransmitMsgU(buffer, (uint8_t)12);
	return NO;
}