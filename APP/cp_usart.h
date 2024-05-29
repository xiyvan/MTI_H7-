#ifndef _CP_USART_H_
#define _CP_USART_H_



#include "main.h"



#define USART6_DMA_RX_BUFF_SIZE 1024      //串口一DMA接收缓冲大小
#define USART6_DMA_RX_DATA_LEN 1024       /*串口1DMA接收数据长度设置  要小于接收缓冲大小*/


void CP_usart_task(u16 len,u8* date);


#endif
