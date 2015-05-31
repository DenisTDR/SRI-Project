/*
 * CFile1.c
 *
 * Created: 5/5/2015 12:51:35 AM
 *  Author: NMs
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz
#endif

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "BTProtocol.h"
#include "../Constants.c"
#include "../Car/Engines.h"
#include "../utile.h"
#include "../Timing/Timing.h"
#include "../Car/Sensors.h"
#include "../functiiParcare.h"
#include "../Settings.h"

void prelucreazaDatele(void);
void BTProtocolReadByte(unsigned char theByte);

ISR(USART0_RX_vect)
{
	BTProtocolReadByte(UDR0);
}


void reTransmit(void){
	//trimite un semnal telefonului pentru a retransmite ultimul mesaj
	printf("Trimite din nou ultimul mesaj!!!");
	char msg[] = { StartByte, ReTransmitLastMsg, 0, 0x55 };
	BTTransmitMsg(msg, 4);
}


BTState state;
CarAction actiune;
unsigned char len;
unsigned char date[10];
unsigned char dateCrtIndex;

void resetBTProtocol(){
	state = WaitingStartByte;
}

extern volatile uint8_t iesire;
void BTProtocolReadByte(unsigned char theByte){
	cli();
	switch (state){
		case WaitingStartByte:
			if(theByte == StartByte){
				state = WaitingCarAction;
				//addEntryToTimerQueue(&resetBTProtocol, 1000UL * 1000UL * 1000UL, Once);
			}
			break;

		case WaitingCarAction:
			if(theByte >= NoAction && theByte < EndAction){
				state = WaitingDataLength;
				actiune = theByte;
			}
			else{
				state = WaitingStartByte;
				reTransmit(); // error ocurred, send retransmit signal
			}
			break;

		case WaitingDataLength:
			if(theByte==0){
				state = WaitingEndByte;
			}
			state = ReadingData;
			len = theByte;
			dateCrtIndex = 0;
			if(len<=0 || len>50)
				state=WaitingStartByte;
			break;

		case ReadingData:
			date[dateCrtIndex++] = theByte;
			if(dateCrtIndex >= len)
			state = WaitingEndByte;
			break;
		case WaitingEndByte:
			if(theByte != 0x55){
				reTransmit(); // error ocurred, send retransmit signal
			}
			else{
				prelucreazaDatele();
			}
			state = WaitingStartByte;
			break;

	}
	sei();
	//printf("\nstare noua %d: ", state);
}
extern volatile uint8_t ms2p1_enabled;

void prelucreazaDatele(void){
	
	//BTTransmitStr("Procesez ceva!");
	//cli();
	if(actiune >= GoFront && actiune <= GoRightB){
		unsigned char timp = date[0];
		unsigned char viteza = date[1];
		if(dateCrtIndex == 3)
			toggleDebuggingOff();
		switch(actiune){
			case GoFront:
			goFront(timp, viteza);
			break;
			case GoBack:
			goBack(timp, viteza);
			break;
			case GoLeftF:
			goFrontLeft(timp, viteza);
			break;
			case GoLeftB:
			goBackLeft(timp, viteza);
			break;
			case GoRightF:
				goFrontRight(timp, viteza);
			break;
			case GoRightB:
				goBackRight(timp, viteza);
			break;
			default:
				break;
		}
		if(dateCrtIndex == 3)
			toggleDebuggingOff();
			
		return;
	}
	
	switch(actiune){
		case GoM2P2:
			completeEnclosedContour();
		break;
		case GoM2P3:
			checkFreeParallelParkingPlace();
		break;
		case Led:
			ledAction(date[0]);
		break;
		case ReadSensorValue:
			toggleSensorSend(date[0]);		
		break;
		case StopEngines:
			stopEngines();
		break;
		case RotirePeLocDreapta:
			rotirePeLoc(date[0], date[1], RightEngines);
		break;		
		case RotirePeLocStanga:
			rotirePeLoc(date[0], date[1], LeftEngines);
		break;
		case DisplayMessage:
			//addEntryToTimerQueue(&fctSmechera, 10UL * 1000UL, Periodic);
			//addEntryToTimerQueue(&testFct1, 500UL * 1000UL, Periodic);
			addEntryToTimerQueue(&sendEncoderCounter, 500UL * 1000UL, Once);
		break;
		case GoM2P1:
			addEntryToTimerQueue(&functieRotireStanga, 1000UL * 500UL, Periodic);
			iesire=date[0];
		break;
		case ResetThings:
			stopEngines();
			resetTimerQueue();
		break;
		case GetAverageSpeed:
			getAverageSpeed(date[0]);
		break;
		default:
		break;
	}
	//alta actiune...
	//sei();
}

void BTTransmitStr(char *theString){
	uint8_t len, i;
	
	len = strlen(theString);
	
	BTTransmitChar(0xAA);
	BTTransmitChar(DisplayMessage);
	BTTransmitChar(len);
	
	for(i=0;i<len;i++)
	{
		BTTransmitChar(theString[i]);
	}
	BTTransmitChar(0x55);
	
}

void BTTransmitMsg(char *theString, uint8_t len){
	uint16_t i;
	for(i=0;i<len;i++)
	{
		BTTransmitChar(theString[i]);
	}
}

void BTTransmitChar(unsigned char theChar){
	//while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = theChar;
	_delay_ms(10);
}

void initBTProtocol()
{
	/*Pentru Receive*/
	UBRR0L = 25; //baud rate 9600 bps
	UCSR0B |= _BV(RXCIE0);
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
		
	state = WaitingStartByte;
}