/*
 * Engines.h
 *
 * Created: 5/5/2015 1:06:24 AM
 *  Author: NMs
 */ 


#ifndef ENGINES_H_
#define ENGINES_H_

#include "../Constants.c"

void goFront(uint8_t timp, uint8_t viteza);
void goBack(uint8_t, uint8_t);
void goFrontLeft(uint8_t, uint8_t);
void goFrontRight(uint8_t, uint8_t);
void goBackLeft(uint8_t, uint8_t);
void goBackRight(uint8_t, uint8_t);
void leftEngines(uint8_t, uint8_t);
void checkFreeParallelParkingPlace(void);
void completeEnclosedContour(void);
void initEngines(void);
uint8_t stopEngines(void);
void rotirePeLoc(uint8_t, uint8_t, uint8_t);
void setEnginesSpeed(Engines, Sens, uint8_t);
void getAverageSpeed(uint8_t);
void resetEncoders();
void rotireSmechera(uint8_t timp, Sens sensStanga, uint8_t vitezaStanga, Sens sensDreapta, uint8_t vitezaDreapta);

#endif /* ENGINES_H_ */