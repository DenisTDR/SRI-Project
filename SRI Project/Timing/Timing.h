/*
 * Timing.h
 *
 * Created: 5/5/2015 11:00:15 AM
 *  Author: NMs
 */ 


#ifndef TIMING_H_
#define TIMING_H_

#include "avr/io.h"


void addEntryToTimerQueue(uint8_t (*) (void), uint32_t, uint8_t);
void removeEntryFromTimerQueue(uint8_t (*) (void));
uint8_t existsEntryInTimerQueue(uint8_t (*) (void));
void addEntryIfNotExists(uint8_t (*) (void), uint32_t, uint8_t);

void initTiming();
void timePassed(uint32_t);

void checkTimeQueue(void);
void initTimeQueue(void);
void resetTimerQueue(uint8_t keepReadSensors);

#endif /* TIMING_H_ */