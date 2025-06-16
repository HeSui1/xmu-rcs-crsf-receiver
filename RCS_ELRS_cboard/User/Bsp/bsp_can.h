#ifndef _BSP_CAN_H
#define _BSP_CAN_H

#include "main.h"

typedef FDCAN_HandleTypeDef hcan_t; 

extern void FDCAN1_Config(void);
extern void FDCAN2_Config(void);
extern void FDCAN3_Config(void);
extern uint8_t canx_send_data(FDCAN_HandleTypeDef *hcan, uint32_t id, uint8_t *data, uint32_t len);

#endif 
