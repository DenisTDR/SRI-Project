/*
 * Sensors.c
 *
 * Created: 5/5/2015 1:15:21 AM
 *  Author: NMs
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "avr/interrupt.h"

#include "Sensors.h"
#include "../Constants.c"
#include "../BTProtocol/BTProtocol.h"
#include "../Timing/Timing.h"


uint32_t getValueOfSensor3(Sensor sensor);
uint16_t getCurrentValueOfSensor(Sensor sensor){	
	startADCConversionForSensor(sensor);	
	while ( ADCSRA & (1 << ADSC));	
	uint16_t val;
	val = ADCL >> 6;
	val |= ADCH << 2;	
	return val;
}

uint16_t getValueOfSensor(Sensor sensor){	
	
	uint16_t sVals[7];
	uint8_t i;
	uint16_t s;
	
	for(i=0;i<7;i++){
		startADCConversionForSensor(sensor);
		
		while ( ADCSRA & (1 << ADSC));
		sVals[i] = ADCL >> 6;
		sVals[i] |= ADCH << 2;		
	}
	
	s=0;
	for(i=0;i<7;i++)
		s+=sVals[i];
		
	return s/7;
}

volatile uint8_t sensorsToRead, sensorsToSend;
char msgBuffer[50];
volatile uint16_t oldValue[4];
volatile uint32_t lastCMValue[4];

void sendSensor(){
	uint8_t i;
	
	for(i=0;i<4;i++){		
		if(sensorsToSend & (1<<i)){			
			sprintf(msgBuffer, "sensor #%d = %lu", i, lastCMValue[i]);			
			BTTransmitStr(msgBuffer);			
		}
	}
}
void readSensors();
void toggleSensorSend(Sensor sensor){
	sensorsToSend ^= 1<<sensor;
	
	sprintf(msgBuffer, "sensorsToSend %d %d %d %d ", sensorsToSend&1, (sensorsToSend&2)>>1, (sensorsToSend&4)>>2, (sensorsToSend&8)>>3);
	BTTransmitStr(msgBuffer);
	sprintf(msgBuffer, "sensorsToRead %d %d %d %d <<--", sensorsToRead&1, (sensorsToRead&2)>>1, (sensorsToRead&4)>>2, (sensorsToRead&8)>>3);
	BTTransmitStr(msgBuffer);
	
	uint8_t existsInTimerQueue = existsEntryInTimerQueue(&sendSensor);
	//toggleSensorRead(sensor);
	if(sensorsToSend && !existsInTimerQueue){
		addEntryToTimerQueue(&sendSensor, 1000UL * 1000UL, Periodic);
	}
	else{
		if(!sensorsToSend && existsInTimerQueue)
		removeEntryFromTimerQueue(&sendSensor);
	}
}

void readSensors(){
	if(sensorsToRead & 1)
		lastCMValue[0] = getValueOfSensor3(0);
	if(sensorsToRead & 2)
		lastCMValue[1] = getValueOfSensor3(1);
	if(sensorsToRead & 4)
		lastCMValue[2] = getValueOfSensor3(2);
	if(sensorsToRead & 8)
		lastCMValue[3] = getValueOfSensor3(3);
}
void toggleSensorRead(Sensor sensor){
	sensorsToRead ^= 1<<sensor;
	
	uint8_t existsInTimerQueue = existsEntryInTimerQueue(&readSensors);
	
	if(sensorsToRead && !existsInTimerQueue){
		addEntryToTimerQueue(&readSensors, 100UL * 1000UL, Periodic);		
	}
	else{
		if(!sensorsToRead && existsInTimerQueue){
			removeEntryFromTimerQueue(&readSensors);
		}
	}
}

uint32_t getValueOfSensor3(Sensor sensor){
	uint8_t i;
	uint16_t s, sVals[7];
	
	//citesc 7 valori ale senzorului
	for(i=0;i<7;i++)
		sVals[i] = getCurrentValueOfSensor(sensor);
		
	do{ // le sortez (bubble sort)
		s=1;
		for(i=0;i<6;i++)
			if(sVals[i]>sVals[i+1]){
				s = sVals[i];
				sVals[i] = sVals[i+1];
				sVals[i+1] = s;
				s=0;
			}
	}while(!s);
	
	s = sVals[2] + sVals[3] + sVals[4];
	s/=3;//calculez media celor 3 din mijloc (eliminandu-le astfel pe cele mai mari si pe cele mai mici)
	
	//valoarea noua este media ponderata a vechii valori (70%) si valorii curente(30%)
	if(!oldValue[sensor])
		oldValue[sensor] = s;
	else
		oldValue[sensor] = (7*oldValue[sensor] + 3*s) / 10;

	//f(x)=769533/((x<<4)+-520)// front
	//f(x)=24886/((x<<4)+-760) // side
	
	if(sensor<2)
		return (322641UL/((oldValue[sensor]<<4)-315));// senzori laterali
	else
		return (769533UL/((oldValue[sensor]<<4)-520));	// senzori frontali
}

typedef enum{
	Paralel,
	Apropiat,
	PreaApropiat,
	Departat,
	PreaDepartat	
}ParallelResult;
        
uint8_t isParalel(uint16_t sideValue, uint16_t frontValue, uint16_t sensorOffset, uint16_t epsilon, uint16_t warningEpsilon)
{
	uint16_t sideCalculated = (frontValue - (sensorOffset * (1<<9) / 362)) * 362 / (1<<9);
			
	if(sideCalculated > sideValue)
		if(sideCalculated - sideValue > warningEpsilon)
			return PreaDepartat;
		else if(sideCalculated - sideValue > epsilon)
			return Departat;
		else 
			return Paralel;
	else
		if(sideValue - sideCalculated > warningEpsilon)
			return PreaApropiat;
		else if(sideValue - sideCalculated > epsilon)
			return Apropiat;
		else return Paralel;
}

void initSensors(void){
		
	//init ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

	ADMUX |= (1 << REFS0);
	ADMUX &=~ (1 << REFS1); // Set ADC reference to AVCC
	
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading
	
	//ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
	
	ADCSRA |= (1 << ADEN);  // Enable ADC
	//ADCSRA |= (1 << ADSC);  // start adc
	sensorsToRead = sensorsToSend = 0;
	toggleSensorRead(0);
	toggleSensorRead(1);
	toggleSensorRead(2);
	toggleSensorRead(3);
}

void startADCConversionForSensor(Sensor sensor){

	ADMUX &= ~31; // set lasts 5 bits to 00000
	//ADMUX |= 0; // 0-ADC0, 1-ADC1, 2-ADC2 ...    datasheet pg 249-251
	ADMUX |= sensor; // Sensor ia valorile 0,1,2,3, exact ca cele pt ADC-uri
	// No MUX values needed to be changed to use ADC0

	
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions
	//_delay_ms(1);
}

