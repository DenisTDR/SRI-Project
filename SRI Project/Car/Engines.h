/*
 * Engines.h
 *
 * Created: 5/5/2015 1:06:24 AM
 *  Author: NMs
 */ 


#ifndef ENGINES_H_
#define ENGINES_H_

#include "../Constants.c"

void goFront(uint8_t, uint8_t);
void goBack(uint8_t, uint8_t);
void goFrontLeft(uint8_t, uint8_t);
void goFrontRight(uint8_t, uint8_t);
void goBackLeft(uint8_t, uint8_t);
void goBackRight(uint8_t, uint8_t);
void leftEngines(uint8_t, uint8_t);
void rightEngines(uint8_t, uint8_t);
void checkFreeParallelParkingPlace(void);
void completeEnclosedContour(void);
void initEngines(void);
void stopEngines(void);
void rotirePeLoc(uint8_t, uint8_t);
void setEnginesSpeed(Engines, Sens, uint8_t);

#endif /* ENGINES_H_ */