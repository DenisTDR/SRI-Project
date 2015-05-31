/*
 * timing.c
 *
 * Created: 5/5/2015 11:00:00 AM
 *  Author: NMs
 */
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdio.h>
 #include "../BTProtocol/BTProtocol.h"
 #include "../Settings.h"

typedef struct{
	uint32_t delay; // delay in ms
	uint32_t repeatDelay; // pentru functii care trebuie apelate periodic, sau 0 pentru cele care nu se apeleaza de mai multe ori
	void (*pointerFct) (void); // pointer catre functia catre va fi apelata
}queueEntry;

void timePassed(uint32_t passed_us);

volatile uint32_t time=0;

ISR(TIMER1_OVF_vect)// Din datasheet timerq(are 8 mh) => 8/8= 1 microsecunde
{
	time++;
	timePassed(8800);
}

volatile queueEntry TimerQueue[20];
uint8_t TimerQueueSize = 20;

void initTiming()
{
	TIMSK1|=(1<<TOIE1);
	TCNT1 = 0x00;
	TCCR1B = 0x01;
}

void initTimeQueue(){
	uint8_t i;
	// initializez toti pointerii cu 0, adica pe fiecare pozitie din coada e o structura neocupata
	for(i=0;i<TimerQueueSize;i++)
		TimerQueue[i].pointerFct = 0;
}

// delay e in microsecunde (10^-6 secunde)
void addEntryToTimerQueue(void (*_theFct) (void), uint32_t _delay, uint8_t _repeat){
	cli(); // dezactivare intreruperi
	uint8_t i;
	
	for(i=0; i<TimerQueueSize; i++){
		if(TimerQueue[i].pointerFct == 0)
			break;
		if(TimerQueue[i].pointerFct == _theFct){
			if(DEBUGGING){
				char msg[100];
				sprintf(msg, "Functia asta e deja in coada! cu delay: %lu", TimerQueue[i].delay);
				BTTransmitStr(msg);
			}
			return;
		}
	}
	if(i == TimerQueueSize){
		//nu mai e loc in coada
		if(DEBUGGING)
			BTTransmitStr("Coada e full!");
		return;
	}
	
	//construire entry pentru functia curenta 
	queueEntry thisEntry;
	thisEntry.delay = _delay;
	thisEntry.repeatDelay = _repeat ? _delay :0;
	thisEntry.pointerFct = _theFct;
	
	TimerQueue[i] = thisEntry;
	
	if(DEBUGGING){
		char msg[100];		
		sprintf(msg, "Entry adaugat in coada! la index %d si cu delay: %lu", i, TimerQueue[i].delay);
		BTTransmitStr(msg);
	}
	sei();
}
void removeEntryFromTimerQueue(void (*_theFct) (void)){
	uint8_t i;
	for(i=0; i<TimerQueueSize; i++){
		if(TimerQueue[i].pointerFct == _theFct){
			TimerQueue[i].pointerFct = 0;
			TimerQueue[i].delay = 0;
			TimerQueue[i].repeatDelay = 0;
			if(DEBUGGING){
				char msg[100];
				sprintf(msg, "Entry sters din coada. de pe pozitia %d", i);
				BTTransmitStr(msg);
			}
		}
	}
}

uint8_t existsEntryInTimerQueue(void (*_theFct) (void)){
	uint8_t i;
	for(i=0; i<TimerQueueSize; i++){
		if(TimerQueue[i].pointerFct == _theFct){			
			return 1;
		}
	}
	return 0;
}

void addEntryIfNotExists(void (*_theFct) (void), uint32_t _delay, uint8_t _repeat){
	if(!existsEntryInTimerQueue(_theFct)){
		addEntryToTimerQueue(_theFct, _delay, _repeat);
	}	
}
//se apeleaza din intrerupere (cate microsecunde au trecut)
//pentru fiecare functie(valida) din queue se scade din delay numarul de microsecunde care a trecut

void timePassed(uint32_t passed_us){
	uint8_t i;
	for(i=0; i<TimerQueueSize; i++){
		if(TimerQueue[i].pointerFct != 0){
			if(TimerQueue[i].delay > 0)
				TimerQueue[i].delay = TimerQueue[i].delay > passed_us ? TimerQueue[i].delay-passed_us : 0;
		}
	}
}

//se apeleaza din main loop
//verifica daca sunt functii care trebuie apelate (daca delay-ul lor a ajuns la 0)
void checkTimeQueue(void){
	cli();
	uint8_t i;
	for(i=0; i<TimerQueueSize; i++){
		if(TimerQueue[i].pointerFct != 0){
			if(TimerQueue[i].delay == 0){
				TimerQueue[i].pointerFct();
				if(TimerQueue[i].repeatDelay)
					TimerQueue[i].delay = TimerQueue[i].repeatDelay;
				else
					TimerQueue[i].pointerFct = 0;
			}
		}
	}
	sei();
}

void resetTimerQueue(){
	initTimeQueue();	
}