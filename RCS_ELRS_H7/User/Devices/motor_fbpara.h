#ifndef MOTOR_FBPARA_H
#define MOTOR_FBPARA_H

#include "main.h"





typedef enum {
    MOTOR_TYPE_8010,    // CAN 控制的关节电机
    MOTOR_TYPE_A1,    // RS485 控制的关节电机
    MOTOR_TYPE_VESC,          // 使用电调（例如本杰明电调）的无刷电机
    MOTOR_TYPE_3508_2006,       // 大疆 RM3508 电机
} MotorType_t;



typedef struct 
{
	
  // 基本信息：每个电机都有自己的编号和状态
  uint32_t id;         // 电机ID
  uint16_t state;      // 电机状态
	MotorType_t type;
	

	int16_t given_current; // 控制器给予的电流值（原始）
  uint8_t temperate;   // 电机温度
	

	
  int16_t v_int;           // 速度原始数据（转换前的数值）
  volatile int16_t v_rpm;   // 电机转速，单位转/分钟
	float v_mps;
	float v_rads;
  float v_set_rads;
	uint16_t v_set_rpm;
	
  int p_int;           // 位置原始数据（如经过一定换算的中间值）
  uint16_t ecd_int;        // 当前编码器计数（原始位置反馈）
  int16_t cycles;         // 跨圈计数（编码器完整转圈数）
  int16_t last_ecd;    // 上一次编码器计数，用于计算变化量
  float start_angle;      // 校准零点：初始参考角度（相当于offset_ecd）
	volatile float float_angle;  // 计算得到的实际角度（浮点形式）
  volatile int32_t int_angle;  // 实际角度的整数形式（便于整数计算）



  int t_int;           // 扭矩原始数据（转换前的数值）
  float pos;           // 实际位置（经过换算后的值，单位可能为度或弧度）
  float vel;           // 实际速度（转换后，单位可能为转/分钟或其他）
  float tor;           // 实际扭矩（经过换算后的物理量）



  // 控制参数：用于闭环控制的PID参数，既保存原始整数值，也保存经过换算的浮点值
  int kp_int;          // 比例增益的原始整数值
  int kd_int;          // 微分增益的原始整数值
  float kp;            // 换算后的比例增益
  float kd;            // 换算后的微分增益

  // 其他反馈参数，例如温度采集的更详细数据
  float Tmos;          // MOSFET温度
  float Tcoil;         // 电机线圈温度

  // 设定值或测试用参数：用于调试或作为目标控制参数
  float pos_set;       // 目标设定位置
  float vel_set;       // 目标设定速度
  float tor_set;       // 目标设定扭矩
  float kp_test;       // 测试用比例增益（上位机调试参数）
  float kd_test;       // 测试用微分增益

  // 上位机传来的数据：这些数据通常不是从电机直接反馈得到，
  // 而是通过上位机调试接口传输，用于参数校准或比较
  int kp_int_test;
  int kd_int_test;
  int p_int_test;
  int v_int_test;
  int t_int_test;
} motor_fbpara_t;


typedef struct
{
	uint16_t mode;
	motor_fbpara_t para;
}Unitree_8010_Motor_t ;

typedef struct
{
	uint16_t mode;
	motor_fbpara_t para;
}Unitree_A1_Motor_t ;


typedef struct
{
	uint16_t mode;
	motor_fbpara_t para;
}VESC_U8_Motor_t ;



#endif

