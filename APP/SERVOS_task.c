/******************************************
    2023/8/28                              
    作者：韩昂轩
    
    1.0  添加了基本舵机驱动    23.8.29
    1.0.1 添加了遥控控制舵机    23.9.16
    1.0.2 从c版移植到了达妙的h7板子上面 24.5.3 

    舵机控制任务
*******************************************
*/


#include "SERVOS_task.h"


void static servos_Init(servos_type_t* servos);

servos_type_t servos_main;



void Servos_task(void *pvParameters)
{
    servos_Init(&servos_main);
    while (1)
    {
        if(servos_main.remote_msg->channel_4 > 600)
        {
            // 改变其ccr寄存器从而改变比较值
            htim1.Instance->CCR3 = 1000;
        }
        else if(servos_main.remote_msg->channel_4 < 600 && servos_main.remote_msg->channel_4 > 0)
        {
            htim1.Instance->CCR3 = 2000;
        }
        vTaskDelay(20);
    }
}




/// @brief 初始化
/// @param servos 舵机结构体指针
void static servos_Init(servos_type_t* servos)
{
    // 获取遥控器信息指针
    servos->remote_msg = get_remote_msg_p();

    // 使能舵机接口5v输出
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);

    // 使能TIM1 CH3 pwm输出
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
}
