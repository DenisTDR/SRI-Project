/*
 * Lights.h
 *
 * Created: 6/3/2015 8:37:57 PM
 *  Author: NMs
 */ 


#ifndef LIGHTS_H_
#define LIGHTS_H_


void initLights();
uint8_t blinkLeftLeds();
uint8_t blinkRightLeds();
void doBlinkLeds(uint32_t perioadaStanga, uint32_t perioadaDreapta);
void turnBlinkingOff();
uint8_t blinkNeons();

#endif /* LIGHTS_H_ */