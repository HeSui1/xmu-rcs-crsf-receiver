#ifndef __RM_MOTOR_H__
#define __RM_MOTOR_H__
#include "main.h"
#include "fdcan.h"
#include "bsp_can.h"
#include "motor_fbpara.h"
#include <stdlib.h>

#define ENCODER_COUNT_TO_DEGREE	  0.0439453125 //360/8192
#define MOTOR_RPM_TO_SPEED          0.00290888208665721596153948461415f
#define MOTOR_ECD_TO_ANGLE          0.000021305288720633905968306772076277f
#define FULL_COUNT                  18
#define HALF_ECD_RANGE              4096
#define ECD_RANGE                   8191
#define M_PI 3.14159f



typedef struct
{
	uint16_t mode;
	motor_fbpara_t para;
}RM_Motor_t ;

		
		
extern volatile RM_Motor_t rm_motor[4];		
		
		
		
extern void rm2006_fbdata(volatile RM_Motor_t *motor, uint8_t *rx_data,uint32_t id);		
		

 
#endif
