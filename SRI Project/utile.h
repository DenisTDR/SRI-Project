/*
 * utile.h
 *
 * Created: 5/5/2015 11:15:44 AM
 *  Author: NMs
 */ 


#ifndef UTILE_H_
#define UTILE_H_


void ledAction(uint8_t a1, uint8_t a2);
void blinkA1(void);
void ReadSensor0(void);
void ReadSensor1(void);
void ReadSensor2(void);
void ReadSensor3(void);
void initLeds(void);
void sendTimeAsString(void);
void fctSmechera(void);
void doTimer(void);
void functieRotireStanga(void);
void sendEncoderCounter(void);

#endif /* UTILE_H_ */