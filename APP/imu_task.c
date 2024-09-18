/**************************************************
 * @author 达妙
 * 移植 ： 韩昂轩
 * 创建时间：24.5.1
 * 
 * @note 移植的达妙例程中的解算算法   自己添加了记圈算法
 * 
 *          AHRS姿态计算任务
***************************************************/


#include "imu_task.h"
#include "bmi088driver.h"
#include "MahonyAHRS.h"
#include "tim.h"
#include <math.h>

#define DES_TEMP    40.0f
#define KP          100.f
#define KI          50.f
#define KD          10.f
#define MAX_OUT     500


///*****************************************************  变量定义  **************************************************///

// 解算出来的信息保存
imu_date_struct_t imu_date_main;


float gyro[3] = {0.0f};
float acc[3] = {0.0f};
// 用来保存上一次的角度数据
float last_INS_angle[3] = {0.0f, 0.0f, 0.0f};
static float temp = 0.0f;

float imuQuat[4] = {0.0f};
float imuAngle[3] = {0.0f};

float out = 0;
float err = 0;
float err_l = 0;
float err_ll = 0;

//*************************************************** 函数定义  *********************************************///

void AHRS_init(float quat[4])
{
    quat[0] = 1.0f;
    quat[1] = 0.0f;
    quat[2] = 0.0f;
    quat[3] = 0.0f;

}

void AHRS_update(float quat[4], float gyro[3], float accel[3])
{
    MahonyAHRSupdateIMU(quat, gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2]);
}

void GetAngle(float q[4], float *yaw, float *pitch, float *roll)
{
    *yaw = atan2f(2.0f*(q[0]*q[3]+q[1]*q[2]), 2.0f*(q[0]*q[0]+q[1]*q[1])-1.0f);
    *pitch = asinf(-2.0f*(q[1]*q[3]-q[0]*q[2]));
    *roll = atan2f(2.0f*(q[0]*q[1]+q[2]*q[3]),2.0f*(q[0]*q[0]+q[3]*q[3])-1.0f);
}



///************************************************* 任务开始  ******************************************///


void ins_task(void const * argument)
{
    osDelay(10);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    while(BMI088_init())
    {
        osDelay(100);
    }
    
    AHRS_init(imuQuat);
    for(;;)
    {
//************************************************************ 角度解析  ***************************************************************************///

        // 读取陀螺仪与加速度计数据
        BMI088_read(gyro, acc, &temp);  

        // 更新四元数
        AHRS_update(imuQuat, gyro, acc);

        // 通过四元数解算欧拉角
        GetAngle(imuQuat, imuAngle + INS_YAW_ADDRESS_OFFSET, imuAngle + INS_PITCH_ADDRESS_OFFSET, imuAngle + INS_ROLL_ADDRESS_OFFSET);

/// ***********************************************  温度控制  ***********************************************************************************///

        err_ll = err_l;
        err_l = err;
        err = DES_TEMP - temp;
        out = KP*err + KI*(err + err_l + err_ll) + KD*(err - err_l);
        if (out > MAX_OUT) out = MAX_OUT;
        if (out < 0) out = 0.f;
        // 将计算所得到的值送给 tim3 ch4 pwm
        htim3.Instance->CCR4 = (uint16_t)out;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 让其加上一个3.14，角度拉倒正数 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<///

        imu_date_main.YAW_angle = imuAngle[0] + IMU_TASK_PI;
        imu_date_main.Pitch_angle = imuAngle[1] + IMU_TASK_PI;
        imu_date_main.Roll_angle = imuAngle[2] + IMU_TASK_PI;

// **************************************************** 记录圈数  ******************************************************************************///

        if((imu_date_main.YAW_angle - last_INS_angle[0]) < -IMU_TASK_2_PI)
        {
            imu_date_main.yaw_qvan ++;
        }
        else if((imu_date_main.YAW_angle - last_INS_angle[0]) > IMU_TASK_2_PI)
        {
            imu_date_main.yaw_qvan --;
        }

        if((imu_date_main.Pitch_angle - last_INS_angle[1]) < -IMU_TASK_2_PI)
        {
            imu_date_main.pitch_qvan ++;
        }
        else if((imu_date_main.Pitch_angle - last_INS_angle[1]) > IMU_TASK_2_PI)
        {
            imu_date_main.pitch_qvan --;
        }

        if((imu_date_main.Roll_angle - last_INS_angle[2]) < -IMU_TASK_2_PI)
        {
            imu_date_main.Roll_angle ++;
        }
        else if((imu_date_main.Roll_angle - last_INS_angle[2]) > IMU_TASK_2_PI)
        {
            imu_date_main.Roll_angle --;
        }

/// ****************************************** 信息更新  ***************************************************///

        last_INS_angle[0] = imuAngle[0];last_INS_angle[1] = imuAngle[1];last_INS_angle[2] = imuAngle[2];
        osDelay(1);
    }
}



