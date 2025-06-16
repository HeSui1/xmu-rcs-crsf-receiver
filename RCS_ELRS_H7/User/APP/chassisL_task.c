/**
  *********************************************************************
  * @file      chassisL_task.c/h
  * @brief     �����������벿�ֵĵ�����ֱ�������DM4310��һ��DM6215�����������������can2������
	*						 �ӵ��������¿������Ͻǵ�DM4310����idΪ8������idΪ4��
	*						 ���½ǵ�DM4310����idΪ6������idΪ3��
	*						 ���DM��챵������idΪ1������idΪ0��
  * @note       
  * @history
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *********************************************************************
  */
	
#include "chassisL_task.h"
#include "fdcan.h"
#include "VMC_calc.h"

#include "INS_task.h"
#include "cmsis_os.h"
#include "pid.h"

/**
  *********************************************************************
  * @file      chassisL_task.c/h
  * @brief     �����������벿�ֵĵ�����ֱ�������DM4310��һ��DM6215�����������������can2������
	*						 �ӵ��������¿������Ͻǵ�DM4310����idΪ8������idΪ4��
	*						 ���½ǵ�DM4310����idΪ6������idΪ3��
	*						 ���DM��챵������idΪ1������idΪ0��
  * @note       
  * @history
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *********************************************************************
  */
	
#include "chassisL_task.h"
#include "fdcan.h"
#include "VMC_calc.h"

#include "INS_task.h"
#include "cmsis_os.h"
#include "pid.h"

vmc_leg_t left;

//float LQR_K_L[12]={  -1.5171 ,  -0.1347  , -2.4105,   -0.9858 ,   0.8685  ,  0.0783,
//    1.2392 ,   0.1251 ,   2.9650 ,   1.0868,   10.7689 ,   0.5026};

//Q=diag([20 0.1 80 110 700 1]);
//R=[90 0;0 4]; 
float LQR_K_L[12]={ 
   -2.1954,   -0.2044  , -0.8826,   -1.3245,    1.2784  ,  0.1112,
    2.5538,   0.2718  ,  1.5728  ,  2.2893  , 12.1973 ,   0.4578};

//float LQR_K_L[12]={     -2.1340  , -0.2028 ,  -0.8838 ,  -1.3155 ,   1.2601 ,   0.1103,
//    2.3696  , 0.2669  ,  1.5572 ,   2.2473  , 12.2365   , 0.4599};
extern float Poly_Coefficient[12][4];

extern chassis_t chassis_move;
		
PidTypeDef LegL_Pid;
extern INS_t INS;

uint32_t CHASSL_TIME=1;				
void ChassisL_task(void)
{
  while(INS.ins_flag==0)
	{//�ȴ����ٶ�����
	  osDelay(1);	
	}	
  ChassisL_init(&chassis_move,&left,&LegL_Pid);//��ʼ����������ؽڵ���������챵����id�Ϳ���ģʽ����ʼ���Ȳ�
	
	while(1)
	{	
		chassisL_feedback_update(&chassis_move,&left,&INS);//��������
		
		chassisL_control_loop(&chassis_move,&left,&INS,LQR_K_L,&LegL_Pid);//���Ƽ���
   		
    if(chassis_move.start_flag==1)	
		{
			mit_ctrl(&hfdcan2,chassis_move.joint_motor[3].para.id, 0.0f, 0.0f,0.0f, 0.0f,left.torque_set[1]);//left.torque_set[1]
			osDelay(CHASSL_TIME);
			mit_ctrl(&hfdcan2,chassis_move.joint_motor[2].para.id, 0.0f, 0.0f,0.0f, 0.0f,left.torque_set[0]);
			osDelay(CHASSL_TIME);
			mit_ctrl2(&hfdcan2,chassis_move.wheel_motor[1].para.id, 0.0f, 0.0f,0.0f, 0.0f,chassis_move.wheel_motor[1].wheel_T);//��߱���챵��
			osDelay(CHASSL_TIME);
		}
		else if(chassis_move.start_flag==0)	
		{
		  mit_ctrl(&hfdcan2,chassis_move.joint_motor[3].para.id, 0.0f, 0.0f,0.0f, 0.0f,0.0f);//left.torque_set[1]
			osDelay(CHASSL_TIME);
			mit_ctrl(&hfdcan2,chassis_move.joint_motor[2].para.id, 0.0f, 0.0f,0.0f, 0.0f,0.0f);
			osDelay(CHASSL_TIME);
			mit_ctrl2(&hfdcan2,chassis_move.wheel_motor[1].para.id, 0.0f, 0.0f,0.0f, 0.0f,0.0f);//�����챵��	
			osDelay(CHASSL_TIME);
		}
	}
}

void ChassisL_init(chassis_t *chassis,vmc_leg_t *vmc,PidTypeDef *legl)
{
  const static float legl_pid[3] = {LEG_PID_KP, LEG_PID_KI,LEG_PID_KD};

	joint_motor_init(&chassis->joint_motor[2],6,MIT_MODE);//����idΪ6
	joint_motor_init(&chassis->joint_motor[3],8,MIT_MODE);//����idΪ8
	
	wheel_motor_init(&chassis->wheel_motor[1],1,MIT_MODE);//����idΪ1
	
	VMC_init(vmc);//���˳���ֵ
	
	PID_init(legl, PID_POSITION,legl_pid, LEG_PID_MAX_OUT, LEG_PID_MAX_IOUT);//�ȳ�pid

	for(int j=0;j<10;j++)
	{
	  enable_motor_mode(&hfdcan2,chassis->joint_motor[3].para.id,chassis->joint_motor[3].mode);
	  osDelay(1);
	}
	for(int j=0;j<10;j++)
	{
	  enable_motor_mode(&hfdcan2,chassis->joint_motor[2].para.id,chassis->joint_motor[2].mode);
	  osDelay(1);
	}
	for(int j=0;j<10;j++)
	{
    enable_motor_mode(&hfdcan2,chassis->wheel_motor[1].para.id,chassis->wheel_motor[1].mode);//�����챵��
	  osDelay(1);
	}
}

void chassisL_feedback_update(chassis_t *chassis,vmc_leg_t *vmc,INS_t *ins)
{
  vmc->phi1=pi/2.0f+chassis->joint_motor[2].para.pos;
	vmc->phi4=pi/2.0f+chassis->joint_motor[3].para.pos;
		
	chassis->myPithL=0.0f-ins->Pitch;
	chassis->myPithGyroL=0.0f-ins->Gyro[0];
	
}

extern uint8_t right_flag;
uint8_t left_flag;
void chassisL_control_loop(chassis_t *chassis,vmc_leg_t *vmcl,INS_t *ins,float *LQR_K,PidTypeDef *leg)
{
	VMC_calc_1_left(vmcl,ins,((float)CHASSL_TIME)*3.0f/1000.0f);//����theta��d_theta��lqr�ã�ͬʱҲ�������ȳ�L0,���������������3*0.001��
	
	for(int i=0;i<12;i++)
	{
		LQR_K[i]=LQR_K_calc(&Poly_Coefficient[i][0],vmcl->L0 );	
	}
			
	chassis->wheel_motor[1].wheel_T=(LQR_K[0]*(vmcl->theta-0.0f)
																	+LQR_K[1]*(vmcl->d_theta-0.0f)
																	+LQR_K[2]*(chassis->x_set-chassis->x_filter)
																	+LQR_K[3]*(chassis->v_set-chassis->v_filter)
																	+LQR_K[4]*(chassis->myPithL-0.0f)
																	+LQR_K[5]*(chassis->myPithGyroL-0.0f));
	
	//�ұ��Źؽ��������				
	vmcl->Tp=(LQR_K[6]*(vmcl->theta-0.0f)
					+LQR_K[7]*(vmcl->d_theta-0.0f)
					+LQR_K[8]*(chassis->x_set-chassis->x_filter)
					+LQR_K[9]*(chassis->v_set-chassis->v_filter)
					+LQR_K[10]*(chassis->myPithL-0.0f)
					+LQR_K[11]*(chassis->myPithGyroL-0.0f));
	 		
	chassis->wheel_motor[1].wheel_T= chassis->wheel_motor[1].wheel_T-chassis->turn_T;	//��챵���������
	mySaturate(&chassis->wheel_motor[1].wheel_T,-1.0f,1.0f);	
	
	vmcl->Tp=vmcl->Tp+chassis->leg_tp;//�Źؽ��������

//	vmcl->F0=13.0f/arm_cos_f32(vmcl->theta)+PID_Calc(leg,vmcl->L0,chassis->leg_left_set) + chassis->now_roll_set;//ǰ��+pd

	jump_loop_l(chassis,vmcl,leg); 	

	left_flag=ground_detectionL(vmcl,ins);//������ؼ��
	
	 if(chassis->recover_flag==0)	
	 {//����������Ҫ����Ƿ����
		if(left_flag==1&&right_flag==1&&vmcl->leg_flag==0)
		{//������ͬʱ��ز���ң����û���ڿ����ȵ�����ʱ������Ϊ���
			chassis->wheel_motor[1].wheel_T=0.0f;
			vmcl->Tp=LQR_K[6]*(vmcl->theta-0.0f)+ LQR_K[7]*(vmcl->d_theta-0.0f);
			
			chassis->x_filter=0.0f;//��λ������
			chassis->x_set=chassis->x_filter;
			chassis->turn_set=chassis->total_yaw;
			vmcl->Tp=vmcl->Tp+chassis->leg_tp;		
		}
		else
		{//û�����
			vmcl->leg_flag=0;//��Ϊ0			
		}
	 }
	 else if(chassis->recover_flag==1)
	 {
		 vmcl->Tp=0.0f;
	 }
	
	mySaturate(&vmcl->F0,-100.0f,100.0f);//�޷� 
	
	VMC_calc_2(vmcl);//���������Ĺؽ��������
	
  //�Ť��
  mySaturate(&vmcl->torque_set[1],-3.0f,3.0f);	
	mySaturate(&vmcl->torque_set[0],-3.0f,3.0f);	
			
}
void jump_loop_l(chassis_t *chassis,vmc_leg_t *vmcl,PidTypeDef *leg)
{
	if(chassis->jump_flag == 1)
	{
		if(chassis->jump_status_l == 0)
		{
			vmcl->F0= Mg/arm_cos_f32(vmcl->theta) + PID_Calc(leg,vmcl->L0,0.07f) ;//ǰ��+pd
			if(vmcl->L0<0.1f)
			{
				chassis->jump_time_l++;
			}
		}
		else if(chassis->jump_status_l == 1)
		{
			vmcl->F0= Mg/arm_cos_f32(vmcl->theta) + PID_Calc(leg,vmcl->L0,0.4f) ;//ǰ��+pd
			if(vmcl->L0>0.16f)
			{
				chassis->jump_time_l++;
			}
		}
		else if(chassis->jump_status_l == 2)
		{
			vmcl->F0=Mg/arm_cos_f32(vmcl->theta) + PID_Calc(leg,vmcl->L0,chassis->leg_right_set) ;//ǰ��+pd
			if(vmcl->L0<(chassis->leg_right_set+0.01f))
			{
				chassis->jump_time_l++;
			}
		}
		else
		{
			vmcl->F0=Mg/arm_cos_f32(vmcl->theta) + PID_Calc(leg,vmcl->L0,chassis->leg_left_set) ;//ǰ��+pd
		}

	}
	else
	{
		vmcl->F0=Mg/arm_cos_f32(vmcl->theta) + PID_Calc(leg,vmcl->L0,chassis->leg_left_set) + chassis->now_roll_set;//ǰ��+pd
	}

}

