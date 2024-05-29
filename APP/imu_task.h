#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "cmsis_os.h"
#include "main.h"

#define INS_YAW_ADDRESS_OFFSET    0
#define INS_PITCH_ADDRESS_OFFSET  1
#define INS_ROLL_ADDRESS_OFFSET   2

#define IMU_TASK_PI 3.1415926f
#define IMU_TASK_2_PI 6.2831853f


/// @brief 姿态解算任务总信息
typedef struct
{
    // 保存各个轴的角度
    float YAW_angle;
    float Pitch_angle;
    float Roll_angle;
    // 保存各个轴旋转的圈数
    s32 yaw_qvan;
    s32 pitch_qvan;
    s32 roll_qvan; 
}imu_date_struct_t;


#endif
