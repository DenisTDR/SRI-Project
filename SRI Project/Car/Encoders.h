/*
 * Encoders.h
 *
 * Created: 6/1/2015 1:54:30 AM
 *  Author: NMs
 */ 

#ifndef ENCODERS_H_
#define ENCODERS_H_

#define DISTANTA_PARCURSA ( ((encoder1CNT+encoder2CNT)>>2) * 21 )
void initEncoders();
void countSecondsForEncoders();
void resetEncoders();
void getAverageSpeed(uint8_t reset);

#endif