#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_


#include "main.h"



void can_bsp_init(void);
uint32_t fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint8_t *buf);
u8 fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len);


#endif
