/**
 * @file Super_Capacitor.h
 * @author Leo Liu
 * @brief header file for super capacitor
 * @version 1.0
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "can.h"
#include "CAN_Setup.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SUPERCAP_ID 0x211

#define Super_Capacitor_Func_GroundInit	\
		{																		\
					&Super_Capacitor_Get_Data,		\
							&Check_Super_Capacitor,		\
		}

typedef struct
{
	float Input_Voltage;
	float Capacitor_Voltage;
	float Input_Current;
	float Actual_Power;
	
	uint16_t Info_Update_Frame;
	uint8_t Offline_Flag;
}Super_Capacitor_t;

typedef struct
{
	void (*Super_Capacitor_Get_Data)(CAN_Export_Data_t RxMessage);
	void (*Check_Super_Capacitor)(void);
}Super_Capacitor_Func_t;

extern Super_Capacitor_t Super_Capacitor;
extern Super_Capacitor_Func_t Super_Capacitor_Func;