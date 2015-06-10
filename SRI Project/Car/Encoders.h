/*
 * Encoders.h
 *
 * Created: 6/1/2015 1:54:30 AM
 *  Author: NMs
 */ 

#ifndef ENCODERS_H_
#define ENCODERS_H_

#define DISTANTA_PARCURSA ( ((encoder1CNT + encoder2CNT)>>2) * 21 )

extern volatile uint32_t encoder1CNT, encoder2CNT;
void initEncoders();
void toggleCountingTimeForEncoders(uint8_t on);
void resetEncoders();
void setSendingDistAndTime(uint8_t on);
void getAverageSpeed(uint8_t reset);

#endif