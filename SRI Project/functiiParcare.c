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
#include "PID/PID1.h"

typedef enum{
	Start,//0
	intrareInParcare,
	rotireLoc,
	rotireMersDR,
	rotireMersSt,
	mersFata,
	poarta,
	bv,
	nuPoarta
}test1;

test1 stare = Start;
volatile uint8_t iesire=0;
extern volatile uint8_t debugging;

uint16_t esteParalel(uint16_t sensorFata, uint16_t sensorSpate)
{
	//0.833333 = 13/16;
	
	
	//145. + 0.833333
	return 0;
}

uint8_t str = 0;
void testFct1(void){

	switch(str){
		case 0:
			str = 1;
			goFront(60, 150);
		break;
		case 1:
			if(0){//getValueOfSensor2(SideRightSensor, 1) < 150){
				str = 2;
				stopEngines();
			}
		break;
		case 2:
			BTTransmitStr("Done!");
			str = 3;
		break;
		case 3:
			removeEntryFromTimerQueue(&testFct1);
			str = 0;
		break;
	}	
}


void functieRotireStanga(void){
	uint16_t senzorSt = getValueOfSensor(SideLeftSensor);
	uint16_t senzorDr = getValueOfSensor(SideRightSensor);
	debugging = 0;
	uint16_t senzorStFata = getValueOfSensor(FrontLeftSensor);
	uint16_t senzorDrFata =getValueOfSensor(FrontRightSensor);
	
	char str[100];
	sprintf(str, "SF=%d  DF=%d  S=%d  D=%d  stare=%d", senzorStFata,senzorDrFata, senzorSt,senzorDr, stare);
	BTTransmitStr(str);
	
	switch(stare){
		case Start:
			stare = intrareInParcare;
			BTTransmitStr("Sa incepem :D");
		break;
		case intrareInParcare:/*
			if(senzorStFata>200 && senzorDrFata>200){
				rotirePeLoc(60, 75, RightEngines);
				stare=rotireLoc;
			}*/
			goFrontRight(60,125);
			stare = rotireMersDR;			
		break;
		case mersFata: // merge in fata
			if(senzorStFata>150 && senzorDrFata>150){
				stare = rotireLoc;
				//goFrontLeft(60, 250);
				rotirePeLoc(60, 75, LeftEngines);
			}
			if(senzorDr<100){
				iesire--;
				if(iesire==0){
					rotirePeLoc(60,75, RightEngines);
					stare= poarta;}
				else stare=nuPoarta;
				
			}
			if(senzorDr > 220){
				stare = rotireMersSt;
				goFrontLeft(60, 75);
			}		
			if(senzorDr<250 && senzorDr>170){
				stare= rotireMersDR;
				goFrontRight(60, 75);
			}
				
		break;
		case poarta:
			if(senzorStFata <91 && senzorDrFata <180){
				goFront(6,75);
				stare=bv;
			}
		break;	
		case bv:
			{
				stopEngines();
				removeEntryFromTimerQueue(&functieRotireStanga);
			}
		break;		
		case rotireMersSt:
			if(senzorDr < 300){
				stare = mersFata;
				goFront(60, 75);
			}		
		break;
		case rotireMersDR:
			//if(range(senzorDr,senzorDrFata)==0)
			{
				stare=mersFata;
				goFront(60, 75);
			}
		break;
		case rotireLoc:
			if(senzorDrFata> 100 && senzorDrFata<140){
				goFront(60, 75);
				stare=mersFata;				
			}
		break;
		case nuPoarta:
			if(senzorDr < 300){
				stare=mersFata;
				goFront(60,75);
			}
		break;
	}
	
	debugging = 1;
}
void parcLat()
{
	uint16_t senzorDr = getValueOfSensor(SideRightSensor);
	debugging = 0;
	uint16_t senzorStFata = getValueOfSensor(FrontLeftSensor);
	uint16_t senzorDrFata =getValueOfSensor(FrontRightSensor);
	
	char str[100];
	sprintf(str, "SF=%d  DF=%d   D=%d  stare=%d", senzorStFata,senzorDrFata,senzorDr, stare);
	BTTransmitStr(str);
	
	switch(stare){
	case Start:
		goFront(60,100);
		if(senzorDr<140)
			{
				stare=rotireMersDR;
			    goFrontRight(100,RightEngines);
				
			}
	break;
	case rotireMersDR:
		if( senzorStFata >300)//senzorStFata >260 &&
		{
			
			stare=mersFata;
		}
	break;
	case mersFata:
		if(senzorDrFata>400)
		{
			rotirePeLoc(30, 100,LeftEngines);
			stare=rotireLoc;
		}
	break;
	case  rotireLoc:
		if(senzorDr >400)
			stopEngines();
	break;
	default:
	break;
	}
}

uint8_t st;
 /*void parcareLaterala()
 {
	 char str[70];
	 uint32_t tmp2;
	 sprintf(str, "st = %d",st);
	 BTTransmitStr(str);

	 switch (state2)
	 {
		 case 0:
			goFront(30, 200);
			st = 1;
			break;
		 case 1:
			tmp2 = getValueOfSensor2(SideRightSensor)
			//getSensorValue(Sensor.SideRight);
			 if (tmp2 < 100)
			 {
				 st = 2;
			 }
			 break;
		 case 2:
		 tmp2 = getValueOfSensor2(SideRightSensor)
		 // funcs.getSensorValue(Sensor.SideRight);
		 if (tmp2 > 120)
		 {
			 st=3;
			 goFrontRight(30, 240);
			 //RealFuncs.goFrontRight(30, 200);
		 }
		 break;
		 case 3:
			tmp2=getValueOfSensor2(FrontRightSensor);
		 //tmp2 = funcs.getSensorValue(Sensor.FrontRight);
		 if (tmp2 < 55)
		 {
			 st=4;
			 goFrontLeft(1, 240);
			 //RealFuncs.goFrontLeft(30, 250);
		 }

		 break;

		 case 4:
		 removeEntryFromTimerQueue(&parcareLaterala);
		 if (isRightParalel() == 0)
		 {
			 RealFuncs.goFront(30, 175);
			 state2 = 5;
			 return false;
		 }
		 break;
		 case 5:
		 if (Math.Abs(funcs.getSensorValue(Sensor.FrontLeft) - funcs.getSensorValue(Sensor.FrontRight)) < 10)
		 {
			 RealFuncs.StopEngines();
			 return true;
		 }
		 break;
		 default:
		 break;

	 }

	 return false;
 }*/