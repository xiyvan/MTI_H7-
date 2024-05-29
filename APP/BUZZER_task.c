/******************************************
    2023/8/28                              
    作者：韩昂轩
    
    1.0  添加了基本的蜂鸣器鸣叫    23.8.28
    1.0.1 说明：该任务的目的是响应掉线的序号  24.1.07

    蜂鸣器鸣叫任务                  
*******************************************
*/


#include "BUZZER_task.h"
#include "cmsis_os.h"
#include "tim.h"

#include "CK_Timeout_task.h"

//************************************* 函数声明  **********************************///

static void Buzzer_Init(void);
static void Buzzer_On(void);
static void Buzzer_Off(void);


//************************************* 变量定义  *********************************///


u8 BUZZER_OUTTIME_NUM = 0;




//************************************* 任务开始  ********************************///


void buzzer_task(void *pvParameters)
{
    Buzzer_Init();
    while (1)
    {
        BUZZER_OUTTIME_NUM = Get_TimeOutNum();
        if(BUZZER_OUTTIME_NUM < 255)
        {
            for(char i = 0; i < BUZZER_OUTTIME_NUM+1;i++)
            {
                Buzzer_On();
                vTaskDelay(150);
                Buzzer_Off();
                vTaskDelay(150);
            }
        }
        osDelay(600);
    }
}





/// @brief buzzer初始化
static void Buzzer_Init(void)
{
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
}



static void Buzzer_On(void)
{
    TIM12->CCR2 = 1500;
}

static void Buzzer_Off(void)
{
    TIM12->CCR2 = 0;
}
