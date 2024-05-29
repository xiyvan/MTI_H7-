/***********************************************************
 * 创建时间 ： 24.4.21
 * @author 韩昂轩（Han Angxvan）
 * 
 *     与底盘通信功能包
 * 
 ************************************************************
*/



#include "BSP_SendDate.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"


Remote_power_limted_t remote_power_limted;



/// @brief CAN发送函数
/// @param CAN can的序号 CAN1 OR CAN2
/// @param date 数据
/// @param ID 发送ID
void SendDate_TO_chassis(u8* date,u16 ID,u16 len)
{
    u8 tx_date[20];
    to_chassis_Sof_t sof;
    sof.sof = REMOTE_SOF;
    sof.cmd_id = ID;
    memcpy(tx_date,&sof,sizeof(sof));
    memcpy(tx_date+sizeof(sof),date,len);
    tx_date[sizeof(sof)+len] = 0xff;           // 帧尾1
    tx_date[sizeof(sof)+len+1] = 0xaa;           // 帧尾2
    // 串口发送
    HAL_UART_Transmit(&huart7,tx_date,sizeof(sof) + len + 2,0xffff);
}


/// @brief 角度装载并发送
/// @param date 角度设定结构体变量
void SendDate_loading_angle(Remote_angle_t* date)
{
    u8 tx_date[8];
    memcpy(tx_date,date,sizeof(Remote_angle_t));
    SendDate_TO_chassis(tx_date,REMOTE_ANGLE_HUAN,8);
}


/// @brief 底盘速度设定发送函数
/// @param date 速度设定结构体变量
void SendDate_loading_speed(Remote_speed_t* date)
{
    u8 tx_date[8];
    memcpy(tx_date,date,sizeof(Remote_speed_t));
    SendDate_TO_chassis(tx_date,REMOTE_SPEED_SET,8);
}


/// @brief 底盘功率限制发送函数
/// @param date 底盘功率限制信息结构体变量
void SendDate_loading_power(Remote_power_limted_t* date)
{
    u8 tx_date[8];
    memcpy(tx_date,date,sizeof(Remote_power_limted_t));
    SendDate_TO_chassis(tx_date,REMOTE_POWER_LIMTED,8);
}
