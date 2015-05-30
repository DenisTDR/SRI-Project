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
        GoM2P1,   // 7
        GoM2P2,   // 8
        GoM2P3,   // 9
        ParkAt,   // 10
        ParallelPark, // 11
        FinalP1,  // 12
        FinalP2,  // 13
        FinalP3,  // 14
        IVelocityAvg, // 15
        IVelocityInst, // 16
        IDistance,     // 17
        Int32Value,    // 18
        ReTransmitLastMsg,  // 19
        DisplayMessage,   // 20
		Led,			  // 21
		ReadSensorValue,  // 22
		StopEngines,      // 23
		RotirePeLocStanga, // 24
		RotirePeLocDreapta, // 25
		GetAverageSpeed,    //26
		ResetThings,		//27
        EndAction         // 28
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