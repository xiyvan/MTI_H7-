
#ifndef _SHOOT_TASK_H_
#define _SHOOT_TASK_H_

#include "main.h"
#include "REMOTE_task.h"
#include "PID.h"
#include "RM_motor.h"


/// 摩擦轮速度pid
#define SHOOT_TASK_WHEEL_SPEED_PID_KP   5.0F
#define SHOOT_TASK_WHEEL_SPEED_PID_KI   0.0F
#define SHOOT_TASK_WHEEL_SPEED_PID_KD   0.1F
#define SHOOT_TASK_WHEEL_SPEDED_PID_MAX  2000.0F
#define SHOOT_TASK_WHEEL_SPEED_PID_MAXI 0.0F

/// 波胆点击角度pid
#define SHOOT_TASK_MOTOR_ANG_PID_KP     0.0F
#define SHOOT_TASK_MOTOR_ANG_PID_KI     0.0F
#define SHOOT_TASK_MOTOR_ANG_PID_KD     0.0F
#define SHOOT_TASK_MOTOR_ANG_PID_MAX    0.0F
#define SHOOT_TASK_MOTOR_ANG_PID_MAXI   0.0F

/// 波胆点击速度pid
#define SHOOT_TASK_MOTOR_SPEED_PID_KP   5.0F
#define SHOOT_TASK_MOTOR_SPEED_PID_KI   0.5F
#define SHOOT_TASK_MOTOR_SPEED_PID_KD   0.0F
#define SHOOT_TASK_MOTOR_SPEED_PID_MAX  2000.0F
#define SHOOT_TASK_MOTOR_SPEED_PID_MAXI 500.0F


#define SHOOT_TASK_SHOOT_SPE   7000    // 摩擦轮转速
#define SHOOT_TASK_ANG_SPEED 1000      // 拨盘电机转速
#define SHOOT_TASK_ANG 0.0F         // 拨盘电机角度（单发的时候）


#define SHOOT_TASK_SHOOT_DIAL_STEP 0.0F //拨盘电机一个周期步进多少度


enum
{
    SHOOT_TASK_MODE_STOP,       // 射击停止模式
    SHOOT_TASK_MODE_READY,      // 射击准备模式
    SHOOT_TASK_MODE_FIRE,       // 射击连续开火模式
    SHOOT_TASK_MODE_ONE,        // 射击单发开火模式
    SHOOT_TASK_MODE_AUTO,       // 自动射击模式
};



/// @brief 设计信息结构体
typedef struct 
{
    Remote_type_t* remove_msg;      // 遥控信息指针

    u8 mode;                        // 保存模式信息
    u16 hoot;                       // 热量数据保存

    motor_return_msg_t motor_msg[3];    // 电机信息     0 1 为摩擦轮电机 2 播单电机

    PID_type_def wheel_speed[2];    // 两个摩擦轮的转速PID
    PID_type_def motor_ang_speed[2];    // 拨弹电机的角度PID与速度PID
    u16 curre_set[3];           // 电机电流设置     0 1 为摩擦轮电机 2 为播弹电机
}shoot_type_t;



void SHOOT_task(void *pvParameters);    


#endif
