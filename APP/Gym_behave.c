/***************************************************
 * 创建时间 ： 24.4.21
 * @author 韩昂轩（Han Angxvan）
 * 
 * 云台行为设置包（把云台运动解算从云台任务中分离出来，方便添加新的功能）
 * 
 * @brief 根据模式设置云台行为
****************************************************
*/



#include "Gym_behave.h"





// 云台初始化模式
void GymBehave_Init(GYM_MSG_t* gym)
{
     //pitch 轴角度pid计算
    PID_cale(&gym->pitch_angle_pid,GYM_PITCH_MEDIAN_ANGLE,gym->motor_msg_6020[1].all_angle);
    //初始化pitch速度pid            
    PID_cale(&gym->yaw_pitch_init_pid[1],gym->pitch_angle_pid.out,gym->motor_msg_6020[1].speed_s);
    //PID_cale(&gym->yaw_pitch_init_pid[1],-22,gym->motor_msg_6020[1].speed);                           // debug
    //判断pitch是否初始化完毕，，进入yaw轴初始化
    if((gym->motor_msg_6020[1].all_angle >= GYM_PITCH_MEDIAN_ANGLE - GYM_PITCH_MEDIAN_DEAD_ANGLE/2) && (gym->motor_msg_6020[1].all_angle <= GYM_PITCH_MEDIAN_ANGLE + GYM_PITCH_MEDIAN_DEAD_ANGLE/2))
    {
        //yaw 轴pid计算
        PID_cale(&gym->yaw_angle_pid,GYM_YAW_MEDIAN_ANGLE,gym->motor_msg_6020[0].all_angle);
        //yaw 速度pdi计算
        PID_cale(&gym->yaw_pitch_init_pid[0],gym->yaw_angle_pid.out,gym->motor_msg_6020[0].speed);
        //PID_cale(&gym->yaw_pitch_init_pid[0],200,gym->motor_msg_6020[0].speed);              //debug
        //判断yaw轴是否初始化完毕，把模式转换为中值对应的模式
        if((gym->motor_msg_6020[0].all_angle >= GYM_YAW_MEDIAN_ANGLE - GYM_YAW_MEDIAN_DEAD_ANGLE/2) && (gym->motor_msg_6020[0].all_angle <= GYM_YAW_MEDIAN_ANGLE + GYM_YAW_MEDIAN_DEAD_ANGLE/2))
        {
            //取消初始化模式
            gym->mode_init = GYM_MODE_INIT_N;
            // 设置当前的yaw值为yaw设定值
            gym->gym_set.yaw_angle = gym->Ins_msg.yaw_all_angle;
            // pitch同理
            gym->gym_set.pitch_angle = gym->Ins_msg.pitch_all_angle;
            
            gym->save_mid_pitch = gym->Ins_msg.pitch_all_angle;
        }
    }
}




// 云台正常运行的时候的模式 
void GymBehave_star(GYM_MSG_t* gym)
{
    //pitch 轴pid计算
    PID_cale(&gym->pitch_angle_pid,gym->gym_set.pitch_angle  ,gym->Ins_msg.pitch_all_angle);
    //pitch 速度pdi计算
    PID_cale(&gym->pitch_speed_pid,gym->pitch_angle_pid.out,gym->motor_msg_6020[1].speed_s);
    //yaw 轴pid计算
    PID_cale(&gym->yaw_angle_pid,gym->gym_set.yaw_angle ,gym->Ins_msg.yaw_all_angle);
    //yaw 速度pdi计算 
    PID_cale(&gym->yaw_speed_pid,gym->yaw_angle_pid.out,gym->motor_msg_6020[0].speed);
}

