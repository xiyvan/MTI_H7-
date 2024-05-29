#ifndef _CK_TIMEOUT_TASK_H_
#define _CK_TIMEOUT_TASK_H_

#include "main.h"


enum
{
    TIMEOUT_REMOTE_TIMEOUT,
    TIMEOUT_GYM_MOTOR_YAW,
    TIMEOUT_GYM_MOTOR_PITCH,
    TIMEOUT_SHOOT_MOTOR_FIR1,
    TIMEOUT_SHOOT_MOTOR_FIR2,
    TIMEOUT_SHOOT_MOTOR_TAR,
    TIMEOUT_ALL
};



typedef struct 
{
    u32 all_time;
    u32 driver_time[TIMEOUT_ALL];
    u8 TimeOut_num;
}CK_Timeout_struct_t;




void CK_Timeout_task(void *pvParameters);
void CkTime_DriverTimeNew(u8 num);
u8 Get_TimeOutNum(void);



#endif
