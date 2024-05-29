
#include "BSP_CAN.h"
#include "BSP_motor.h"
#include "RM_motor.h"
#include "fdcan.h"
#include "Gym_task.h"
#include "CK_Timeout_task.h"
#include "SHOOT_task.h"

extern GYM_MSG_t GYM_Main;     //云台结构体变量
extern shoot_type_t shoot_main;    // 射击信息结构体




void CAN1_motor_decode(void)
{
    uint32_t ID;
    uint8_t date[8];
    ID = fdcanx_receive(&hfdcan1,date);
    motor_msg_decode_6020(ID-CAN_6010_RETURN,date,&GYM_Main.motor_msg_6020);  // 信息解码
    CkTime_DriverTimeNew(ID-CAN_6010_RETURN + TIMEOUT_GYM_MOTOR_YAW - 1);       // 掉线检测
}





void CAN2_motor_decode(void)
{
    uint32_t ID;
    uint8_t date[8];
    ID = fdcanx_receive(&hfdcan2,date);
    motor_msg_decode_3508(ID-CAN_3508_RETURN,date,&shoot_main.motor_msg);
    CkTime_DriverTimeNew(ID-CAN_6010_RETURN + TIMEOUT_SHOOT_MOTOR_FIR1 - 1);       // 掉线检测
}

