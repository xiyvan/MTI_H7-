/************************************
 * @author 韩昂轩（Han Angxvan）
 * 创建时间 ： 24.4.26
 * 
 *    LED闪烁任务包
************************************/



#include "WS2812_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_ws1812.h"
#include "cmsis_os.h"

#include "CK_Timeout_task.h"


void ws2812_task(void *argument)
{
    u8 state = 0xff;
    while (1)
    {
        // 获取掉线设备id
        state = Get_TimeOutNum();
        
        if(state == 0xff)
        {
            // 如果没有设备掉线的时候
            WS2812_Ctrl(0,255,0);
            osDelay(500);
            WS2812_Ctrl(0,0,255);
            osDelay(500);
        }
        else
        {
            // 如果有设备掉线
            WS2812_Ctrl(255,0,0);
            osDelay(100);
            WS2812_Ctrl(0,0,0);
            osDelay(100);
        }
    }
}



