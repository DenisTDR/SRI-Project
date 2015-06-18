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
#define SENDING_SENSORS (settings & 8)
#define NEONS_SETTING ( (settings>>4) & 0x7 )
#define LAST_DEBUGGING (settings & 128)

extern volatile uint8_t settings;

void getSettings();
void setSettings(uint8_t _setting);
void setDebugging(uint8_t val);
void setReadingSensors(uint8_t val);
void setNeonSetting(uint8_t val);
void setSendingInfos(uint8_t val);
void toggleDebuggingOff(uint8_t val);
void reSetSettings();

#endif /* SETTINGS_H_ */