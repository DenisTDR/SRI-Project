/*
 * Settings.h
 *
 * Created: 5/30/2015 11:05:07 PM
 *  Author: NMs
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#define DEBUGGING (debugging & 1)
#define LAST_DEBUGGING (debugging & 2)

volatile uint8_t debugging;
void turnDebuggingOff();
void turnDebuggingOn();
void toggleDebuggingOff();



#endif /* SETTINGS_H_ */