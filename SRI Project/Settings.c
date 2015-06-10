/*
 * Settings.c
 *
 * Created: 5/5/2015 12:45:43 PM
 *  Author: NMs
 */ 
#include <avr/io.h>
#include "BTProtocol/BTProtocol.h"
#include "Constants.c"
#include "Timing/Timing.h"
#include "Car/Encoders.h"
#include "Car/Sensors.h"


#define setBit(nr, bit, pos) (nr ^= (-bit ^ nr) & (1 << pos));

#define DEBUGGING (settings & 1)
#define READING_SENSORS (settings & 2)
#define SENDING_DISTTIME (settings & 4)
#define SENDING_SENSORS (settings & 8)
#define LAST_DEBUGGING (settings & 16)

volatile uint8_t settings = 0;

void setDebugging(uint8_t val){
	setBit(settings, (val&1), 0);	
}
void setReadingSensors(uint8_t val){
	setBit(settings, (val&1), 1);
	setSensorsRead(val);
}
void setSendingInfos(uint8_t val){
	setBit(settings, (val&1), 2);
	setSendingDistAndTime(val);
}
void setSendingSensors(uint8_t val){
	setBit(settings, (val&1), 3);
	setSensorsSend(val);
}

void toggleDebuggingOff(uint8_t val){
	if(!val){
		if(DEBUGGING){
			setDebugging(0);
			setBit(settings, 1, 4);
		}
	}
	else
		if(LAST_DEBUGGING){
			setDebugging(1);
			setBit(settings, 0, 4);
		}
}

void getSettings(){
	uint8_t msgtt[6];
	msgtt[0] = StartByte;
	msgtt[1] = ICarSettings;
	msgtt[2] = 1;
	msgtt[3] = settings;
	msgtt[4] = EndByte;
	BTTransmitMsgU(msgtt, 5);
}
void setSettings(uint8_t _setting){
	setDebugging( _setting & 1 );
	setReadingSensors( (_setting>>1) & 1 );
	setSendingInfos( (_setting>>2) & 1 );
	setSendingSensors( (_setting >>3) & 1);
	getSettings();
}
