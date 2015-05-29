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
		
	//sprintf(msg,"lv: %d %d %d %d %d %d %d  =%d",
	//sVals[sensor][0], sVals[sensor][1], sVals[sensor][2], sVals[sensor][3], sVals[sensor][4], sVals[sensor][5], sVals[sensor][6], s/7);
	//BTTransmitStr(msg);
		
	return s/7;
}

volatile uint8_t sensorsToRead=0;
volatile Sensor sensorToRead;
volatile uint16_t valueOfSensor;
char msgBuffer[50];
void readSensorsAndSend(){
	uint8_t i;
	
	for(i=0;i<4;i++){		
		if(sensorsToRead & (1<<i)){
			sensorToRead = (Sensor)i;
			valueOfSensor = getValueOfSensor3(sensorToRead);
			sprintf(msgBuffer, "sensor #%d = %hu", sensorToRead, valueOfSensor);			
			BTTransmitStr(msgBuffer);			
		}
	}
}
void toggleSensorRead(Sensor sensor){
	sensorsToRead ^= 1<<sensor;
	sprintf(msgBuffer, "sensorsToRead #%d ", sensorsToRead);
	BTTransmitStr(msgBuffer);
	
	uint8_t existsInTimerQueue = existsEntryInTimerQueue(&readSensorsAndSend);
	
	if(sensorsToRead && !existsInTimerQueue){
		addEntryToTimerQueue(&readSensorsAndSend, 250UL * 1000UL, Periodic);
	}
	else{
		if(!sensorsToRead && existsInTimerQueue)
			removeEntryFromTimerQueue(&readSensorsAndSend);
	}
}
volatile uint16_t oldValue[4];
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
	oldValue[sensor]=s;
	return s;
	/*if(!oldValue[sensor])
		oldValue[sensor] = s;
	else
		oldValue[sensor] = (7*oldValue[sensor] + 3*s) / 10;
*/	
	//f(x)=77038/((x<<4)+-516) // front
	//f(x)=24886/((x<<4)+-760) // side
	
	if(sensor<2)
		return 77038UL/((oldValue[sensor]<<4)-516); // senzori frontali
	else
		return 24886UL/((oldValue[sensor]<<4)-760);	// senzori laterali
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


/*
uint32_t valPePortiuniLateral(uint32_t mv);
uint32_t valPePortiuniFata(uint32_t mv);
uint32_t getValueOfSensor2(Sensor sensor, uint8_t debug){
	
	resetSensorQueue(sensor);
	sVals[sensor][0] = 0;
	uint8_t i, j;
	uint32_t tmp;
	
	while(sVals[sensor][0]==0){
		startADCConversionForSensor(sensor);
		for(i=0;i<6;i++)
		sVals[sensor][i] = sVals[sensor][i+1];
		
		while ( ADCSRA & (1 << ADSC));
		sVals[sensor][6] = ADCL >> 6;
		sVals[sensor][6] |= ADCH << 2;
		
	}
	//sort
	
	for(i=0;i<7-1;i++)		
		for(j=i+1;j<7;j++)
			if(sVals[sensor][i] > sVals[sensor][j]){
				tmp=sVals[sensor][i];
				sVals[sensor][i]=sVals[sensor][j];
				sVals[sensor][j]=tmp;
			}
	tmp = 0;
	for(i=2; i<5; i++)
		tmp += sVals[sensor][i];

	
	tmp /= 3;
		
	if(debug){
		//uint32_t vmv = vr*5000UL/1024;
		char msg[70];
		
		//uint32_t real = valPePortiuni(vmv) / 100;
		//sprintf(msg, "sensorbf #%d: %lu", sensor, tmp);
		BTTransmitStr(msg);
	}
	tmp = tmp * 5000 / 1024;
	
	if(sensor >= 2)
		tmp = valPePortiuniLateral(tmp) / 100;
	else
		tmp = valPePortiuniFata(tmp) / 100;
	
	return tmp;
}
*/

void sendValueOfSensor(Sensor sensor){
	uint32_t real = getValueOfSensor2(sensor, 1);
	//uint32_t vmv = vr*5000UL/1024;	
	char msg[70];
	
	//uint32_t real = valPePortiuni(vmv) / 100;
	sprintf(msg, "sensorn #%d: %lu", sensor, real);
	BTTransmitStr(msg);
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
}

void startADCConversionForSensor(Sensor sensor){

	ADMUX &= ~31; // set lasts 5 bits to 00000
	//ADMUX |= 0; // 0-ADC0, 1-ADC1, 2-ADC2 ...    datasheet pg 249-251
	ADMUX |= sensor; // Sensor ia valorile 0,1,2,3, exact ca cele pt ADC-uri
	// No MUX values needed to be changed to use ADC0

	
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions
	//_delay_ms(1);
}

