/******************************************
    2023/6/17                              
    作者：韩昂轩                       
    
    1.0  添加了位置PID  23.6.17

    pid算法包             
*******************************************
*/

#include "PID.h"


//限幅
static float lim_max(float x,float max);




/// @brief PID结构体初始化
/// @param pid_init pid结构体变量指针
/// @param param kp、ki、kd变量数组
/// @param MAX_OUT 最大输出
/// @param MAXI_OUT 最大积分输出
void PID_Init(PID_type_def* pid_init,float* param,float MAX_OUT,float MAXI_OUT)
{
    if(pid_init == 0)
    {
        return;
    }

    pid_init->kp = param[0];
    pid_init->ki = param[1];
    pid_init->kd = param[2];
    pid_init->max_out = MAX_OUT;
    pid_init->maxi_out = MAXI_OUT;
}



/// @brief pid计算函数
/// @param pid_calc pid结构体
/// @param set 设置值
/// @param feekbak 返回值
/// @return 计算后的值
float PID_cale(PID_type_def* pid_calc,float set,float feekbak)
{
    pid_calc->set = set;
    pid_calc->erro[2] = pid_calc->erro[1];
    pid_calc->erro[1] = pid_calc->erro[0];
    pid_calc->erro[0] = set - feekbak;
    pid_calc->fdb = feekbak;

    pid_calc->p_out = pid_calc->kp * pid_calc->erro[0];
    pid_calc->i_out += pid_calc->ki * pid_calc->erro[0];
    pid_calc->d_buffer[2] = pid_calc->d_buffer[1];
    pid_calc->d_buffer[1] = pid_calc->d_buffer[0];
    pid_calc->d_buffer[0] = (pid_calc->erro[0] - pid_calc->erro[1]);
    pid_calc->d_out = pid_calc->kd * pid_calc->d_buffer[0];

    pid_calc->i_out = lim_max(pid_calc->i_out,pid_calc->maxi_out);
    pid_calc->out = lim_max(pid_calc->i_out+pid_calc->p_out+pid_calc->d_out,pid_calc->max_out);
    return pid_calc->out;

}





/// @brief 限幅函数
/// @param x 输入值
/// @param max 最大值
/// @return 限幅后的值
static float lim_max(float x,float max)
{
    if(x > max)
    {
        return max;
    }
    else if(x < -max)
    {
        return -max;
    }
    else
    {
        return x;
    }
}
