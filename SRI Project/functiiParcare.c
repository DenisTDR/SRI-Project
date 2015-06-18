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
			if(SMRPcheck()==Paralel)
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
	//uint32_t frontRight = getValueOfSensor(FrontRightSensor);
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
uint8_t intrareParcareSMF();

uint8_t stareFP1,stareLocLiber,liber,stareIntrareParcare, stareIntrare;
uint8_t findPlaces1();
uint32_t lastVFP1, distP, distP2,distLocLiber;

uint8_t locLiberSMF();
void initLocLiber()
{
	stareLocLiber=0;
	distP=0;
	addEntryIfNotExists(&locLiberSMF, 150 * 1000UL, Periodic);
}
uint8_t locLiberSMF(){
	uint32_t sideRight = getValueOfSensor(SideRightSensor);
	char msg[50];
	switch(stareLocLiber){
		case 0:
			stareLocLiber = 1;
			liber = 1;
		break;
		case 1:
			distLocLiber = DISTANTA_PARCURSA + 40;
			liber *= (sideRight > 300);
			sprintf(msg, "l1=%d", liber);
			BTTransmitStr(msg);
			stareLocLiber=2;
			goBack(60,50);			
		break;
		case 2:
			if(DISTANTA_PARCURSA > distLocLiber){
				liber *= (sideRight > 300);
				sprintf(msg, "l1=%d", liber);
				BTTransmitStr(msg);
				distLocLiber = DISTANTA_PARCURSA + 80;
				stareLocLiber = 3;
				goFront(60, 50);
			}
		break;
		case 3:	
			if(DISTANTA_PARCURSA > distLocLiber){
				liber *= (sideRight > 300);
				sprintf(msg, "l1=%d", liber);
				BTTransmitStr(msg);
				stopEngines();
				return liber | 2;				
			}
			
		break;
	}
	return NO;

}

void initFindPlaces1(){
	stareFP1 = 0;
	lastVFP1=0;
	distP=0;
	distP2=0;
	addEntryIfNotExists(&findPlaces1, 250 * 1000UL, Periodic);
}

uint8_t findPlaces1(){
	uint32_t frontRight = getValueOfSensor(FrontRightSensor);
	char msg[100];
	switch(stareFP1){
		case 0:
			lastVFP1 = frontRight;
			goFront(60, 90);
			stareFP1 = 1;
			BTTransmitStr("0->1");
		break;
		case 1:
			if(frontRight > lastVFP1 + 100){
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
			lastVFP1 = frontRight;
		break;
		case 2:
			if(DISTANTA_PARCURSA > distP2){
				stareFP1= 3;
				lastVFP1 = frontRight;
			
				sprintf(msg, "stare->3  DP=%lu", DISTANTA_PARCURSA);
				BTTransmitStr(msg);
			}
		break;
		case 3:
			if(frontRight > lastVFP1 + 100){
				//stopEngines();
				distP = (DISTANTA_PARCURSA - distP)/2 + DISTANTA_PARCURSA + 50;
				stareFP1 = 4;
				goBack(60, 60);
				sprintf(msg, "stare->4 distP=%lu DP=%lu", distP, DISTANTA_PARCURSA);
				BTTransmitStr(msg);
			
			}
			lastVFP1 = frontRight;
		break;
		case 4:
			if(DISTANTA_PARCURSA > distP){
				//stopEngines();
				//return YES;
				stareLocLiber = 0;
				stareFP1 = 5;
			}
		break;
		case 5:
			if((distP = locLiberSMF())){
				if(distP & 1){
					BTTransmitStr("locul e liber");
				}
				else{
					BTTransmitStr("locul nu e liber");
				}
				return YES;
				stopEngines();
			}
		break;
	}
	return NO;
}
extern ParallelResult oldRez;
uint8_t intrareLocParcareSMF();
void initIntrareParcare()
{
	stareIntrareParcare = 0;
	oldRez = PreaApropiat;
	distP=0;
	stareIntrare =0;
	addEntryIfNotExists(&intrareParcareSMF, 250*1000UL, Periodic);
}
uint32_t lastLRsum = 0;

uint8_t intrareLocParcareSMF()
{
	uint32_t sideRight = getValueOfSensor(SideRightSensor);
	uint32_t sideLeft = getValueOfSensor(SideLeftSensor);
	uint32_t frontLeft = getValueOfSensor(FrontLeftSensor);
	uint32_t frontRight = getValueOfSensor(FrontRightSensor);
	char msg[100];
	sprintf(msg, "stare=%d ", stareIntrareParcare);
	BTTransmitStr(msg);
	switch(stareIntrareParcare)
	{
		case 0:
			rotirePeLoc(60,120,LeftEngines);
			distP = DISTANTA_PARCURSA + 190;
			stareIntrareParcare=1;
		break;
		case 1:
			if(DISTANTA_PARCURSA > distP )
			{
				 goFront(60, 90);
				 stareIntrareParcare = 2;
				 distP = DISTANTA_PARCURSA + 200;
			}
		break;
		case 2:
			if(distP < DISTANTA_PARCURSA)
			{
				stareIntrareParcare = 3;
				stopEngines();
			}
		break;
		case 3:
			if( frontRight > 150 && frontLeft > 150){
				if( SMPcheckSide()==YES)
				{
					stareIntrareParcare = 4;
				}
			}
			else {
				stareIntrareParcare = 5;
				stopEngines();
			}
		break;
		case 4:
			goFront(60, 40);
			stareIntrareParcare = 3;
		break;
		case 5:
			if(sideLeft + sideRight  > 150 )
			{
				rotirePeLoc(10, 70, LeftEngines);
				lastLRsum = sideRight + sideLeft; 
				stareIntrareParcare = 59;
			}
			else
				stareIntrareParcare = 100;
		break;
		case 59:
			stareIntrareParcare = 6;
		break;
		case 6:
			if(sideLeft + sideRight  > lastLRsum + 3 )
				rotirePeLoc(10, 70, RightEngines);
			stareIntrareParcare = 7;
			break;
		case 7:
			if(sideLeft + sideRight < 150)
				stareIntrareParcare = 100;
		break;
		case 100:
			stopEngines();
			BTTransmitStr("xAm parcat");
			return YES;
		break;
			
	}
	return NO;
}

uint8_t intrareParcareSMF()
{
	uint32_t sideRight = getValueOfSensor(SideRightSensor);
	//uint32_t sideLeft = getValueOfSensor(SideLeftSensor);
	uint32_t frontLeft = getValueOfSensor(FrontLeftSensor);
	uint32_t frontRight = getValueOfSensor(FrontRightSensor);
	char msg[100];
	sprintf(msg, "stareI =%d ", stareIntrare);
	BTTransmitStr(msg);
	switch(stareIntrare){
		case 0:
			goFront(60,70);
			stareIntrare = 1;
		break;
		
		case 1:
			if(frontLeft < 150 || frontRight < 150)
			{
				stareIntrare = 2;
				BTTransmitStr("YOLO");
				distP = DISTANTA_PARCURSA + 190;
				rotirePeLoc(10, 120,RightEngines);
			}
		break;
		case 2:
			if( DISTANTA_PARCURSA > distP)
			{
				stopEngines();
				stareIntrare = 3;
			}
		break;
		case 3:
			if(SMRPcheck()==YES)
			{
				goFront(19,100);
				stareIntrare = 4;
			}
		break;
		case 4:
			if(sideRight < 30 || sideRight > 150)
			{
				goBack(20,40);
				stareIntrare = 5;
			}
		break;
		case 5:
			if(sideRight > 50 && sideRight < 120)
			{
				stopEngines();
				distP = DISTANTA_PARCURSA + 150;
				goFront(20,60);
				stareIntrare = 6;
			}
		break; 
		case 6:
			if(DISTANTA_PARCURSA > distP)
			{
				stareIntrare = 7;
				distP = DISTANTA_PARCURSA + 190;
				rotirePeLoc(10, 120,LeftEngines);
			}
		break;
		case 7:
			if(DISTANTA_PARCURSA > distP)
			{
				stopEngines();
				stareIntrare = 8;
			}
		break;
		case 8:
			if(SMLPcheck()==YES){
				stopEngines();
				stareFP1 = 0;
				stareIntrare = 9;
			}
		break;
		case 9:
			if(findPlaces1())
				return YES;
		break;
		}
	return NO;
}