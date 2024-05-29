#ifndef _GYM_TASK_H_
#define _GYM_TASK_H_

#include "REMOTE_task.h"
#include "RM_motor.h"
#include "PID.h"
#include "imu_task.h"


// GYM的pitch轴限位角度
#define GYM_PITCH_LIMTED_MAX 3.86946f
#define GYM_PITCH_LIMTED_MIN 3.42307806f
// pitch轴的中值范围
#define GYM_PITCH_FW  ((GYM_PITCH_LIMTED_MAX - GYM_PITCH_LIMTED_MIN)/2)

//中值角度  需测定
#define GYM_PITCH_MEDIAN_ANGLE  3.47830153f
#define GYM_YAW_MEDIAN_ANGLE    2.69597125f
#define GYM_PITCH_MEDIAN_DEAD_ANGLE 0.2f  //初始化判定死区角度
#define GYM_YAW_MEDIAN_DEAD_ANGLE 0.2f    //初始化判定死区角度




///yaw轴电机速度环PID参数
#define GYM_PID_YAW_speed_KP  100.0f
#define GYM_PID_YAW_speed_KI  0.0f
#define GYM_PID_YAW_speed_KD  3.0f
#define GYM_PID_YAW_speed_MAXOUT  30000.0f
#define GYM_PID_YAW_speed_MAXIOUT  0.0f

///yaw轴电机角度环PID参数
#define GYM_PID_YAW_angle_KP  200.0f
#define GYM_PID_YAW_angle_KI  0.0f
#define GYM_PID_YAW_angle_KD  5000.0f
#define GYM_PID_YAW_angle_MAXOUT  300.0f
#define GYM_PID_YAW_angle_MAXIOUT  0.0f

///PITCH轴电机速度PID参数
#define GYM_PID_PITCH_speed_KP  300.0f
#define GYM_PID_PITCH_speed_KI  0.0f
#define GYM_PID_PITCH_speed_KD  0.0f
#define GYM_PID_PITCH_speed_MAXOUT  30000.0f
#define GYM_PID_PITCH_speed_MAXIOUT  0.0f

///PITCH轴电机角度PID参数
#define GYM_PID_PITCH_angle_KP  60.0f
#define GYM_PID_PITCH_angle_KI  0.0f
#define GYM_PID_PITCH_angle_KD  100.0f
#define GYM_PID_PITCH_angle_MAXOUT  400.0f
#define GYM_PID_PITCH_angle_MAXIOUT  0.0f

///初始化状态下的YAW轴电机速度PID
#define GYM_PID_YAW_speed_init_KP  40.0f
#define GYM_PID_YAW_speed_init_KI  1.0f
#define GYM_PID_YAW_speed_init_KD  0.1f
#define GYM_PID_YAW_speed_init_MAXOUT  10000.0f
#define GYM_PID_YAW_speed_init_MAXIOUT  2000.0f

///初始化状态下的PITCH轴电机速度PID
#define GYM_PID_PITCH_speed_init_KP  130.0f
#define GYM_PID_PITCH_speed_init_KI  1.0f
#define GYM_PID_PITCH_speed_init_KD  0.0f
#define GYM_PID_PITCH_speed_init_MAXOUT  10000.0f
#define GYM_PID_PITCH_speed_init_MAXIOUT  5000.0f


//遥控器转换为角度设定值的系数
#define GYM_YAW_REMOVE_CHANGE   6600.0f
#define GYM_PITCH_REMOVE_CHANGE   6600.0f
#define GYM_YAW_REMOVE_MOUSE 0.00015f
#define GYM_PITCH_REMOVE_MOUSE 0.00015f

enum
{
    GYM_MODE_CHASSIS_FOLLOW_GYM,            /*底盘跟随云台模式*/
    GYM_MODE_CHASSIS_FOLLOW_CHASSIS,        /*地盘跟随底盘模式*/
    GYM_MODE_ZERO,                          /*零电流模式*/
    GYM_MODE_CHASSIS_NO_FOLLOW_GYM,         /*不跟随云台模式*/
    GYM_OPEN_LOOP,                          /*底盘开环控制模式*/
    GYM_revolve_mode,                       /*底陀螺模式*/
    GYM_MODE_INIT_N,                        /*云台非初始化模式*/
    GYM_MODE_INIT,                          /*云台初始化模式*/
};


typedef struct 
{
    float pitch_angle;
    float yaw_angle;
    float current_set[2];       //云台两个电机的电流赋值
}GYM_CTRL_SET;


typedef struct 
{
    float *YawAngle;
    float *PitchAngle;
    s32* Yaw_qvan;
    s32* Pitch_qvan;
    float yaw_all_angle;
    float pitch_all_angle;
}Gym_ins_struct_t;



typedef struct 
{
    GYM_CTRL_SET gym_set;
    float save_mid_pitch;                 /* 保存pitch中值 */
    Remote_type_t* remote_msg;      /*遥控器数据指针*/
    
    u8 mode;            /*模式设置*/
    u8 mode_init;
    u8 last_mode;       /*上一次的模式是什么*/
    motor_return_msg_t motor_msg_6020[2];   /*6020两个电机的消息-----yaw----pitch*/

    PID_type_def yaw_speed_pid;
    PID_type_def yaw_angle_pid;
    PID_type_def pitch_speed_pid;
    PID_type_def pitch_angle_pid;
    PID_type_def yaw_pitch_init_pid[2]; /*yaw pitch 轴初始化PID*/

    Gym_ins_struct_t Ins_msg;       /* 陀螺仪信息*/

}GYM_MSG_t;



void Gym_task(void *pvParameters);

#endif
