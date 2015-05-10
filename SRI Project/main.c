/*
 * SRI_Project.c
 *
 * Created: 5/5/2015 12:49:42 AM
 *  Author: NMs
 */ 
#define F_CPU 8000000UL // 8 MHz

#include <stdio.h>
#include <avr/io.h>
#include "avr/interrupt.h"
#include <util/delay.h>

#include "Car/Sensors.h"
#include "Car/Engines.h"
#include "BTProtocol/BTProtocol.h"
#include "BTProtocol/Serializer.h"
#include "Timing/Timing.h"
#include "Car/Sensors.h"
#include "utile.h"


volatile uint32_t time=0;
void timer_init()
{
	//PIND = (1 << PIND0);
	TIMSK1|=(1<<TOIE1);
	TCNT1 = 0x00;
	TCCR1B = 0x01;//pt ca modificam ultimu bit din datasheet
}
ISR(TIMER1_OVF_vect)// Din datasheet timerq(are 8 mh) => 8/8= 1 microsecunde
{
	time++;
	//TCCR1B = 0x01; // init pentru 7.5 ms
	//TCCR1B = 0x02; // init pentru 60 ms
	//timePassed(7500); //7.5 ms = 7500 us;
	timePassed(8800);
	
	//PORTD ^= 1<<PIND5;
}

ISR(USART0_RX_vect)
{
	BTProtocolReadByte(UDR0);
}

volatile uint8_t sradc0, shouldBlink=0;

int main(void)
{
	BTInit();
	timer_init();
	initLeds();
	initEngines();
	initSensors();
	initTimeQueue();
	
	sei();
	BTTransmitStr("  >>>main start<<<  ");
	
	PORTD |= 1<<PIND6;
	
	//addEntryToTimerQueue(&functieRotireStanga, 1000UL * 50UL, Periodic);
	
    while(1)
    {
		//_delay_ms(1);
		checkTimeQueue();
		//test de la mine
		
		//BTTransmitStr("-main loop-");
    }
}