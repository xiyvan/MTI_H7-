/************************************
 * @author 韩昂轩（Han Angxvan）
 * 创建时间：24.4.27
 * 
 *    CAN的驱动包
************************************/


#include "BSP_CAN.h"
#include "fdcan.h"
#include "BSP_motor.h"


///*****************************************************  函数声明  ***************************************///
static void CAN1_rx_callback(void);
static void CAN2_rx_callback(void);
static void CAN3_rx_callback(void);



///**************************************************** endl  *******************************************///


/// @brief CANFD初始化
void can_bsp_init(void)
{
    HAL_FDCAN_Start(&hfdcan1);                //开启FDCAN
	HAL_FDCAN_Start(&hfdcan2);
	HAL_FDCAN_Start(&hfdcan3);
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);     // 使能中断
	HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}


u8 fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
{	
	FDCAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.Identifier = id;
  TxHeader.IdType = FDCAN_STANDARD_ID;																// 标准ID 
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;														// 数据帧 
  TxHeader.DataLength = len << 16;																		// 发送数据长度 
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;										// 设置错误状态指示 								
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;															// 不开启可变波特率 
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;															// 普通CAN格式 
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;										// 用于发送事件FIFO控制, 不存储 
  TxHeader.MessageMarker = 0x00; 			// 用于复制到TX EVENT FIFO的消息Maker来识别消息状态，范围0到0xFF                
    
  if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, data)!=HAL_OK) 
		return 1;//发送
	return 0;	
}




/// @brief CAN接收函数
/// @param hfdcan can号
/// @param buf 接受缓冲数组
/// @return 接受的ID
uint32_t fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint8_t *buf)
{	
	FDCAN_RxHeaderTypeDef fdcan_RxHeader;
    if(HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0, &fdcan_RxHeader, buf)!=HAL_OK)
		return 0;//接收数据
    return fdcan_RxHeader.Identifier;	
}




/// @brief CAN接受中断函数
/// @param hfdcan 触发中断的CAN号
/// @param RxFifo0ITs 触发中断标志
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
		if(hfdcan == &hfdcan1)
		{
			CAN1_rx_callback();
		}
		if(hfdcan == &hfdcan2)
		{
			CAN2_rx_callback();
		}
		if(hfdcan == &hfdcan3)
		{
			CAN3_rx_callback();
		}
	}
}



/// @brief CAN1接受回调
static void CAN1_rx_callback(void)
{
	CAN1_motor_decode();
}



/// @brief  CAN2接受回调
static void CAN2_rx_callback(void)
{
	CAN2_motor_decode();
}



/// @brief CAN3接受回调 
static void CAN3_rx_callback(void)
{

}

