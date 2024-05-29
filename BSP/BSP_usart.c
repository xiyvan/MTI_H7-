/*****************************************
 * @author 韩昂轩 （Han Angxvan）
 * 创建时间：24.5.2
 * 
 *      串口回调分配
******************************************
*/



#include "BSP_usart.h"
#include "usart.h"
#include "REMOTE_task.h"
#include "string.h"
#include "cp_usart.h"

uint8_t usart3_rx_data_buf[REMOTE_RECEVER_LEN+5];
uint8_t uart6_rx_data_buf[USART6_DMA_RX_DATA_LEN];



/// @brief 串口接收中断
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{

	if(huart->Instance == UART5)
	{
        // 遥控器接受
		if (Size <= REMOTE_RECEVER_LEN)
		{
			HAL_UARTEx_ReceiveToIdle_DMA(&huart5,usart3_rx_data_buf, REMOTE_RECEVER_LEN); // 接收完毕后重启
			REMOTE_TASK(Size,usart3_rx_data_buf);
		}
		else  // 接收数据长度大于BUFF_SIZE，错误处理
		{
			HAL_UARTEx_ReceiveToIdle_DMA(&huart5, usart3_rx_data_buf, REMOTE_RECEVER_LEN); // 接收完毕后重启
			memset(usart3_rx_data_buf, 0, REMOTE_RECEVER_LEN);
		}
	}
    else if(huart->Instance == USART1)
    {
        // 裁判系统的接受
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart6_rx_data_buf,USART6_DMA_RX_DATA_LEN);
        CP_usart_task(Size,USART6_DMA_RX_DATA_LEN);
    }
}







void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
	if(huart->Instance == UART5)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, usart3_rx_data_buf, REMOTE_RECEVER_LEN); // 接收发生错误后重启
		memset(usart3_rx_data_buf, 0, REMOTE_RECEVER_LEN);							   // 清除接收缓存		
	}
}
