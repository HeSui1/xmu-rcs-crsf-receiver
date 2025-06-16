#ifndef __MOTOR_TASK_H__
#define __MOTOR_TASK_H__

#include "main.h"
#include "dm4310_drv.h"
#include "rm_motor_drv.h"
#include "pid.h"
#include "VMC_calc.h"
#include "INS_task.h"


#define PID_KP 8.3f
#define PID_KI 0.25f 
#define PID_KD 0.38f
#define PID_MAX_OUT  16000
#define PID_MAX_IOUT 10000


extern void MOTOR_task(void);
extern volatile RM_Motor_t rm_motor[4];

typedef struct {

volatile RM_Motor_t *rm_motor_ptrs[4];

  // 针对不同电机类型的扩展参数，可以用 union 实现
   union {
        struct {
            // 例如，CAN 控制关节电机可能有特定的通信状态或校准参数
            // uint16_t can_specific_param;
        } can_joint;
        struct {
            // RS485 控制关节电机的特有参数
            // uint16_t rs485_specific_param;
        } rs485_joint;
        struct {
            // 电调无刷电机可能需要记录电调状态、电压、电流等
            // float esc_voltage;
            // float esc_current;
        } esc;
        struct {
            // 针对RM3508可能的特殊校准参数
            // float rm3508_calibration;
        } rm3508;
        struct {
            // 针对2006电机的特殊需求
            // float motor2006_extra;
        } motor2006;
    } specific;
} Motor_t;

extern Motor_t motor_test;




#endif
