/*
 * Constants.c
 *
 * Created: 5/5/2015 12:55:02 AM
 *  Author: NMs
 */ 
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
	NoAction, // 0
	GoFront,  // 1
	GoBack,	  // 2
	GoLeftF,  // 3
	GoRightF, // 4
	GoLeftB,  // 5
	GoRightB, // 6
	RotirePeLocStanga, // 7
	RotirePeLocDreapta, // 8
	RotireSmechera,
	GoM2P1,   // 9
	GoM2P2,   // 10
	GoM2P3,   // 11
	ParkAt,   // 12
	ParallelPark, // 13
	FinalP1,  // 14
	FinalP2,  // 15
	FinalP3,  // 16
	InfoCarStats,     // 19
	Int32Value,    // 20
	ReTransmitLastMsg,  // 21
	DisplayMessage,   // 22
	Led,			  // 23
	ReadSensorValue,  // 24
	StopEngines,      // 25
	GetAverageSpeed,   //26
	ParcurgereDistanta, //27
	ResetThings,		//28
	GetSettings,		//29
	SetSettings,		//29
	EndAction         // 30
} CarAction;

typedef enum{
    WaitingStartByte,  //0
    WaitingCarAction,  //1
    WaitingDataLength, //2
    ReadingData,        //3
    WaitingEndByte      //4
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


#endif