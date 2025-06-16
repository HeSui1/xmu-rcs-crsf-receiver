#include "rm_motor_drv.h"


#include "arm_math.h"


volatile RM_Motor_t rm_motor[4] = {0};



/**
************************************************************************
* @brief:      	uint_to_float: 无符号整数转换为浮点数函数
* @param[in]:   x_int: 待转换的无符号整数
* @param[in]:   x_min: 范围最小值
* @param[in]:   x_max: 范围最大值
* @param[in]:   bits:  无符号整数的位数
* @retval:     	浮点数结果
* @details:    	将给定的无符号整数 x_int 在指定范围 [x_min, x_max] 内进行线性映射，映射结果为一个浮点数
************************************************************************
**/




static float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	/* converts unsigned int to float, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

static float Hex_To_Float(uint32_t *Byte,int num)//十六进制到浮点数
{
	return *((float*)Byte);
}

static uint32_t FloatTohex(float HEX)//浮点数到十六进制转换
{
	return *( uint32_t *)&HEX;
}

//void RM_motor_init(RM_Motor_t *motor,uint32_t id)
//{
//	motor->para.id = id;
//	motor->para.type = MOTOR_TYPE_3508_2006;
//	motor->para.ecd_int = 0;
//	motor->para.start_angle = 0;
//	motor->para.cycles =0;
//	motor->para.int_angle = 0;
//	motor->para.float_angle = 0;
//	motor->para.v_rpm = 0;
//	motor->para.v_int = 0;
//	
//	HAL_Delay(10);
//	
//	motor->para.start_angle = motor->para.float_angle;
//	
//}
void RM_motor_init()
{
    for(int i = 0; i < 4; i++)  // 遍历数组的四个结构体
    {
        rm_motor[i].para.id = i + 1;  // 假设 id 依次递增
        rm_motor[i].para.type = MOTOR_TYPE_3508_2006;
        rm_motor[i].para.ecd_int = 0;
        rm_motor[i].para.start_angle = 0;
        rm_motor[i].para.cycles = 0;
        rm_motor[i].para.int_angle = 0;
        rm_motor[i].para.float_angle = 0;
        rm_motor[i].para.v_rpm = 0;
        rm_motor[i].para.v_int = 0;

        HAL_Delay(10);

        rm_motor[i].para.start_angle = rm_motor[i].para.float_angle;  // 初始化起始角度
    }
}


void rm2006_fbdata(volatile RM_Motor_t *motor, uint8_t *rx_data,uint32_t id)
{ 
	if ((id & 0x000f) == motor->para.id)
	{ 
// 解析编码器数据	
    motor->para.ecd_int = (uint16_t)(rx_data[0] << 8 | rx_data[1]);            
    motor->para.v_int = (uint16_t)(rx_data[2] << 8 | rx_data[3]);      
    motor->para.given_current = (uint16_t)(rx_data[4] << 8 | rx_data[5]);  
    motor->para.temperate = rx_data[6];         
	
// 解析速度数据
	
		if (motor->para.v_int <= 3 && motor->para.v_int >= -3)
			motor->para.v_rpm = 0;
		else
			motor->para.v_rpm = motor->para.v_int;
// 解析编码器角度数据
		
		//跨0度线判定

		if (motor->para.ecd_int - motor->para.last_ecd > HALF_ECD_RANGE)
    {
        motor->para.cycles++;
    }
    else if (motor->para.ecd_int - motor->para.last_ecd < -HALF_ECD_RANGE)
    {
        motor->para.cycles --;
    }
		
		
// 更新上一次的角度		
    motor->para.last_ecd = motor->para.ecd_int;
// 计算实际角度（float_angle）
// 实际角度 = 完整转圈角度 + 当前角度转换后 - 校准零点(start_angle)
	  motor->para.float_angle = 360.0 * motor->para.cycles + motor->para.ecd_int * ENCODER_COUNT_TO_DEGREE - 	motor->para.start_angle ;
// 将浮点角度转换为整数形式
	  motor->para.int_angle = (int32_t)motor->para.float_angle;
		
	}
}

void rm_motor_send(hcan_t* hcan,RM_Motor_t *motor)
{

	uint8_t data[8];
  uint16_t id = 0x200;       

    data[0] = (uint8_t)(motor[0].para.v_set_rpm >> 8);
    data[1] = (uint8_t)(motor[0].para.v_set_rpm & 0xFF);
    
    data[2] = (uint8_t)(motor[1].para.v_set_rpm >> 8);
    data[3] = (uint8_t)(motor[1].para.v_set_rpm & 0xFF);
    
    data[4] = (uint8_t)(motor[2].para.v_set_rpm >> 8);
    data[5] = (uint8_t)(motor[2].para.v_set_rpm & 0xFF);
    
    data[6] = (uint8_t)(motor[3].para.v_set_rpm >> 8);
    data[7] = (uint8_t)(motor[3].para.v_set_rpm & 0xFF);
	
	canx_send_data(hcan, id, data, 8);
	
}


void rm_motor_speed_ctrl(hcan_t* hcan,RM_Motor_t *motor,float v_rads)
{
	motor->para.v_set_rads = v_rads;
	uint16_t v_set_rpm = (uint16_t)((v_rads * 36 * 60 / (2 * M_PI)) * 256.0f / (2 * M_PI) + 0.5f);
	motor->para.v_set_rpm = v_set_rpm;
}



