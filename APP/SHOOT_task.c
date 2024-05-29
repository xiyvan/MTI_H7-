/************************************************
 * 
 * 作者：韩昂轩
 * 创建时间：2023.11.16
 * 
 * 射击任务
 * **********************************************
*/

#include "SHOOT_task.h"
#include "fdcan.h"
#include "RM_motor.h"
#include "cmsis_os.h"
#include "cp.h"

/********************************************************  变量定义     **************************************************/

shoot_type_t shoot_main;    // 射击信息结构体



/********************************************************   函数声明       **********************************************/
static void Shoot_Init(shoot_type_t* shoot);
static void shoot_mode_set(shoot_type_t* shoot);
static void shoot_out_set(shoot_type_t* shoot);
static void shoot_ctrl_set(shoot_type_t* shoot);

/********************************************************* 函数声明 -end**************************************************/


/****************************
 * @brief 射击主任务函数
 * @author 韩昂轩
 * **************************
*/
void SHOOT_task(void *pvParameters)
{
    Shoot_Init(&shoot_main);
    while(1)
    {
        shoot_mode_set(&shoot_main);
        shoot_ctrl_set(&shoot_main);
        shoot_out_set(&shoot_main);
        RM_motor_send(&hfdcan2,-shoot_main.curre_set[0],shoot_main.curre_set[1],
                        -shoot_main.curre_set[2],0,CAN_3508_ID14);       // RM系列电机电流发送
      //RM_motor_send(CAN1,-400,400,400,0,CAN_3508_ID14);
        osDelay(20);
    }
}






/****************************
 * @brief 射击参数初始化函数
 * @param shoot 射击结构信息体指针
 * @author 韩昂轩
 * **************************
*/
static void Shoot_Init(shoot_type_t* shoot)
{
    shoot->remove_msg = get_remote_msg_p();     // 获取遥控器信息指针

    float PID_param[3] = {SHOOT_TASK_WHEEL_SPEED_PID_KP,SHOOT_TASK_WHEEL_SPEED_PID_KI,SHOOT_TASK_WHEEL_SPEED_PID_KD};
    PID_Init(&shoot->wheel_speed[0],PID_param,SHOOT_TASK_WHEEL_SPEDED_PID_MAX,SHOOT_TASK_WHEEL_SPEED_PID_MAXI);
    PID_Init(&shoot->wheel_speed[1],PID_param,SHOOT_TASK_WHEEL_SPEDED_PID_MAX,SHOOT_TASK_WHEEL_SPEED_PID_MAXI);

    PID_param[0] = SHOOT_TASK_MOTOR_ANG_PID_KP;PID_param[1] = SHOOT_TASK_MOTOR_ANG_PID_KI;PID_param[2] = SHOOT_TASK_MOTOR_ANG_PID_KD;       // 拨弹电机角度PID
    PID_Init(&shoot->motor_ang_speed[0],PID_param,SHOOT_TASK_MOTOR_ANG_PID_MAX,SHOOT_TASK_MOTOR_ANG_PID_MAXI);

    PID_param[0] = SHOOT_TASK_MOTOR_SPEED_PID_KP;PID_param[1] = SHOOT_TASK_MOTOR_SPEED_PID_KI;PID_param[2] = SHOOT_TASK_MOTOR_SPEED_PID_KD;     // 拨弹电机速度PID
    PID_Init(&shoot->motor_ang_speed[1],PID_param,SHOOT_TASK_MOTOR_SPEED_PID_MAX,SHOOT_TASK_MOTOR_SPEED_PID_MAXI);
}




/****************************
 * @brief 射击模式设置
 * @param shoot 射击结构信息体指针
 * @author 韩昂轩
 * **************************
*/
static void shoot_mode_set(shoot_type_t* shoot)
{
    u16 heat;       // 用来获取热量
    u16 limted;     // 用来获取热量限制
    get_robot_fire_heat(&heat,&limted);
    switch (shoot->remove_msg->S2)
    {
    case REMOVE_S_MID:
        shoot->mode = SHOOT_TASK_MODE_READY;        // 射击准备模式
        break;
    case REMOVE_S_UP:
        shoot->mode = SHOOT_TASK_MODE_FIRE;         // 射击连续开火模式
        break;
    default:
        shoot->mode = SHOOT_TASK_MODE_STOP;         // 射击停止模式
        break;
    }

    // 鼠标左键开火
    // 如果左键按下并且档杆在中间的时候触发设计开始
    if((shoot->remove_msg->mouse_left) && (shoot->remove_msg->S2 == REMOVE_S_MID))
    {
        shoot->mode = SHOOT_TASK_MODE_FIRE;
    }

    // 热量限制
    if(heat + 15 >= limted)
    {
        shoot->mode = SHOOT_TASK_MODE_READY;
    }

    // 档杆向下的时候设计停止
    if(shoot->remove_msg->S2 == REMOVE_S_DOWN)
    {
        shoot->mode = SHOOT_TASK_MODE_STOP;
    }
}





/****************************
 * @brief 射击控制量设置
 * @param shoot 射击结构信息体指针
 * @author 韩昂轩
 * **************************
*/
static void shoot_ctrl_set(shoot_type_t* shoot)
{
    switch (shoot->mode)
    {
        case SHOOT_TASK_MODE_FIRE:
        {
            //PID_cale(&shoot->motor_ang_speed[0],shoot->motor_msg[2].all_angle+SHOOT_TASK_SHOOT_DIAL_STEP,shoot->motor_msg[2].all_angle);      // 计算拨盘电机的转动角度
            PID_cale(&shoot->motor_ang_speed[1],SHOOT_TASK_ANG_SPEED,-shoot->motor_msg[2].speed);            // 角度串联速度
            PID_cale(&shoot->wheel_speed[0],SHOOT_TASK_SHOOT_SPE,-shoot->motor_msg[0].speed);
            PID_cale(&shoot->wheel_speed[1],SHOOT_TASK_SHOOT_SPE,shoot->motor_msg[1].speed);     // 控制摩擦轮转速
        }break;

        case SHOOT_TASK_MODE_READY:
            {
                PID_cale(&shoot->motor_ang_speed[1],0,-shoot->motor_msg[2].speed);
                PID_cale(&shoot->wheel_speed[0],SHOOT_TASK_SHOOT_SPE,-shoot->motor_msg[0].speed);
                PID_cale(&shoot->wheel_speed[1],SHOOT_TASK_SHOOT_SPE,shoot->motor_msg[1].speed);     // 控制摩擦轮转速
            }break;
    }
}




/****************************
 * @brief 射击输出量设置
 * @param shoot 射击结构信息体指针
 * @author 韩昂轩
 * **************************
*/
static void shoot_out_set(shoot_type_t* shoot)
{
    if(shoot->mode == SHOOT_TASK_MODE_STOP)             // 当处于设计停止状态
    {
        shoot->curre_set[0] = 0;
        shoot->curre_set[1] = 0;
        shoot->curre_set[2] = 0;
    }
    else if(shoot->mode == SHOOT_TASK_MODE_READY || shoot->mode == SHOOT_TASK_MODE_FIRE)        // 当处于射击准备或者射击开火状态的时候
    {
        shoot->curre_set[0] = shoot->wheel_speed[0].out;
        shoot->curre_set[1] = shoot->wheel_speed[1].out;
		shoot->curre_set[2] = 0;
    }
    if(shoot->mode == SHOOT_TASK_MODE_FIRE)                 // 当处于设计开火状态的时候
    {
        shoot->curre_set[2] = shoot->motor_ang_speed[1].out;
    }
}




