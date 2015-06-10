
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
#include "Car/Encoders.h"
#include "Settings.h"

volatile uint8_t e_stins = 0;
uint8_t blinkA1(){
	//if(!e_stins)
	//	PORTD ^=( 1<<PIND2);
	PORTA ^= _BV(PINA5);
	return NO;
}
void blinkLedD6_v2(){
	e_stins = !e_stins;
}

extern volatile uint8_t debugging;

void ledAction(uint8_t a1, uint8_t a2){
	//char msg[] = "led action:  ";
	//msg[strlen(msg)-1]= act+'0';
	//BTTransmitStr(msg);
	
	a1 = (a1!=0);
	a2 = (a2!=0);
	
	if(a1)
		PORTA |= _BV(PINA5);
	else
		PORTA &= ~_BV(PINA5);
	if(a2)
		PORTA |= _BV(PINA4);
	else
		PORTA &= ~_BV(PINA4);
		
	
	return;
	switch(a1){
		case 0:
			removeEntryFromTimerQueue(&blinkA1);
			//PORTD &=~ (1<<PIND2);
			//shouldBlink = 0;
			BTTransmitStr("ledul a fost stins.");
			break;
		case 1:
			removeEntryFromTimerQueue(&blinkA1);
			//PORTD |= 1<<PIND2;
			//shouldBlink = 0;
			BTTransmitStr("ledul a fost aprins.");
			break;
		
		case 2:
			e_stins = 0;
			addEntryToTimerQueue(&blinkA1, (1000UL * 1000UL), Periodic);
			BTTransmitStr("ledul va 'blincari'.");
			break;
	}
}

extern uint32_t time;

uint8_t stateMachineTest1();
uint8_t stareSMT1 = 0;
void initAndStartStateMachineTest1(){
	stareSMT1 = 0;
	addEntryToTimerQueue(&stateMachineTest1, 100UL * 1000UL, Periodic);
}
uint8_t stateMachineTest1(){
	uint32_t fl, fr;//, sl, sr;
	fl = getValueOfSensor(FrontLeftSensor);
	fr = getValueOfSensor(FrontRightSensor);
	//sl = getValueOfSensor(SideLeftSensor);
	//sr = getValueOfSensor(SideRightSensor);
	if(DEBUGGING){
		char bfr[100];
		sprintf(bfr, "fl:%lu  fr:%lu", fl, fr);
		BTTransmitStr(bfr);
	}
	
	switch(stareSMT1){
		case 0:
			stareSMT1 = 1;
			goFront(30, 150);
		break;
		case 1:
			if(fl<130 && fr<130)
				stareSMT1 = 2,
				goBack(1, 150);
		break;
		case 2:
			stopEngines();
			return YES;		
		break;
	}
	return NO;	
}

uint8_t SMFParcurgereDistanta();
uint8_t starePD;
uint32_t distantaDeParcurs;
uint8_t vitezaCuCareParcurg;
void initParcurgereDistanta(uint32_t distanta, uint8_t viteza){
	starePD = 0;
	distantaDeParcurs = distanta + DISTANTA_PARCURSA;
	vitezaCuCareParcurg = viteza;
	addEntryIfNotExists(&SMFParcurgereDistanta, 10*1000UL, Periodic);
}

extern volatile uint32_t deciSecondsPassed;
uint8_t SMFParcurgereDistanta(){
	//char buffer[100];
	//sprintf(buffer, "dp: %lu  time:%lu", DISTANTA_PARCURSA, deciSecondsPassed);
	//BTTransmitStr(buffer);
	
	switch(starePD){
		case 0:
			goFront(100, vitezaCuCareParcurg);
			starePD = 1;
		break;
		case 1:
			if(DISTANTA_PARCURSA > distantaDeParcurs){
				goBack(1, 130);
				starePD = 2;
			}
		break;
		case 2:
			stopEngines();
			char buffer[100];
			sprintf(buffer, "dp: %lu  time:%lu", DISTANTA_PARCURSA, deciSecondsPassed);
			BTTransmitStr(buffer);
			return YES;
		break;
	}	
	return NO;
}



ParallelResult isParalel(uint32_t sideValue, uint32_t frontValue, uint32_t sensorOffset, uint32_t epsilon, uint32_t warningEpsilon)
{
	uint32_t sideCalculated = (frontValue - (sensorOffset * (1<<9) / 362)) * 362 / (1<<9);
	
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
		else 
			return Paralel;
}
uint8_t SMPcheck();
void initParalelCheck(){
	if(existsEntryInTimerQueue(&SMPcheck))
		removeEntryFromTimerQueue(&SMPcheck),
		BTTransmitStr("removed paralel check fct");
	else
		addEntryIfNotExists(&SMPcheck, 250UL * 1000UL, Periodic),
		BTTransmitStr("added paralel check fct");
}

volatile ParallelResult lastParallelResult = Paralel;
uint8_t SMPcheck(){
	//sensor offset 95 mm;	
	uint32_t fr, sr;
	fr = getValueOfSensor(FrontRightSensor);
	//fr = getValueOfSensor(FrontRightSensor);
	sr = getValueOfSensor(SideRightSensor);
	//sr = getValueOfSensor(SideRightSensor);
	
	ParallelResult rez = isParalel(sr, fr, 95, 22, 45);
	
	//char msg[50];
	//sprintf(msg, "is paralel: %d sr:%lu  fr:%lu", rez, sr, fr);
	//BTTransmitStr(msg);
	if(rez == Paralel && lastParallelResult == Paralel)
		return YES;
	if(rez != lastParallelResult){
		switch(rez){
			case PreaApropiat:
				rotirePeLoc(10, 75, RightEngines);
			break;		
			case Apropiat:
				rotirePeLoc(10, 60, RightEngines);
			break;		
			case Paralel:
				stopEngines();
				return YES;
			break;
			case Departat:
				rotirePeLoc(10, 60, LeftEngines);
			break;
			case PreaDepartat:
				rotirePeLoc(10, 75, LeftEngines);
			break;
		}
		lastParallelResult = rez;
	}	
	return NO;
}

uint8_t paralelParkingSMF();
uint8_t statePP, secsCounter;

uint32_t srTmp;
uint8_t lastState;

void initParalelParking(){
	statePP = 33;
	secsCounter = 0;
	//statePP = 7;
	lastState = 0;
	srTmp = 0;
	addEntryIfNotExists(&paralelParkingSMF, 250*1000UL, Periodic);
}

//DISTANTA_PARCURSA
uint32_t distDeParcurs;

uint8_t checkMinim(void);
uint32_t xx;
uint8_t cnt2;
uint8_t paralelParkingSMF(){
	uint32_t fr, sr;
	fr = getValueOfSensor(FrontRightSensor);
	//fr = getValueOfSensor(FrontRightSensor);
	sr = getValueOfSensor(SideRightSensor);
	switch(statePP){
		case 33:
			goFront(30, 160);
			distDeParcurs = DISTANTA_PARCURSA + 200;
			statePP = 34;
		break;
		case 34:
			if(DISTANTA_PARCURSA > distDeParcurs){
				statePP = 0;
				stopEngines();
			}		
		break;
		case 0:
			if(SMPcheck() == 1){
				statePP = 1;
				goFront(30, 60);
			}
		break;
		case 1:
			if(sr<130 && sr>40){
				statePP = 2;
			}
		break;
		case 2:
			if(sr > 300){
				statePP = 100;
				goBack(10, 30);				
				break;
				
				//distDeParcurs= DISTANTA_PARCURSA + 40;
				//statePP = 3;
				//goBack(10, 30);
				//stopEngines();
			}
			
		break;
		case 100:
			if(sr<130 && sr>40){
				//stopEngines();
				goFront(20, 50);
				distDeParcurs = DISTANTA_PARCURSA + 110;
				statePP = 3;
				break;
			}
		break;
		/*case 101:
			if(DISTANTA_PARCURSA > distDeParcurs){
				stopEngines();				
				//return YES;
			}
		break;*/
		case 3:
			if(DISTANTA_PARCURSA > distDeParcurs){
				stopEngines();
				statePP = 4;
				rotireSmechera(5, 0, 170, 1, 50);
			}
		break;
		case 4:
			if(sr<200){
				statePP = 5;
				rotirePeLoc(10, 50, LeftEngines);
			}
		break;
		case 5:
			if(sr>220)
			{
				goFront(15, 50);
				statePP = 6;
			}
		break;
		case 6:
			if(fr < 170){
				statePP = 7;
				lastState = 0;
				srTmp = 0;
			}
		break;
		case 7:
			if(checkMinim()){
				//stopEngines();
				statePP = 8;
				goFront(15, 40);
			}		
		break;
		case 8:
			if(fr<100){
				//stopEngines();
				//return YES;
				distDeParcurs = DISTANTA_PARCURSA + 100;
				statePP = 9;
				goBack(10, 50);				
			}
		break;
		case 9:
			if(DISTANTA_PARCURSA > distDeParcurs){
				stopEngines();
				//return YES;
				statePP = 10;
			}
		
		break;
		case 10:
			if( 1 || SMPcheck()){
				stopEngines();
				distDeParcurs = DISTANTA_PARCURSA + 30;
				statePP = 11;
				goFront(10, 50);
				xx = sr;
				//return YES;
			}
		
		break;
		case 11:
			if(DISTANTA_PARCURSA > distDeParcurs){
				if(xx > sr +5){
					cnt2 = (xx-sr)%10;
					rotirePeLoc(2, 75, RightEngines);
				}
				else if(sr > xx + 5){
					cnt2 = (sr-xx)%10;
					rotirePeLoc(2, 75, LeftEngines);
				}else{
					cnt2 = 1;
					stopEngines();
				}
				statePP = 12;
			}
		break;
		case 12:
			//if(DISTANTA_PARCURSA > distDeParcurs){
				cnt2 --;
				if(cnt2 == 0){
					BTTransmitStr("xAm parcat paralel!");
					//stopEngines();
					return YES;
				}
			//}
		
		break;
	}
	return NO;	
}
char msg[100];
uint8_t tmpCnt;
uint32_t minTest;
uint8_t checkMinim(void)
{
	uint32_t sr;
	sr = getValueOfSensor(SideRightSensor);	
	
	sprintf(msg, "cm st:%u ", lastState);
	BTTransmitStr(msg);
	
	switch(lastState){
		case 0:
			lastState = 1;
			srTmp = sr;
			rotirePeLoc(15, 90, RightEngines);
		break;
		case 1:
			if( sr > srTmp + 5){
				srTmp = sr;
			}
			else if(sr < srTmp - 5){
				lastState = 2;
				tmpCnt=0;
			}
		break;
		case 2:
			tmpCnt++;
			srTmp = sr;
			if(tmpCnt>3){
				lastState = 3;
				minTest = sr;	
			}
		break;
		case 3:
			if( sr > srTmp )
			{
				lastState = 4;
				tmpCnt = 0;
				rotirePeLoc(4, 65, LeftEngines);
			}
			srTmp = sr;
			minTest = minTest>sr?sr:minTest;
		break;
		case 4:
			tmpCnt++;
			if(tmpCnt>4){
				stopEngines();
				return YES;
			}
		
		break;
		
		
	}
	
	return NO;
	
}