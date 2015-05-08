/*
 * PID1.c
 *
 * Created: 5/8/2015 12:28:23 AM
 *  Author: NMs
 */ 

#include <math.h>
#define epsilon 0.1
#define dt 0.01
#define MAX 4
#define MIN -4
#define Kp 0.1
#define Kd 0.01
#define Ki 0.005
static float pre_error= 0;

float PID1cal(float setpoint, float actual_position)
{

	static float integral=0;
	float error;
	float derivative;
	float output;

	error=setpoint- actual_position;
	if(fabs(error)>epsilon)
	{
		integral = integral +error*dt;
	}
	derivative =(error-pre_error)/dt;
	output = Kp*error + Ki*integral + Kd*derivative;
	if(output > MAX)
	{
		output=MAX;
	}
	if(output < MIN)
	{
		output=MIN;
	}
	pre_error=error;
	return output;
}
