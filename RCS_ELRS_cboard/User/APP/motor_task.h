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

  // ��Բ�ͬ������͵���չ������������ union ʵ��
   union {
        struct {
            // ���磬CAN ���ƹؽڵ���������ض���ͨ��״̬��У׼����
            // uint16_t can_specific_param;
        } can_joint;
        struct {
            // RS485 ���ƹؽڵ�������в���
            // uint16_t rs485_specific_param;
        } rs485_joint;
        struct {
            // �����ˢ���������Ҫ��¼���״̬����ѹ��������
            // float esc_voltage;
            // float esc_current;
        } esc;
        struct {
            // ���RM3508���ܵ�����У׼����
            // float rm3508_calibration;
        } rm3508;
        struct {
            // ���2006�������������
            // float motor2006_extra;
        } motor2006;
    } specific;
} Motor_t;

extern Motor_t motor_test;




#endif
