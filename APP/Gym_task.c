/**************************************
*   作者：韩昂轩
*    1.0.0   添加了云台任务函数      23.10.23
*
*   云台任务
***************************************
*/

#include "Gym_task.h"
#include "Gym_behave.h"
#include "imu_task.h"
#include "fdcan.h"
#include "cmsis_os.h"
#include "BSP_SendDate.h"
#include "cp.h"


///*********************************************************** 函数声明 **********************************************************///
static void Gym_Init(GYM_MSG_t* GYM);
static void Gym_msg_updata(GYM_MSG_t* gym);
static void Gym_Mode_set(GYM_MSG_t* Gym);
static void Gym_Ctrl_set(GYM_MSG_t* GYM);
static void Gym_Solve(GYM_MSG_t* gym);
static void Gym_cale(GYM_MSG_t* gym);

///*********************************************************** 变量定义 *********************************************************///

GYM_MSG_t GYM_Main;     //云台结构体变量
extern imu_date_struct_t imu_date_main;
Remote_speed_t remote_speed;    // 发送给底盘速度设置
Remote_angle_t remote_angle;    // 相对角度发送

///************************************************************ endl ******************************************************///




/****************************
 * @brief 云台任务函数
 * @author 韩昂轩
 * **************************
*/
void Gym_task(void *pvParameters)
{
	Gym_Init(&GYM_Main);        //初始化云台
    osDelay(200);            // 等待陀螺仪达到正常工作状态
	while(1)
	{
        Gym_Mode_set(&GYM_Main);    //云台模式设置    
        Gym_msg_updata(&GYM_Main);  // 云台信息更新
        Gym_Ctrl_set(&GYM_Main);    //云台控制量设置
        Gym_Solve(&GYM_Main);       //云台解算
        Gym_cale(&GYM_Main);        //云台电流幅值

        RM_motor_send(&hfdcan1,GYM_Main.gym_set.current_set[0],GYM_Main.gym_set.current_set[1],0,0,CAN_6010_ID14);        //电流发送给电机
        //RM_motor_send(CAN2,4000,-4000,1000,1000,CAN_6010_ID14);               // debug
        //GYM_send_realy_angly();                 // 向底盘发送相对角度
        osDelay(3);
	}
}






/****************************
 * @brief 云台参数初始化函数
 * @param GYM 云台结构体变量指针
 * @author 韩昂轩
 * **************************
*/
static void Gym_Init(GYM_MSG_t* GYM)
{
    GYM->remote_msg = get_remote_msg_p();       /*获取遥控器信息指针*/
    
    float pid_cone[3] = {GYM_PID_YAW_speed_KP,GYM_PID_YAW_speed_KI,GYM_PID_YAW_speed_KD};
    PID_Init(&GYM->yaw_speed_pid,pid_cone,GYM_PID_YAW_speed_MAXOUT,GYM_PID_YAW_speed_MAXIOUT);  //YAW轴速度PID初始化

    pid_cone[0] = GYM_PID_YAW_angle_KP;pid_cone[1] = GYM_PID_YAW_angle_KI;pid_cone[2] = GYM_PID_YAW_angle_KD;
    PID_Init(&GYM->yaw_angle_pid,pid_cone,GYM_PID_YAW_angle_MAXOUT,GYM_PID_YAW_angle_MAXIOUT);  //YAW轴角度PID初始化

    pid_cone[0] = GYM_PID_PITCH_speed_KP;pid_cone[1] = GYM_PID_PITCH_speed_KI;pid_cone[2] = GYM_PID_PITCH_speed_KD;
    PID_Init(&GYM->pitch_speed_pid,pid_cone,GYM_PID_PITCH_speed_MAXOUT,GYM_PID_PITCH_speed_MAXIOUT);    //pitch轴PID初始化

    pid_cone[0] = GYM_PID_PITCH_angle_KP;pid_cone[1] = GYM_PID_PITCH_angle_KI;pid_cone[2] = GYM_PID_PITCH_angle_KD;
    PID_Init(&GYM->pitch_angle_pid,pid_cone,GYM_PID_PITCH_angle_MAXOUT,GYM_PID_PITCH_angle_MAXIOUT);    //PITCH轴pid初始化

    pid_cone[0] = GYM_PID_YAW_speed_init_KP;pid_cone[1] = GYM_PID_YAW_speed_init_KI;pid_cone[2] = GYM_PID_YAW_speed_init_KD;
    PID_Init(&GYM->yaw_pitch_init_pid[0],pid_cone,GYM_PID_YAW_speed_init_MAXOUT,GYM_PID_YAW_speed_init_MAXIOUT);    //YAW轴初始化PID初始化

    pid_cone[0] = GYM_PID_PITCH_speed_init_KP;pid_cone[1] = GYM_PID_PITCH_speed_init_KI;pid_cone[2] = GYM_PID_PITCH_speed_init_KD;
    PID_Init(&GYM->yaw_pitch_init_pid[1],pid_cone,GYM_PID_PITCH_speed_init_MAXOUT,GYM_PID_PITCH_speed_init_MAXIOUT);    //PITCH轴初始化PID初始化

    GYM->Ins_msg.YawAngle = &imu_date_main.YAW_angle;          // yaw轴的陀螺仪解算角度
    GYM->Ins_msg.Yaw_qvan = &imu_date_main.yaw_qvan;
    GYM->Ins_msg.PitchAngle = &imu_date_main.Pitch_angle;       // pitch轴的陀螺仪解算角度
    GYM->Ins_msg.Pitch_qvan = &imu_date_main.pitch_qvan;        
}











/****************************
 * @brief 云台模式选择函数
 * @param GYM 云台信息指针
 * @author 韩昂轩
 * **************************
*/
static void Gym_Mode_set(GYM_MSG_t* Gym)
{
    switch (Gym->remote_msg->S1)
    {
    case REMOVE_S_DOWN:
        {   
            //波妞开关向下零电流
            Gym->mode = GYM_MODE_ZERO;                      
        }break;

    case REMOVE_S_MID :
        {
            //波妞开关中间 地盘跟随底盘
            Gym->mode = GYM_MODE_CHASSIS_FOLLOW_CHASSIS;    
        }break;

    case REMOVE_S_UP :
        {
            //上面 地盘跟随云台
            Gym->mode = GYM_MODE_CHASSIS_FOLLOW_GYM;
        }break;

    default:
        {
            //其他情况 云台零电流
            Gym->mode = GYM_MODE_ZERO;
        }break;
    }

///**********************************************************  模式切换的时候要发生的事件  ******************************************************************///
    if(Gym->last_mode == GYM_MODE_ZERO && (Gym->mode == GYM_MODE_CHASSIS_FOLLOW_GYM || Gym->mode == GYM_MODE_CHASSIS_FOLLOW_CHASSIS))
    {
        Gym->mode_init = GYM_MODE_INIT;          //零电流模式向其他模式转变的时候进入初始化模式，防止超过其富裕角度过小而导致系统发散
        Gym->motor_msg_6020[0].qvan = 0;         // 为了防止圈数溢出每次上电都让其置零
        Gym->motor_msg_6020[1].qvan = 0;
        *Gym->Ins_msg.Pitch_qvan = 0;
        *Gym->Ins_msg.Yaw_qvan = 0;
    }
///********************************************************  更新last_mode  ******************************************************************************///
    Gym->last_mode = Gym->mode;             //更新上一次模式模式

}








/*********************************************************************************************************************************************************************
 * @brief 云台控制量设置函数
 * @param GYM 云台信息指针
 * @author 韩昂轩
 * @note 云台角度控制设置方式为 通过逐步增加 'gym_set'这个变量中的值来设置目标角度。后面让6020信息反馈追寻该设置值，每次初始化后都会让电机的圈数置零，然后让‘gym_set’=当前的值。
 * ********************************************************************************************************************************************************************
*/
static void Gym_Ctrl_set(GYM_MSG_t* GYM)
{
    if(GYM->mode_init == GYM_MODE_INIT)
    {
        GYM->gym_set.pitch_angle = 0;
        GYM->gym_set.yaw_angle = 0;
    }
    else if(GYM->mode == GYM_MODE_CHASSIS_FOLLOW_CHASSIS || GYM->mode == GYM_MODE_CHASSIS_FOLLOW_GYM)    //如果为底盘跟随地盘 或者为地盘跟随云台 模式的话
    {
        GYM->gym_set.pitch_angle = GYM->gym_set.pitch_angle + GYM->remote_msg->channel_3 / GYM_PITCH_REMOVE_CHANGE + GYM->remote_msg->mouse_x * GYM_YAW_REMOVE_MOUSE;     // 设置 pitch 轴角度为当前角度加上一些值
        GYM->gym_set.yaw_angle =GYM->gym_set.yaw_angle + GYM->remote_msg->channel_2 / GYM_YAW_REMOVE_CHANGE + GYM->remote_msg->mouse_y * GYM_PITCH_REMOVE_MOUSE;

        // 对pitch角度进行限制
        if(GYM->gym_set.pitch_angle >= GYM->save_mid_pitch + GYM_PITCH_FW)            
        {
            GYM->gym_set.pitch_angle = GYM->save_mid_pitch + GYM_PITCH_FW;
        }
        else if(GYM->gym_set.pitch_angle <=  GYM->save_mid_pitch - GYM_PITCH_FW)
        {
            GYM->gym_set.pitch_angle = GYM->save_mid_pitch - GYM_PITCH_FW;
        }
    }
}








/****************************
 * @brief 云台运动解算函数
 * @param gym 云台信息指针
 * @author 韩昂轩
 * **************************
*/
static void Gym_Solve(GYM_MSG_t* gym)
{
    if(gym->mode_init == GYM_MODE_INIT)
    {
        // 云台初始化
        GymBehave_Init(gym);
    }
    else if(gym->mode == GYM_MODE_CHASSIS_FOLLOW_CHASSIS || gym->mode == GYM_MODE_CHASSIS_FOLLOW_GYM)
    {
        // 云台正常运行
        GymBehave_star(gym);
    }
}







/**************************************************************************************
 * @brief 云台输出计算函数
 * @param gym 云台信息指针
 * @author 韩昂轩
 * @note 该函数的意思是让前面计数出来的数值，根据模式对电机的输出值进行赋值，达到解耦的目的。
 * *************************************************************************************
*/
static void Gym_cale(GYM_MSG_t* gym)
{
    if(gym->mode == GYM_MODE_ZERO)          
    {
        //当云台模式为零电流的时候
        gym->gym_set.current_set[0] = 0;
        gym->gym_set.current_set[1] = 0;
    }
    else if(gym->mode_init == GYM_MODE_INIT)
    {   
        // 当云台模式为初始化模式的时候
        gym->gym_set.current_set[0] = gym->yaw_pitch_init_pid[0].out;
        gym->gym_set.current_set[1] = gym->yaw_pitch_init_pid[1].out;
    }
    else if((gym->mode == GYM_MODE_CHASSIS_FOLLOW_CHASSIS) || (gym->mode == GYM_MODE_CHASSIS_FOLLOW_GYM))
    {
        // 当云台是正常模式的时候
        gym->gym_set.current_set[0] = gym->yaw_speed_pid.out;
        gym->gym_set.current_set[1] = gym->pitch_speed_pid.out;
    }
}




/*******************************
 * @brief 云台信息更新
 * @param gym 云台信息结构体指针
 * @note 兼顾给地盘发送控制信息的任务
*******************************/
static void Gym_msg_updata(GYM_MSG_t* gym)
{
    // 给云台的yaw与pitch轴记圈
    gym->Ins_msg.pitch_all_angle = *gym->Ins_msg.Pitch_qvan * 6.28 + *gym->Ins_msg.PitchAngle;
    gym->Ins_msg.yaw_all_angle = *gym->Ins_msg.Yaw_qvan * 6.28 + *gym->Ins_msg.YawAngle;

    // 给底盘发送相关信息
    ///*********************  vx set ***************************************///

    remote_speed.vx_set = gym->remote_msg->channel_1;
    if(gym->remote_msg->key & KEY_PRESSED_OFFSET_W)
    {
        remote_speed.vx_set = 660;
    }
    else if(gym->remote_msg->key & KEY_PRESSED_OFFSET_S)
    {
        remote_speed.vx_set = -660;
    }

    ///*********************  vy set ************************************///

    remote_speed.vy_set = gym->remote_msg->channel_0;
    if(gym->remote_msg->key & KEY_PRESSED_OFFSET_D)
    {
        remote_speed.vy_set = 660;
    }
    else if(gym->remote_msg->key & KEY_PRESSED_OFFSET_A)
    {
        remote_speed.vy_set = -660;
    }

    ///*********************  wz set ************************************///

    remote_speed.wz_set = gym->remote_msg->channel_2;
    if(gym->remote_msg->mouse_x != 0)
    {
        /// 这里可能要乘以一个系数等测试后确定（未测试）
        remote_speed.wz_set = gym->remote_msg->mouse_x;
    }

    remote_speed.mode_set = gym->remote_msg->S1;
    remote_speed.sof = 0xa5;
    SendDate_loading_speed(&remote_speed);

    ///************************ 云台相对中值角度  ***************************///
	osDelay(1);
    remote_angle.angle = gym->motor_msg_6020[0].angle - GYM_YAW_MEDIAN_ANGLE;
    remote_angle.huan = get_chassis_power_buf();
    remote_angle.sof = 0xa5;
    SendDate_loading_angle(&remote_angle);
}


