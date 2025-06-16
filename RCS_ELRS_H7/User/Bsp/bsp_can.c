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

	/* 配置标准帧过滤器 */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;//扩展ID不接收
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00000000;
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
		Error_Handler();
  }		
  /* 配置扩展帧过滤器 */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;         // 扩展帧
  sFilterConfig.FilterIndex = 1;                    // 使用第二个过滤器
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00000000;             // 允许所有扩展ID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }		
/* 全局过滤设置 */
/* 接收到消息ID与标准ID过滤不匹配，不接受 */
/* 接收到消息ID与扩展ID过滤不匹配，不接受 */
/* 过滤标准ID远程帧 */ 
/* 过滤扩展ID远程帧 */ 
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, 
																		FDCAN_REJECT, 
																		FDCAN_REJECT, 
																		FDCAN_FILTER_REMOTE, 
																		FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }
	 
/* 开启RX FIFO0的新数据中断 */
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }		
	 
  /* 启动 FDCAN 模块 */
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
  /* 配置扩展帧过滤器 */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;         // 扩展帧
  sFilterConfig.FilterIndex = 3;                    // 使用第四个过滤器
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
  sFilterConfig.FilterID1 = 0x00000000;             // 允许所有扩展ID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
/* 全局过滤设置 */
/* 接收到消息ID与标准ID过滤不匹配，不接受 */
/* 接收到消息ID与扩展ID过滤不匹配，不接受 */
/* 过滤标准ID远程帧 */ 
/* 过滤扩展ID远程帧 */ 
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, 
																		FDCAN_REJECT, 
																		FDCAN_REJECT, 
																		FDCAN_FILTER_REMOTE, 
																		FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }

  /* 开启 RX FIFO1 新数据中断 */
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }
	
  /* 启动 FDCAN 模块 */
  if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
}

void FDCAN3_Config(void)
{
  FDCAN_FilterTypeDef sFilterConfig;
  
  /* 配置标准帧过滤器 */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;          // 只接收标准帧
  sFilterConfig.FilterIndex = 4;                      // 使用过滤器索引4
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;       // 掩码过滤模式
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // 数据进入 RX FIFO0
  sFilterConfig.FilterID1 = 0x00000000;               // 允许所有标准ID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 配置扩展帧过滤器 */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;           // 接收扩展帧
  sFilterConfig.FilterIndex = 5;                      // 使用过滤器索引5
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00000000;               // 允许所有扩展ID
  sFilterConfig.FilterID2 = 0x00000000;
  if(HAL_FDCAN_ConfigFilter(&hfdcan3, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
/* 全局过滤设置 */
/* 接收到消息ID与标准ID过滤不匹配，不接受 */
/* 接收到消息ID与扩展ID过滤不匹配，不接受 */
/* 过滤标准ID远程帧 */ 
/* 过滤扩展ID远程帧 */ 
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan3,
                                   FDCAN_REJECT,  // 拒绝不匹配标准ID的消息
                                   FDCAN_REJECT,  // 拒绝不匹配扩展ID的消息
                                   FDCAN_FILTER_REMOTE,  // 过滤标准ID远程帧
                                   FDCAN_FILTER_REMOTE) != HAL_OK) // 过滤扩展ID远程帧
  {
    Error_Handler();
  }
  
  /* 开启 RX FIFO0 新数据中断 */
  if (HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 启动 FDCAN 模块 */
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
	  TxHeader.DataLength = len<<16;     // 发送长度：8byte
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
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;//比特率切换关闭，不适用于经典CAN
  TxHeader.FDFormat =  FDCAN_CLASSIC_CAN;            // CANFD
  TxHeader.TxEventFifoControl =  FDCAN_NO_TX_EVENTS;  
  TxHeader.MessageMarker = 0;//消息标记

   // 发送CAN指令
//  if(HAL_FDCAN_AddMessageToTxFifoQ(hcan, &TxHeader, data) != HAL_OK)
//  {
//        // 发送失败处理
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
			memset(g_Can1RxData, 0, sizeof(g_Can1RxData));	//接收前先清空数组	
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









