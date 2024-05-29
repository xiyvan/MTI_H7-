/************************************************
 * 
 * 作者：韩昂轩 （Han Angxvan）
 * 创建时间：2024.2.15
 * 
 * 1.0.0 添加了裁判系统解包任务
 * 
 * 裁判系统消息解包任务
 * **********************************************
*/


#include "cp.h"
#include "Fifo.h"
#include "BSP_Usart.h"
#include "CRC8_CRC16.h"
#include "string.h"
#include "stdio.h"
#include "freeRTOS.h"
#include "task.h"


static void One_SolvePack(void);
static void cp_solve_date(u8* date);



FIFO_t CP_Fifo;            // fifo结构体指针
CP_struct_t CP_main;               // cp解包主要
u8 quque[1024] = {0};

///********************************************************** 裁判系统解包专用的变量定义  ***************************************************///

frame_header_struct_t cp_receive_header;        // 裁判系统接受信息头部信息
game_status_t game_state;                       // 比赛阶段以及该阶段剩余时间
game_result_t game_result;
game_robot_HP_t game_robot_HP;
robot_status_t robot_status;
power_heat_data_t power_heat_data;
buff_t buff;
air_support_data_t air_support_data;
rfid_status_t rfid_status;
dart_client_cmd_t dart_client_cmd;              // 飞镖控制书
radar_mark_data_t radar_mark_data;              // 被标记进度





///**************************************************************  endl  *****************************************************************///


void CP_task(void *pvParameters)
{
    CP_Fifo.max = 1024;
    CP_Fifo.queue = quque;
    CP_Fifo.get_num = 0;
    CP_Fifo.num = 0;
    CP_Fifo.add_num = 0;
    while (1)
    {       
        One_SolvePack();
        vTaskDelay(2);
    }
}














/************************************
 * @brief 裁判系统单字节解包
 * @author 韩昂轩(Han Angxvan)
 * 
 * **********************************
*/
static void One_SolvePack(void)
{
    u8 byte = 0;
    u8 sof = CP_SOF_NUM;
    CP_struct_t *p_obj = & CP_main;

    while(CP_Fifo.num)
    {
        byte = Fifo_Get(&CP_Fifo);               // 从fifo中取出一个字节
        switch(p_obj->unpack_step)
        {
            case CP_SOF:
            {
                if(byte == sof)
                {
                    p_obj->unpack_step = CP_DAT_LEN_LOW;
                    p_obj->protocol_packet[p_obj->index++] = byte;
                }
                else
                {
                    p_obj->index = 0;
                }
            }break;

            case CP_DAT_LEN_LOW:
            {
                p_obj->data_len = byte;
                p_obj->protocol_packet[p_obj->index++] = byte;
                p_obj->unpack_step = CP_DAT_LEN_HIGH;
            }break;

            case CP_DAT_LEN_HIGH:
            {
                p_obj->data_len |= (byte << 8);                                 // 合成长度
                p_obj->protocol_packet[p_obj->index++] = byte;
                if(p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN))
                {
                    p_obj->unpack_step = CP_SEQ;
                }
                else
                {
                    p_obj->unpack_step = CP_SOF;          // 如果数据包的长度大于最大的长度，就认为数据包损坏放弃这一个
                    p_obj->index = 0;
                }
            }break;

            case CP_SEQ:
            {
                p_obj->protocol_packet[p_obj->index++] = byte;
                p_obj->unpack_step = CP_CRC8;
            }break;

            case CP_CRC8:
            {
                p_obj->protocol_packet[p_obj->index++] = byte;
                if(p_obj->index == REF_PROTOCOL_HEADER_SIZE)
                {
                    //p_obj->unpack_step = CP_CRC16;			// debug
                    if(verify_CRC8_check_sum(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE))
                    {
                        p_obj->unpack_step = CP_CRC16;
                    }
                    else
                    {
                        p_obj->unpack_step = CP_SOF;
                        p_obj->index = 0;
                    }
                }
            }break;

            case CP_CRC16:
            {
                if(p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                {
                    p_obj->protocol_packet[p_obj->index++] = byte;
                }
                if(p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                {
                    p_obj->unpack_step = CP_SOF;
                    p_obj->index = 0;
                    //cp_solve_date(p_obj->protocol_packet);			// debug
                    if(verify_CRC16_check_sum(p_obj->protocol_packet,REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                    {
                        cp_solve_date(p_obj->protocol_packet);
                    }
									
                }
            }break;

            default:
            {
                p_obj->unpack_step = CP_SOF;
                p_obj->index = 0;
            }break;
        }
    }
}





/// @brief 数据解包
/// @param cmdid 命令码
/// @param date 数据数组
/// @author 韩昂轩 （Hang Angxvan）
static void cp_solve_date(u8* date)
{
    u16 cmd_id = 0;
    u8 index = 0;

    memcpy(&cp_receive_header,date,sizeof(frame_header_struct_t));          // 将指定字节数直接复制到cp_receive结构体里面
    index += sizeof(frame_header_struct_t);
    memcpy(&cmd_id,date + index,sizeof(u16));                               // 将16位的命令码赋给cmdid
    index += sizeof(u16);

    switch (cmd_id)
    {
        case game_status:
        {
            memcpy(&game_state,date + index,sizeof(game_status_t));
        }break;
        
        case game_result_ID:
        {
            memcpy(&game_result,date + index,sizeof(game_result_t));
        }break;

        case game_robot_HP_ID:
        {
            memcpy(&game_robot_HP,date + index,sizeof(game_robot_HP_t));
        }break;

        case robot_status_ID:
        {
            memcpy(&robot_status,date + index,sizeof(robot_status_t));
        }break;

        case power_heat_data_ID:
        {
            memcpy(&power_heat_data,date + index,sizeof(power_heat_data_t));
        }break;

        case buff_ID:
        {
            memcpy(&buff,date + index,sizeof(buff_t));
        }break;

        case air_support_data_ID:
        {
            memcpy(&air_support_data,date + index,sizeof(air_support_data_t));
        }break;

        case rfid_status_ID:
        {
            memcpy(&rfid_status,date + index,sizeof(rfid_status_t));
        }break;

        case radar_mark_data_ID:
        {
            memcpy(&radar_mark_data,date + index,sizeof(radar_mark_data_t));
        }break;

        case dart_client_cmd_ID:
        {
            memcpy(&dart_client_cmd,date + index,sizeof(dart_client_cmd_t));
        }break;

        default:
        {
            break;
        }
    }
}




/// @brief 初始化一些需要使用到的结构体
static void cp_struct_data_init(void)
{
    memset(&cp_receive_header,0,sizeof(frame_header_struct_t));     // 初始化接收头部信息结构体
    memset(&game_state,0,sizeof(game_status_t));
    memset(&game_result,0,sizeof(game_result_t));
    memset(&game_robot_HP,0,sizeof(game_robot_HP_t));
    memset(&robot_status,0,sizeof(robot_status_t));
    memset(&power_heat_data,0,sizeof(power_heat_data_t));
    memset(&buff,0,sizeof(buff_t));
    memset(&air_support_data,0,sizeof(air_support_data_t));
    memset(&rfid_status,0,sizeof(rfid_status_t));
    memset(&dart_client_cmd,0,sizeof(dart_client_cmd_t));
    memset(&radar_mark_data,0,sizeof(radar_mark_data_t));
}




/// @brief 获取机器人ID
/// @param data 信息指针
void get_robot_id(u16* ID)
{
    *ID = robot_status.robot_id;
}


/// @brief 获取机器人发射热量与热量限制
/// @param heat 信息指针
void get_robot_fire_heat(u16* heat,u16* limted)
{
    *heat = power_heat_data.shooter_17mm_1_barrel_heat;
    *limted = robot_status.shooter_barrel_heat_limit;
}



/// @brief 获取机器人的底盘能量缓冲 
/// @return 底盘剩余能量缓冲罐
u16 get_chassis_power_buf(void)
{
    return power_heat_data.buffer_energy;
}
