#ifndef MOTOR_FBPARA_H
#define MOTOR_FBPARA_H

#include "main.h"





typedef enum {
    MOTOR_TYPE_8010,    // CAN ���ƵĹؽڵ��
    MOTOR_TYPE_A1,    // RS485 ���ƵĹؽڵ��
    MOTOR_TYPE_VESC,          // ʹ�õ�������籾�������������ˢ���
    MOTOR_TYPE_3508_2006,       // �� RM3508 ���
} MotorType_t;



typedef struct 
{
	
  // ������Ϣ��ÿ����������Լ��ı�ź�״̬
  uint32_t id;         // ���ID
  uint16_t state;      // ���״̬
	MotorType_t type;
	

	int16_t given_current; // ����������ĵ���ֵ��ԭʼ��
  uint8_t temperate;   // ����¶�
	

	
  int16_t v_int;           // �ٶ�ԭʼ���ݣ�ת��ǰ����ֵ��
  volatile int16_t v_rpm;   // ���ת�٣���λת/����
	float v_mps;
	float v_rads;
  float v_set_rads;
	uint16_t v_set_rpm;
	
  int p_int;           // λ��ԭʼ���ݣ��羭��һ��������м�ֵ��
  uint16_t ecd_int;        // ��ǰ������������ԭʼλ�÷�����
  int16_t cycles;         // ��Ȧ����������������תȦ����
  int16_t last_ecd;    // ��һ�α��������������ڼ���仯��
  float start_angle;      // У׼��㣺��ʼ�ο��Ƕȣ��൱��offset_ecd��
	volatile float float_angle;  // ����õ���ʵ�ʽǶȣ�������ʽ��
  volatile int32_t int_angle;  // ʵ�ʽǶȵ�������ʽ�������������㣩



  int t_int;           // Ť��ԭʼ���ݣ�ת��ǰ����ֵ��
  float pos;           // ʵ��λ�ã�����������ֵ����λ����Ϊ�Ȼ򻡶ȣ�
  float vel;           // ʵ���ٶȣ�ת���󣬵�λ����Ϊת/���ӻ�������
  float tor;           // ʵ��Ť�أ�������������������



  // ���Ʋ��������ڱջ����Ƶ�PID�������ȱ���ԭʼ����ֵ��Ҳ���澭������ĸ���ֵ
  int kp_int;          // ���������ԭʼ����ֵ
  int kd_int;          // ΢�������ԭʼ����ֵ
  float kp;            // �����ı�������
  float kd;            // ������΢������

  // �������������������¶Ȳɼ��ĸ���ϸ����
  float Tmos;          // MOSFET�¶�
  float Tcoil;         // �����Ȧ�¶�

  // �趨ֵ������ò��������ڵ��Ի���ΪĿ����Ʋ���
  float pos_set;       // Ŀ���趨λ��
  float vel_set;       // Ŀ���趨�ٶ�
  float tor_set;       // Ŀ���趨Ť��
  float kp_test;       // �����ñ������棨��λ�����Բ�����
  float kd_test;       // ������΢������

  // ��λ�����������ݣ���Щ����ͨ�����Ǵӵ��ֱ�ӷ����õ���
  // ����ͨ����λ�����Խӿڴ��䣬���ڲ���У׼��Ƚ�
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

