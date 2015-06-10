/*
 * functiiParcare.c
 *
 * Created: 5/24/2015 7:59:29 PM
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
#include "PID/PID1.h"

/*
void initParcare()
{
	stareV1=0;
	addEntryIfNotExists(&ParcareV1SMF,250*1000UL,Periodic);
}
uint8_t stareV1;
void ParcareV1SMF()
{
	//uint32_t frontLeft = getValueOfSensor(FrontLeftSensor);
	uint32_t frontRight = getValueOfSensor(FrontRightSensor);
	uint32_t sideLeft = getValueOfSensor(SideLeftSensor);
	uint32_t sideRight = getValueOfSensor(SideRightSensor);
	switch(stare)
	{
		case 0:
			goFront(60, 75);
			stareV1=1;
		break;
		case 1:
			if(frontRight < 208)
			{
				rotirePeLoc(60,75,RightEngines);
				stareV1 = 2;
			}
		break;
		case 2:
			if(sideRight > 150 && sideRight < 200)
				stareV1 =  3;
		break;
		case 3:
			if(SMPcheck()==Paralel)
			{
				goFront(60,75);
				stareV1 = 4;
			}	
		case 4:
		break;
		
	}
}*/
uint32_t cosPortiuni(uint32_t unghi){
	
	if(unghi <  15)
		return 126;
	if(unghi < 25)
		return 120;
	if(unghi < 35)
		return 111;
	return 98;
}

uint8_t doRightDistanceSMF(uint16_t dist, uint8_t eps);
uint8_t drdSMF();
uint8_t stareLeft;
void initDoLeftDistance (){
	stareLeft=0;
	addEntryIfNotExists(&drdSMF, 250*1000UL, Periodic);
}

uint8_t drdSMF(){
	return doRightDistanceSMF(100, 10);	
}
ParallelResult drdPR;
uint32_t drdTmp1;
uint8_t doRightDistanceSMF(uint16_t dist, uint8_t eps){
	uint32_t frontRight = getValueOfSensor(FrontRightSensor);
	uint32_t sideRight = getValueOfSensor(SideRightSensor);
	uint16_t diferenta;
	char msg[50];
		
	switch(stareLeft){
		case 0:
			if(sideRight > dist + eps/2){
				diferenta = sideRight - dist;
				drdPR = Departat;
			}else if(sideRight < dist - eps/2){
				diferenta = - sideRight + dist;				
				drdPR = Apropiat;
			}else return YES;
			drdTmp1 = ((sideRight+ 60)<<7) / cosPortiuni(diferenta / 3);
			rotirePeLoc(60, 85, RightEngines);
			stareLeft=1;
			sprintf(msg, "diff=%d   drdtmp=%lu  sideR=%lu", diferenta, drdTmp1, sideRight);
			BTTransmitStr(msg);
		break;
		case 1:
			if(sideRight > drdTmp1){
				stopEngines();
				return YES;				
			}
		break;
	}
	
	
	return NO;
}


uint8_t stareFP1;
uint8_t findPlaces1();
uint32_t lastVFP1, distP, distP2;

void initFindPlaces1(){
	stareFP1 = 0;
	lastVFP1=0;
	distP=0;
	distP2=0;
	addEntryIfNotExists(&findPlaces1, 150 * 1000UL, Periodic);
}

uint8_t findPlaces1(){
	uint32_t sideRight = getValueOfSensor(SideRightSensor);
	char msg[100];
	//sprintf(msg, "stare=%u\nlastV=%lu\nsr=%lu", stareFP1, lastVFP1, sideRight);
	//BTTransmitStr(msg);
	switch(stareFP1){
		case 0:
			lastVFP1 = sideRight;
			goFront(60, 50);
			stareFP1 = 1;		
		break;
		case 1:
			if(sideRight > lastVFP1 + 100){
				//stopEngines();
				stareFP1 = 2;
				distP = DISTANTA_PARCURSA;
				//char msg[50];
				//sprintf(msg, "%lu   ", distP);
				//BTTransmitStr(msg);
				distP2 = DISTANTA_PARCURSA + 100;
				
				sprintf(msg, "stare->2  distP=%lu", distP);
				BTTransmitStr(msg);
				
			}
			lastVFP1 = sideRight;
		break;
		case 2:
			if(DISTANTA_PARCURSA > distP2){
				stareFP1= 3;
				lastVFP1 = sideRight;
				
				sprintf(msg, "stare->3  DP=%lu", DISTANTA_PARCURSA);
				BTTransmitStr(msg);
			}
		break;
		case 3:
			if(sideRight > lastVFP1 + 100){
				//stopEngines();
				distP = (DISTANTA_PARCURSA - distP)/2 + DISTANTA_PARCURSA;
				stareFP1 = 4;
				goBack(60, 40);
				sprintf(msg, "stare->4 distP=%lu DP=%lu", distP, DISTANTA_PARCURSA);
				BTTransmitStr(msg);
				
			}
			lastVFP1 = sideRight;
		break;
		case 4:
			if(DISTANTA_PARCURSA > distP){
				stopEngines();
				return YES;
			}		
		break;
	}
	return NO;
}