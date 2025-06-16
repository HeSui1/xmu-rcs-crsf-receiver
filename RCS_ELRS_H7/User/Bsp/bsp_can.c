#include "bsp_can.h"
#include "fdcan.h"
#include "dm4310_drv.h"
#include "rm_motor_drv.h"
#include "string.h"
#include "chassisR_task.h"
#include "motor_task.h"

FDCAN_RxHeaderTypeDef RxHeader1;
uint8_t g_Can1RxData[64];

FDCAN_RxHeaderTypeDef RxHeader2;
uint8_t g_Can2RxData[64];

void FDCAN1_Config(void)
{
  FDCAN_FilterTypeDef sFilterConfig;

	/* ���ñ�׼֡������ */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;//��չID������
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00000000;
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
		Error_Handler();
  }		
  /* ������չ֡������ */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;         // ��չ֡
  sFilterConfig.FilterIndex = 1;                    // ʹ�õڶ���������
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00000000;             // ����������չID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }		
/* ȫ�ֹ������� */
/* ���յ���ϢID���׼ID���˲�ƥ�䣬������ */
/* ���յ���ϢID����չID���˲�ƥ�䣬������ */
/* ���˱�׼IDԶ��֡ */ 
/* ������չIDԶ��֡ */ 
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, 
																		FDCAN_REJECT, 
																		FDCAN_REJECT, 
																		FDCAN_FILTER_REMOTE, 
																		FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }
	 
/* ����RX FIFO0���������ж� */
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }		
	 
  /* ���� FDCAN ģ�� */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }		 
}
void FDCAN2_Config(void)
{
  FDCAN_FilterTypeDef sFilterConfig;
  /* Configure Rx filter */
  sFilterConfig.IdType =  FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 2;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
  sFilterConfig.FilterID1 = 0x00000000;
  sFilterConfig.FilterID2 = 0x00000000;
  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* ������չ֡������ */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;         // ��չ֡
  sFilterConfig.FilterIndex = 3;                    // ʹ�õ��ĸ�������
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
  sFilterConfig.FilterID1 = 0x00000000;             // ����������չID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
/* ȫ�ֹ������� */
/* ���յ���ϢID���׼ID���˲�ƥ�䣬������ */
/* ���յ���ϢID����չID���˲�ƥ�䣬������ */
/* ���˱�׼IDԶ��֡ */ 
/* ������չIDԶ��֡ */ 
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, 
																		FDCAN_REJECT, 
																		FDCAN_REJECT, 
																		FDCAN_FILTER_REMOTE, 
																		FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }

  /* ���� RX FIFO1 �������ж� */
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }
	
  /* ���� FDCAN ģ�� */
  if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
}

void FDCAN3_Config(void)
{
  FDCAN_FilterTypeDef sFilterConfig;
  
  /* ���ñ�׼֡������ */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;          // ֻ���ձ�׼֡
  sFilterConfig.FilterIndex = 4;                      // ʹ�ù���������4
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;       // �������ģʽ
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // ���ݽ��� RX FIFO0
  sFilterConfig.FilterID1 = 0x00000000;               // �������б�׼ID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ������չ֡������ */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;           // ������չ֡
  sFilterConfig.FilterIndex = 5;                      // ʹ�ù���������5
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00000000;               // ����������չID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
/* ȫ�ֹ������� */
/* ���յ���ϢID���׼ID���˲�ƥ�䣬������ */
/* ���յ���ϢID����չID���˲�ƥ�䣬������ */
/* ���˱�׼IDԶ��֡ */ 
/* ������չIDԶ��֡ */ 
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan3,
                                   FDCAN_REJECT,  // �ܾ���ƥ���׼ID����Ϣ
                                   FDCAN_REJECT,  // �ܾ���ƥ����չID����Ϣ
                                   FDCAN_FILTER_REMOTE,  // ���˱�׼IDԶ��֡
                                   FDCAN_FILTER_REMOTE) != HAL_OK) // ������չIDԶ��֡
  {
    Error_Handler();
  }
  
  /* ���� RX FIFO0 �������ж� */
  if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ���� FDCAN ģ�� */
  if (HAL_FDCAN_Start(&hfdcan3) != HAL_OK)
  {
    Error_Handler();
  }
}



uint8_t canx_send_data(FDCAN_HandleTypeDef *hcan, uint32_t id, uint8_t *data, uint32_t len)
{
	FDCAN_TxHeaderTypeDef TxHeader;

	TxHeader.Identifier = id;                 // CAN ID
  TxHeader.IdType =  FDCAN_STANDARD_ID ;        
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;  
  if(len<=8)	
	{
	  TxHeader.DataLength = len<<16;     // ���ͳ��ȣ�8byte
	}
	else  if(len==12)	
	{
	   TxHeader.DataLength =FDCAN_DLC_BYTES_12;
	}
	else  if(len==16)	
	{
	  TxHeader.DataLength =FDCAN_DLC_BYTES_16;
	
	}
  else  if(len==20)
	{
		TxHeader.DataLength =FDCAN_DLC_BYTES_20;
	}		
	else  if(len==24)	
	{
	 TxHeader.DataLength =FDCAN_DLC_BYTES_24;	
	}else  if(len==48)
	{
	 TxHeader.DataLength =FDCAN_DLC_BYTES_48;
	}else  if(len==64)
   {
		 TxHeader.DataLength =FDCAN_DLC_BYTES_64;
	 }
											
	TxHeader.ErrorStateIndicator =  FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;//�������л��رգ��������ھ���CAN
  TxHeader.FDFormat =  FDCAN_CLASSIC_CAN;            // CANFD
  TxHeader.TxEventFifoControl =  FDCAN_NO_TX_EVENTS;  
  TxHeader.MessageMarker = 0;//��Ϣ���

   // ����CANָ��
//  if(HAL_FDCAN_AddMessageToTxFifoQ(hcan, &TxHeader, data) != HAL_OK)
//  {
//        // ����ʧ�ܴ���
//       Error_Handler();      
//  }
	 HAL_FDCAN_AddMessageToTxFifoQ(hcan, &TxHeader, data);
	 return 0;
}


extern chassis_t chassis_move;
extern Motor_t motor_test; 


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{ 
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    if(hfdcan->Instance == FDCAN1)
    {
      /* Retrieve Rx messages from RX FIFO0 */
			memset(g_Can1RxData, 0, sizeof(g_Can1RxData));	//����ǰ���������	
      HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader1, g_Can1RxData);
			
			switch(RxHeader1.Identifier)
			{
        case 3 :dm4310_fbdata(&chassis_move.joint_motor[0], g_Can1RxData,RxHeader1.DataLength);break;
        case 4 :dm4310_fbdata(&chassis_move.joint_motor[1], g_Can1RxData,RxHeader1.DataLength);break;	         	
				case 0 :dm6215_fbdata(&chassis_move.wheel_motor[0], g_Can1RxData,RxHeader1.DataLength);break;
				case 0x201 : rm2006_fbdata(motor_test.rm_motor_ptrs[0],g_Can1RxData,0x201);break;
				case 0x202 : rm2006_fbdata(motor_test.rm_motor_ptrs[1],g_Can1RxData,0x202);break;
				case 0x203 : rm2006_fbdata(motor_test.rm_motor_ptrs[2],g_Can1RxData,0x203);break;
				case 0x204 : rm2006_fbdata(motor_test.rm_motor_ptrs[3],g_Can1RxData,0x204);break;
				default: break;
			}			
	  }
  }
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
  if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET)
  {
    if(hfdcan->Instance == FDCAN2)
    {
      /* Retrieve Rx messages from RX FIFO0 */
			memset(g_Can2RxData, 0, sizeof(g_Can2RxData));
      HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &RxHeader2, g_Can2RxData);
			switch(RxHeader2.Identifier)
			{
        case 3 :dm4310_fbdata(&chassis_move.joint_motor[2], g_Can2RxData,RxHeader2.DataLength);break;
        case 4 :dm4310_fbdata(&chassis_move.joint_motor[3], g_Can2RxData,RxHeader2.DataLength);break;	         	
				case 0 :dm6215_fbdata(&chassis_move.wheel_motor[1], g_Can2RxData,RxHeader2.DataLength);break;
				case 0x201 : rm2006_fbdata(motor_test.rm_motor_ptrs[0],g_Can2RxData,0x201);break;
				case 0x202 : rm2006_fbdata(motor_test.rm_motor_ptrs[1],g_Can2RxData,0x202);break;
				case 0x203 : rm2006_fbdata(motor_test.rm_motor_ptrs[2],g_Can2RxData,0x203);break;
				case 0x204 : rm2006_fbdata(motor_test.rm_motor_ptrs[3],g_Can2RxData,0x204);break;
				default: break;
			}	
    }
  }
}









