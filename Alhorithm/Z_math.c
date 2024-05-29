/******************************************
    2023/9/10                              
    作者：韩昂轩
    
    1.0  添加了浮点型数据的绝对值计算函数    23.9.10
    1.0.1 添加了浮点型数据的限制函数        23.10.19
    1.0.2 快速开方                         24.4.1

    数学运算库                  
*******************************************
*/

#include "Z_math.h"


/// @brief float类型的绝对值计算函数
/// @param x float类型的数据
/// @return 返回的该数据绝对值
float FZ_math_absolute(float x)
{
    if(x == 0)
    {
        return 0;
    }
    if(x > 0)
    {
        return x;
    }
    else if (x < 0)
    {
        return -x;
    }
}


/// @brief float类型的限制函数
/// @param x float类型数据
/// @param max 最大值
/// @param min 最小值
/// @return 限制后的值
float FZ_math_limt(float x,float max,float min)
{
    if(x > max)
    {
        x = max;
    }
    else if(x < min)
    {
        x = min;
    }
    return x;
}


/// @brief 快速开放
/// @param x 
/// @return 结果
float FZ_math_Sqrt(float x)
{
    float y;
    float delta;
    float maxError;
    if (x <= 0)
    {
        return 0;
    }
    // initial guess
    y = x / 2;
    // refine
    maxError = x * 0.001f;
    do
    {
        delta = (y * y) - x;
        y -= delta / (2 * y);
    } while (delta > maxError || delta < -maxError);

    return y;
}






/// @brief 均值滤波
/// @param date 均值滤波数据结构体
unsigned short SZ_math_MeanFilt(MeanFilt_msg_t* date)
{
    date->temp[2] = date->temp[1];
    date->temp[1] = date->temp[0];
    date->temp[0] = date->input;

    date->out = (date->temp[2] + date->temp[1] + date->temp[0]) / 3;
    return date->out;
}
