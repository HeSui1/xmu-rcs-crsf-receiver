#include "bsp_usart.h"
#include "usart.h"


#include "string.h"  // for strlen



int send_finish_count = 0;



void send_message(void)
{
    char msg[] = "Hello USART10\r\n";
    HAL_UART_Transmit(&huart7, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}



//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart == &huart7)
//    {

//				send_finish_count ++;
//        HAL_UART_Receive_DMA(&huart7, uart7_rx_buf, sizeof(uart7_rx_buf));
//			
//			
//    }
//}
