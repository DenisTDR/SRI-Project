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
 #include "../Constants.c"
 #include "../utile.h"
 #include "../Car/Sensors.h"
 #include "../Car/Engines.h"
 
 #define TimingQueueSize 20

typedef struct{
	uint32_t delay; // delay in ms
	uint32_t repeatDelay; // pentru functii care trebuie apelate periodic, sau 0 pentru cele care nu se apeleaza de mai multe ori
	uint8_t (*pointerFct) (void); // pointer catre functia catre va fi apelata
}queueEntry;

void timePassed(uint32_t passed_us);
void shiftTimeQueue(uint8_t i, char *reason);
void initTimeQueue();
volatile uint32_t timerClocks;
ISR(TIMER1_OVF_vect)// Din datasheet timerq(are 8 mh) => 8/8= 1 microsecunde
{
	timerClocks++;
	timePassed(8210);
	//blinkA1();
}

void initTiming()
{
	timerClocks=0;
	initTimeQueue();
	TIMSK1|=(1<<TOIE1);
	TCNT1 = 0x00;
	TCCR1B = 0x01;
}
volatile queueEntry TimerQueue[TimingQueueSize+1];
volatile uint8_t TimingQueueTop;


void initTimeQueue(){
	uint8_t i;
	// initializez toti pointerii cu 0, adica pe fiecare pozitie din coada e o structura neocupata
	for(i=0;i<TimingQueueSize;i++)
		TimerQueue[i].pointerFct = NULL;
	TimingQueueTop = 0;
}

// delay e in microsecunde (10^-6 secunde)
void addEntryToTimerQueue(uint8_t (*_theFct) (void), uint32_t _delay, uint8_t _repeat){
	//cli(); // dezactivare intreruperi
	
	
	//construire entry pentru functia curenta 
	queueEntry thisEntry;
	thisEntry.delay = _delay;
	thisEntry.repeatDelay = _repeat ? _delay : 0;
	thisEntry.pointerFct = _theFct;	
	TimerQueue[TimingQueueTop] = thisEntry;
	
	TimingQueueTop++;
	
	if(DEBUGGING){
		char msg[100];		
		sprintf(msg, "Entry adaugat in coada! la index %d si cu delay: %lu", TimingQueueTop-1, TimerQueue[TimingQueueTop-1].delay);
		BTTransmitStr(msg);
	}
	//sei();
}
uint8_t removeEntryFromTimerQueue(uint8_t (*_theFct) (void)){
	uint8_t i, c=0;
	for(i=0; i<TimingQueueTop; i++){
		if(TimerQueue[i].pointerFct == _theFct){
			if(DEBUGGING){
				char msg[100];
				sprintf(msg, "Entry sters din coada. de pe pozitia %u", i);
				BTTransmitStr(msg);
			}
			shiftTimeQueue(i, "remove function"); 
			i--;
			c++;
		}
	}
	return c;
}

uint8_t existsEntryInTimerQueue(uint8_t (*_theFct) (void)){
	uint8_t i;
	for(i=0; i<TimingQueueTop; i++){
		if(TimerQueue[i].pointerFct == _theFct){			
			return YES;
		}
	}
	return NO;
}

void addEntryIfNotExists(uint8_t (*_theFct) (void), uint32_t _delay, uint8_t _repeat){
	uint8_t i;
	for(i=0; i<TimingQueueTop; i++){
		if(TimerQueue[i].pointerFct == _theFct){
			TimerQueue[i].delay = _delay;
			TimerQueue[i].repeatDelay = _repeat?_delay:0;
			break;
		}
	}	
	if(i==TimingQueueTop)
		addEntryToTimerQueue(_theFct, _delay, _repeat);
}



//se apeleaza din intrerupere (cate microsecunde au trecut)
//pentru fiecare functie(valida) din queue se scade din delay numarul de microsecunde care a trecut

void timePassed(uint32_t passed_us){
	uint8_t i;
	for(i=0; i<TimingQueueTop; i++){
		if(TimerQueue[i].pointerFct != NULL){
			if(TimerQueue[i].delay > 0)
				TimerQueue[i].delay = TimerQueue[i].delay > passed_us ? TimerQueue[i].delay - passed_us : 0;
		}
	}
}

//se apeleaza din main loop
//verifica daca sunt functii care trebuie apelate (daca delay-ul lor a ajuns la 0)
void checkTimeQueue(void){
	uint8_t i;
	for(i=0; i<TimingQueueTop; i++){
		if(TimerQueue[i].pointerFct != NULL){
			if(TimerQueue[i].delay == 0){
				if(TimerQueue[i].pointerFct()){
					shiftTimeQueue(i, "returned true");
					i--;
				}
				else
					if(TimerQueue[i].repeatDelay)
						TimerQueue[i].delay = TimerQueue[i].repeatDelay;
					else{
						if(TimerQueue[i].pointerFct ==  &stopEngines)
							i -= removeEntryFromTimerQueue(&stopEngines);
						else
							shiftTimeQueue(i, "not periodic"),
							i--;
					}
			}
		}
	}
	//sei();
}
void shiftTimeQueue(uint8_t i, char *reason){
	if(DEBUGGING){
		//char str[50];
		//sprintf(str, "shift i=%d (%s)", i, reason);
		//BTTransmitStr(str);
	}
	for(; i<TimingQueueTop; i++)
		TimerQueue[i] = TimerQueue[i+1];
	TimingQueueTop--;
}

void resetTimerQueue(void){
	uint8_t i;
	for(i=0; i<TimingQueueSize; i++)
		shiftTimeQueue(i, "reset timer queue");
	TimingQueueTop = 1;
}