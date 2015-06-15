/*
 * Constants.c
 *
 * Created: 5/5/2015 12:55:02 AM
 *  Author: NMs
 */ 

#include <avr/io.h>

#ifndef CONSTANTS_C
#define CONSTANTS_C


#define Periodic 1
#define Once 0
#define ON 1
#define OFF 0
#define YES 1
#define NO 0
#define StartByte (unsigned char)0xAA
#define EndByte (unsigned char)0x55
typedef enum{
        NoAction,
        GoFront,
        GoBack,
        GoLeftF,
        GoRightF,
        GoLeftB,
        GoRightB,
        RotirePeLocStanga,
        RotirePeLocDreapta,
        RotireSmechera,
        GoM2P1,
        GoM2P2,
        GoM2P3,
        ParkAt,
        ParallelPark,
        FinalP1,
        FinalP2,
        FinalP3,
        InfoCarStats,
        ISensorsValues,
        ICarSettings,
        Int32Value,
        ReTransmitLastMsg,
        DisplayMessage,
        Led,
        ReadSensorValue,
        StopEngines,
        GetAverageSpeed,
        ParcurgereDistanta,
        ResetThings,
        GetSettings,
        SetSettings,
		CarStarted,
		CRCSumFailed,
        EndAction
} CarAction;

typedef enum{
    WaitingStartByte,  //0
    WaitingCarAction,  //1
    WaitingDataLength, //2
    ReadingData,        //3
    WaitingCommandId, //4
	WaitingCRCByte, //5
    WaitingEndByte  //6
} BTState;

typedef enum{
	SideLeftSensor,	//0
	SideRightSensor,	//1
	FrontRightSensor, // cel care e indreptat inspre dreapta	2
	FrontLeftSensor // cel care e indreptat inspre stanga  3
}Sensor;

typedef enum{
	RightEngines,
	LeftEngines
}Engines;

typedef enum{
	SensFata,
	SensSpate
}Sens;

typedef enum{
	PreaApropiat,
	Apropiat,
	Paralel,
	Departat,
	PreaDepartat
}ParallelResult;


union unionUInt32ToArray{
	uint32_t nr;
	uint8_t array[4];
};

#endif