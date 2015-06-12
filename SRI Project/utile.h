/*
 * utile.h
 *
 * Created: 5/5/2015 11:15:44 AM
 *  Author: NMs
 */ 


#ifndef UTILE_H_
#define UTILE_H_
#include "Constants.c"

#define setBit(nr, bit, pos) (nr ^= (-bit ^ nr) & (1 << pos));
uint8_t SMPcheckSide();
void ledAction(uint8_t a1, uint8_t a2);
uint8_t blinkA1(void);
void sendTimeAsString(void);
uint8_t fctSmechera(void);
uint8_t doTimer(void);
uint8_t functieRotireStanga(void);
void sendEncoderCounter(void);
void initAndStartStateMachineTest1();
void initParcurgereDistanta(uint32_t distanta, uint8_t viteza);
uint8_t SMPcheck();
ParallelResult isParalel();

void initParalelCheck();

void initParalelParking();

#endif /* UTILE_H_ */