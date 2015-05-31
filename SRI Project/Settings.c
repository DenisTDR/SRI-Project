/*
 * Settings.c
 *
 * Created: 5/5/2015 12:45:43 PM
 *  Author: NMs
 */ 
#include <avr/io.h>

#define DEBUGGING (debugging & 1)
#define LAST_DEBUGGING (debugging & 2)


volatile uint8_t debugging = 0;

void turnDebuggingOff(){
	debugging = 0;
}

void turnDebuggingOn(){
	debugging = 1;
}

void toggleDebuggingOff(){
	if(DEBUGGING){
		debugging = 2;
	}
	else
		if(LAST_DEBUGGING)
			debugging = 1;
}