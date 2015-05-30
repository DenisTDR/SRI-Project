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
}

ISR(USART0_RX_vect)
{
	BTProtocolReadByte(UDR0);
	//PORTD ^=( 1<<PIND2);
	//UDR0 = 66;
}
extern volatile uint32_t encoder1CNT, encoder2CNT;
ISR(PCINT1_vect)
{
	//if(PORTB &( 1<<PINB0))
	encoder1CNT++;
	//if(PORTB &( 1<<PINB7))
	//	encoder2CNT++;
}

volatile uint8_t sradc0, shouldBlink=0;

int main(void)
{
	sei();
	BTInit();
	timer_init();
	initLeds();
	initEngines();
	initTimeQueue();
	initSensors();
	
	//PCMSK1 |= PCINT8;
	PCMSK1 |= PCINT15;
	//DDRB &= ~(1 << PINB0);
	DDRB &= ~(1 << PINB7);    
	PCICR |= (1<<PCIE1);
	
	addEntryToTimerQueue(&resetEncoders, 500UL*1000UL, Once);
	
	encoder1CNT=0;
	encoder2CNT=0;
	
	BTTransmitStr("  >>>main start<<<  ");
	
	char strBuffer[100];
	sprintf(strBuffer, "ic %lu", encoder1CNT);
	BTTransmitStr(strBuffer);
	
    while(1)
    {
		
		checkTimeQueue();
		//BTTransmitStr("-main loop-");
    }
}