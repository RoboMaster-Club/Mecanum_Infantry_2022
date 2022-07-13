/**
 * @file Gimbal_Control.c
 * @author Leo Liu
 * @brief control gimbal
 * @version 1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
#include "Gimbal_Control.h"

void Gimbal_Init(void);
void Gimbal_Control_Get_Data(Gimbal_t *Gimbal);
void Gimbal_Processing(Gimbal_t *Gimbal);

Gimbal_Func_t Gimbal_Func = Gimbal_Func_GroundInit;
#undef Gimbal_Func_GroundInit

Gimbal_t Gimbal;

void Gimbal_Init(void)
{
	GM6020_Yaw.Target_Angle = YAW_MID_MECH_ANGLE;
	GM6020_Pitch.Target_Angle = PITCH_MID_MECH_ANGLE;
}

void Gimbal_Control_Get_Data(Gimbal_t *Gimbal)
{
	GM6020_Yaw.Target_Angle -= DR16_Export_Data.Remote_Control.Joystick_Right_Vx/150;
	GM6020_Pitch.Target_Angle += DR16_Export_Data.Remote_Control.Joystick_Right_Vy/300;
	GM6020_Pitch.Target_Angle = VAL_LIMIT(GM6020_Pitch.Target_Angle,PITCH_UPPER_LIMIT,PITCH_LOWER_LIMIT);
}

void Gimbal_Processing(Gimbal_t *Gimbal)
{
	switch(Gimbal->Current_Mode)
	{
		case(Follow_Gimbal):
		{
			if(DR16_Export_Data.Remote_Control.Joystick_Right_Vx != 0)
			{
				Gimbal->Temp.Yaw_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
				Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,GM6020_Yaw.Target_Angle,Gimbal->Temp.Yaw_Angle);
				GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
			}
			else
			{
				Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,YAW_MID_MECH_ANGLE,Find_Gimbal_Min_Angle(GM6020_Yaw.Actual_Angle));
				GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
				GM6020_Yaw.Target_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			}
			
			Gimbal->Temp.Pitch_Angle = PITCH_DIRECTION * (GM6020_Pitch.Actual_Angle);
			Gimbal->Temp.Pitch_Speed = PID_Func.Positional_PID(&Pitch_Angle_PID,GM6020_Pitch.Target_Angle,Gimbal->Temp.Pitch_Angle);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Temp.Pitch_Speed,GM6020_Pitch.Actual_Speed);
			
			break;
		}
		
		case(Not_Follow_Gimbal):
		{
			Gimbal->Temp.Yaw_Angle = YAW_DIRECTION * (GM6020_Yaw.Total_Angle);
			Gimbal->Temp.Yaw_Speed = PID_Func.Positional_PID(&Yaw_Angle_PID,GM6020_Yaw.Target_Angle,Gimbal->Temp.Yaw_Angle);
			GM6020_Yaw.Output_Current = PID_Func.Positional_PID(&Yaw_Speed_PID,Gimbal->Temp.Yaw_Speed,-GM6020_Yaw.Actual_Speed);
			
			Gimbal->Temp.Pitch_Angle = PITCH_DIRECTION * (GM6020_Pitch.Actual_Angle);
			Gimbal->Temp.Pitch_Speed = PID_Func.Positional_PID(&Pitch_Angle_PID,GM6020_Pitch.Target_Angle,Gimbal->Temp.Pitch_Angle);
			GM6020_Pitch.Output_Current = PID_Func.Positional_PID(&Pitch_Speed_PID,Gimbal->Temp.Pitch_Speed,GM6020_Pitch.Actual_Speed);
			
			break;
		}
		
		case(Spin_Top):
		{
			
		}
		
		case(Disabled):
		{
			GM6020_Yaw.Output_Current = 0;
			GM6020_Pitch.Output_Current = 0;
			PID_Func.Clear_PID_Data(&Yaw_Angle_PID);
			PID_Func.Clear_PID_Data(&Yaw_Speed_PID);
			PID_Func.Clear_PID_Data(&Pitch_Angle_PID);
			PID_Func.Clear_PID_Data(&Pitch_Speed_PID);
			
			break;
		}
	}
}
