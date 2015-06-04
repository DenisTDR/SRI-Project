/*
 * Settings.h
 *
 * Created: 5/30/2015 11:05:07 PM
 *  Author: NMs
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#define setBit(nr, bit, pos) (nr ^= (-bit ^ nr) & (1 << pos));

#define DEBUGGING (settings & 1)
#define READING_SENSORS (settings & 2)
#define SENDING_DISTTIME (settings & 4)
#define LAST_DEBUGGING (settings & 8)

extern volatile uint8_t settings;

void getSettings();
void setSettings(uint8_t _setting);
void setDebugging(uint8_t val);
void setReadingSensors(uint8_t val);
void setSendingInfos(uint8_t val);
void toggleDebuggingOff();


#endif /* SETTINGS_H_ */