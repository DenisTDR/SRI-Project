
/*
 * utile.c
 *
 * Created: 5/5/2015 11:15:56 AM
 *  Author: NMs
 */ 
#include <stdio.h>
#include "utile.h"
#include "avr/io.h"
#include "Timing/Timing.h"
#include "BTProtocol/BTProtocol.h"
#include "Constants.c"
#include "Car/Sensors.h"
#include "Car/Engines.h"
#include "PID/PID1.h"

volatile uint8_t e_stins = 0;
void blinkLedD6_v1(){
	if(!e_stins)
		PORTD ^= 1<<PIND6;
}
void blinkLedD6_v2(){
	e_stins = !e_stins;
}

extern volatile uint8_t debugging;

void ledAction(char act){
	//char msg[] = "led action:  ";
	//msg[strlen(msg)-1]= act+'0';
	//BTTransmitStr(msg);
	
	switch(act){
		case 0:
			removeEntryFromTimerQueue(&blinkLedD6_v1);
			PORTD &=~ (1<<PIND6);
			//shouldBlink = 0;
			BTTransmitStr("ledul a fost stins.");
			break;
		case 1:
			removeEntryFromTimerQueue(&blinkLedD6_v1);
			PORTD |= 1<<PIND6;
			//shouldBlink = 0;
			BTTransmitStr("ledul a fost aprins.");
			break;
		
		case 2:
			addEntryToTimerQueue(&blinkLedD6_v1, (500UL * 1000UL), Periodic);
			BTTransmitStr("ledul va 'blincari'.");
			break;
	}
}

void initLeds(){	
	DDRD |=1<<PIND6;
	DDRD |=1<<PIND5;
}
extern uint32_t time;

void ReadSensor0(){
	char msg[70];
	resetSensorQueue(0);
	uint16_t x = getValueOfSensor(0);
	sprintf(msg, "sensor #0: %d", x);	
	BTTransmitStr(msg);
}
void ReadSensor1(){
	char msg[70];
	resetSensorQueue(1);
	uint16_t x = getValueOfSensor(1);
	sprintf(msg, "sensor #1: %d", x);
	BTTransmitStr(msg);
}
void sendTimeAsString(){
	char msg[30];
	sprintf(msg, "t: %lu", time);
	BTTransmitStr(msg);
}
uint8_t started = 0, stopped = 0, reload = 0;
volatile uint32_t lastValues[4];
void fctSmechera(){
	if(reload){
		reload = 0;
		started = stopped = 0;
		return;
	}
	if(!started){
		started = 1;
		lastValues[0] = lastValues[1] = lastValues[2] = lastValues[3] = 0;
		goFront(60, 175);
		BTTransmitStr("started!");
		return;
	}
	if(!stopped){
		lastValues[0]=lastValues[1];
		lastValues[1]=lastValues[2];
		lastValues[2]=lastValues[3];
		lastValues[3] = getValueOfSensor(0);
		
		if( (lastValues[0] + lastValues[1] + lastValues[2] + lastValues[3])/4 > 300){
			stopEngines();
			removeEntryFromTimerQueue(&stopEngines);
			removeEntryFromTimerQueue(&fctSmechera); 
			ReadSensor0();
			stopped = 1;
			reload = 1;
		}
	}
}
volatile uint8_t state=-1;
void doTimer(){
	uint16_t sensorValue = getValueOfSensor(1);
	float diff = PID1cal(430, sensorValue);
	
	removeEntryFromTimerQueue(&stopEngines);
	if(diff > 2 && diff < 2){
		if(state!=0)
			goFront(10, 175),
			state = 0;
			
	}
	else if(diff>2){
		if(state!=1)
			goFrontLeft(10, 175),
			state = 1;
	}
	else{
		if(state!=2)
			goFrontRight(10, 175),
			state = 2;
	}
		
	
}