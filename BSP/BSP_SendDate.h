#ifndef _BSP_SENDDATE_H_
#define _BSP_SENDDATE_H_


#include "main.h"


#define REMOTE_SOF 0XA5
// CAN角度ID
#define REMOTE_ANGLE_HUAN 0X555
// CAN 速度设置ID
#define REMOTE_SPEED_SET 0X556
// CAN 功率限制ID
#define REMOTE_POWER_LIMTED 0X557


// 8
typedef struct __attribute__((packed))
{
    u8 sof;
    float angle;        // 云台相对角度
    u16 huan;           // 缓冲功率
    u8 mode_set;        // 模式设置
}Remote_angle_t;



// 8
typedef struct __attribute__((packed))
{
    u8 sof;
    u16 power_limted;        // 功率限制
    u8 K[5];                 // 预留位
}Remote_power_limted_t;


// 8
typedef struct __attribute__((packed))
{
    u8 sof;
    s16 vx_set;           // vx速度设置
    s16 vy_set;           // vy速度设置
    s16 wz_set;
    u8 mode_set;        // 模式设置
}Remote_speed_t;


// 3
typedef struct __attribute__((packed))
{
    u8 sof;
    u16 cmd_id;
}to_chassis_Sof_t;


void SendDate_TO_chassis(u8* date,u16 ID,u16 len);
void SendDate_loading_angle(Remote_angle_t* date);
void SendDate_loading_speed(Remote_speed_t* date);
void SendDate_loading_power(Remote_power_limted_t* date);


#endif
