/*
 * Sensors.h
 *
 * Created: 5/5/2015 1:19:50 AM
 *  Author: NMs
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

#include "../Constants.c"

void initSensors(void);
uint16_t getValueOfSensor(Sensor);
uint16_t getCurrentValueOfSensor(Sensor);
void resetSensorQueue(Sensor);
void resetTimerQueue(void);

void startADCConversionForSensor(Sensor);
void initADC(void);



#endif /* SENSORS_H_ */