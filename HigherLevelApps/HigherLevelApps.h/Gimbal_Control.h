/**
 * @file Gimbal_Control.h
 * @author Leo Liu
 * @brief header file for gimbal control
 * @version 1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __GIMBAL_CONTROL_H
#define __GIMBAL_CONTROL_H

#include "DR16_Remote.h"
#include "User_Defined_Math.h"
#include "PID.h"
#include "Robot_Control.h"
#include "Board_A_IMU.h"
#include "MPU6050_IMU.h"

#define YAW_DIRECTION 1 //Yaw motor direction
#define YAW_MID_MECH_ANGLE 6650 //This is measured for yaw origin
#define PITCH_DIRECTION 1 //Pitch motor direction
#define PITCH_MID_MECH_ANGLE 5800 // This is measured for pitch origin
#define PITCH_LOWER_LIMIT 5120 //Pitch cannot rotate 360 degree, so these are the limits in mechanical angle
#define PITCH_UPPER_LIMIT 6600

#define Gimbal_Func_GroundInit				\
		{																	\
				&Gimbal_Init,									\
						&Gimbal_Control_Get_Data,	\
						&Gimbal_Processing,				\
		}
		
typedef struct
{
  int Current_Mode;	
	int Prev_Mode;

	struct
	{
		float Yaw_Angle;
		float Yaw_Speed;
		float Pitch_Angle;
		float Pitch_Speed;
	}Temp; //Temporary storing data, write here for easy debug
	
	uint8_t Gimbal_Back_Mid_Flag;
	uint8_t Gimbal_Offline_Flag;
}Gimbal_t;

typedef struct
{
	void (*Gimbal_Init)(void);
	void (*Gimbal_Control_Get_Data)(Gimbal_t *Gimbal);
	void (*Gimbal_Processing)(Gimbal_t *Gimbal);
}Gimbal_Func_t;
 
extern Gimbal_Func_t Gimbal_Func;
extern Gimbal_t Gimbal;

#endif
