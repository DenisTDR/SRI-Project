/*
 * Timing.h
 *
 * Created: 5/5/2015 11:00:15 AM
 *  Author: NMs
 */ 


#ifndef TIMING_H_
#define TIMING_H_

#include "avr/io.h"


void addEntryToTimerQueue(void (*) (void), uint32_t, uint8_t);
void removeEntryFromTimerQueue(void (*) (void));
int existsEntryInTimerQueue(void (*) (void));
void addEntryIfNotExists(void (*) (void), uint32_t, uint8_t);

void initTiming();
void timePassed(uint32_t);

void checkTimeQueue(void);
void initTimeQueue(void);
void resetTimerQueue(void);

#endif /* TIMING_H_ */