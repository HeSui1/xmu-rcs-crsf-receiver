#include "motor_task.h"


Motor_t motor_test;


void MotorTest_Init(void)
{
    for (int i = 0; i < 4; i++) {
        motor_test.rm_motor_ptrs[i] = &rm_motor[i];
    }
}


void MOTOR_task ()
{
	
	
}


