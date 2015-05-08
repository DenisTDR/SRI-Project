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

uint16_t getCurrentValueOfSensor(Sensor sensor){
	
	startADCConversionForSensor(sensor);
	
	while ( ADCSRA & (1 << ADSC));
	
	uint16_t val;
	val = ADCL >> 6;
	val |= ADCH << 2;
	
	return val;
}

uint16_t sVals[4][7];

uint16_t getValueOfSensor(Sensor sensor){	
	
	resetSensorQueue(sensor);
	sVals[sensor][0] = 0;
	uint8_t i;
	uint16_t s;
	
	while(sVals[sensor][0]==0){
		startADCConversionForSensor(sensor);
		for(i=0;i<6;i++)
			sVals[sensor][i] = sVals[sensor][i+1];
		
		while ( ADCSRA & (1 << ADSC));
		sVals[sensor][6] = ADCL >> 6;
		sVals[sensor][6] |= ADCH << 2;
		
	}
	
	

	s=0;
	for(i=0;i<7;i++)
		s+=sVals[sensor][i];
		
	//sprintf(msg,"lv: %d %d %d %d %d %d %d  =%d",
	//sVals[sensor][0], sVals[sensor][1], sVals[sensor][2], sVals[sensor][3], sVals[sensor][4], sVals[sensor][5], sVals[sensor][6], s/7);
	//BTTransmitStr(msg);
		
	return s/7;
}

void resetSensorQueue(Sensor sensor){
	sVals[sensor][0] = 0;
	uint8_t i;
	for(i=0;i<6;i++)
		sVals[sensor][i] = 0;
}

void initSensors(void){
	
	uint8_t i, j;
	for(i=0; i<4;i++)
		for(j=0; j<7;j++)
			sVals[i][j] = 0;
	
	//init ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

	ADMUX |= (1 << REFS0);
	ADMUX &=~ (1 << REFS1); // Set ADC reference to AVCC
	
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading
	
	//ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
	
	ADCSRA |= (1 << ADEN);  // Enable ADC
	//ADCSRA |= (1 << ADSC);
}

void startADCConversionForSensor(Sensor sensor){

	ADMUX &= ~31; // set lasts 5 bits to 00000
	//ADMUX |= 0; // 0-ADC0, 1-ADC1, 2-ADC2 ...    datasheet pg 249-251
	ADMUX |= sensor; // Sensor ia valorile 0,1,2,3, exact ca cele pt ADC-uri
	// No MUX values needed to be changed to use ADC0

	
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions
	//_delay_ms(1);
}

