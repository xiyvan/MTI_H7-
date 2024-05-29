
#ifndef _CP_H_
#define _CP_H_

#include "main.h"


#define CP_SOF_NUM 0xa5                 // 包头序号

// 单个包最大长度
#define REF_PROTOCOL_FRAME_MAX_SIZE         128             
// 头部包长度
#define REF_PROTOCOL_HEADER_SIZE            sizeof(frame_header_struct_t)
#define REF_PROTOCOL_CMD_SIZE               2
#define REF_PROTOCOL_CRC16_SIZE             2
#define REF_HEADER_CRC_LEN                  (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE)
#define REF_HEADER_CRC_CMDID_LEN            (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE + sizeof(u16))
#define REF_HEADER_CMDID_LEN                (REF_PROTOCOL_HEADER_SIZE + sizeof(u16))


/// @brief 包头信息
typedef struct __attribute__((packed))
{
    u8 SOF;
    u16 data_len;
    u8 seq;
    u8 CRC8;
}frame_header_struct_t;




/// @brief 解包步骤
typedef enum
{
    CP_SOF,
    CP_DAT_LEN_LOW,
    CP_DAT_LEN_HIGH,
    CP_SEQ,
    CP_CRC8,
    CP_CRC16,
}unpack_step_e;



typedef struct 
{
    frame_header_struct_t *p_header;                            // 保存头部信息
    u16 data_len;                                               // 记录其数据包实际长度
    u8 protocol_packet[REF_PROTOCOL_FRAME_MAX_SIZE];            //  整个包的数据
    unpack_step_e unpack_step;                                  // 用来保存当前进行的解包步骤    
    u16 index;                                                  // 用来指向包的第几个数据
}CP_struct_t;





///*********************************************************************  裁判系统命令码  ****************************************************///

typedef enum
{
    game_status                         =       0x0001,             // 比赛阶段以及该阶段剩余时间
    game_result_ID                      =       0x0002,             // 比赛结局
    game_robot_HP_ID                    =       0x0003,             // 各个机器人的血量
    event_data                          =       0x0101,             // 场地事件
    ext_supply_projectile_action        =       0x0102,             // 补单情况
    referee_warning                     =       0x0104,             // 机器人判罚情况
    dart_info                           =       0x0105,             // 飞镖剩余打击时间，上一次命中情况
    robot_status_ID                   	=       0x0201,             // 机器人等级血量，各个机构上电情况
    power_heat_data_ID                  =       0x0202,             // 功率以及枪口热亮，缓冲能量
    robot_pos                           =       0x0203,             // 本机器人坐标以及头朝向
    buff_ID                             =       0x0204,             // 机器人buff
    air_support_data_ID                 =       0x0205,             // 飞机状态
    hurt_data                           =       0x0206,
    shoot_data                          =       0x0207,
    projectile_allowance                =       0x0208,
    rfid_status_ID                      =       0x0209,             // RFID 状态书
    dart_client_cmd_ID                  =       0x020a,             // 飞镖站命令书
    ground_robot_position               =       0x020b,             // 己方各个机器人的坐标
    radar_mark_data_ID                  =       0x020c,             // 己方各个机器人被雷达标记进度 0-120
    sentry_info                         =       0x020d,             // 哨兵可兑换弹丸与血量信息
    radar_info                          =       0x020e,             // 雷达触发双倍以上的机会，以及对方是否正在启动双倍易伤
    robot_interaction_data              =       0x0301,             // 机器人交互数据
    custom_info                         =       0x0308,             // 机器人向选手端发送数据
}cp_cmd_id_t;

///*******************************************************************  各个包的结构体信息  **************************************************///

/// @brief 比赛阶段以及该阶段剩余时间
typedef struct __attribute__((packed))
{
	u8 game_type : 4;                           // 比赛类型
    u8 game_progress : 4;                       // 当前比赛阶段
    u16 stage_remain_time;                      // 当前阶段剩余时间
    unsigned long long synctimestamp;           // 没用（占位）
}game_status_t;


/// @brief 比赛结局
typedef struct __attribute__((packed))
{
    u8 winner;                                  // 游戏结局
}game_result_t;


/// @brief 各个机器人的血量
typedef struct __attribute__((packed))
{
    u16 red_1_robot;
    u16 red_2_robot;
    u16 red_3_robot;
    u16 red_4_robot;
    u16 red_5_robot;
    u16 red_7_robot;
    u16 red_outpost;
    u16 red_base;
    u16 blue_1_robot;
    u16 blue_2_robot;
    u16 blue_3_robot;
    u16 blue_4_robot;
    u16 blue_5_robot;
    u16 blue_7_robot;
    u16 blue_outpost;
    u16 blue_base;
}game_robot_HP_t;




/// @brief 补单情况
typedef struct __attribute__((packed))
{
    u8 reserved;                            // 没用（暂时）
    u8 supply_robot_id;                     // 补单机器人ID
    u8 supply_projectile_step;              // 出单口开闭状态
    u8 supply_projectile_num;               // 补单数量
}ext_supply_projectile_action_t;



/// @brief 判罚情况
typedef struct __attribute__((packed))
{
    u8 level;                               // 判罚等级
    u8 offending_robot_id;                  // 最后一次判罚机器人ID
    u8 count;                               // 最后一次受到怕发的违规机器人对应的判罚登记的违规次数
}referee_warning_t;



/// @brief 飞镖信息
typedef struct __attribute__((packed)) 
{
    u8 dart_remaining_time;                 // 己方飞镖发射剩余时间
    u16 dart_info;                          // 最近一次集中地目标
}dart_info_t;




/// @brief 机器人状态信息
typedef struct __attribute__((packed)) 
{
    u8 robot_id;                    // 本机器人ID
    u8 robot_level;                 // 本机器人等级
    u16 current_HP;                  // 当前机器人血量
    u16 maximum_HP;                  // 机器人血量上限
    u16 shooter_barrel_cooling_value;   // 机器人枪口热量每秒冷却值
    u16 shooter_barrel_heat_limit;      // 机器人枪口热量上限
    u16 chassis_power_limit;            // 机器人底盘功率上限
    u8 power_gimbal_output : 1;
    u8 power_chassis_output : 1;
    u8 power_shooter_output : 1;
}robot_status_t;



/// @brief 底盘电源以及枪口热量信息
typedef struct __attribute__((packed)) 
{
    u16 chassis_voltage;                // 底盘出电压
    u16 chassis_current;                // 底盘输出电流
    float chassis_power;                // 底盘功率
    u16 buffer_energy;                  // 缓冲能量
    u16 shooter_17mm_1_barrel_heat;     // 1号巷口热量
    u16 shooter_17mm_2_barrel_heat;     // 2号枪口热量
    u16 shooter_42mm_barrel_heat;       // 42mm枪口热量
}power_heat_data_t;



/// @brief 本车位置坐标
typedef struct __attribute__((packed))
{
    float x;
    float y;
    float angle;            // 枪口朝向角度
}robot_pos_t;




/// @brief 机器人buff信息
typedef struct __attribute__((packed)) 
{
    u8 recovery_buff;                   // 机器人回血增益
    u8 cooling_buff;                    // 机器人枪口冷却增益
    u8 defence_buff;                    // 防御增益
    u8 vulnerability_buff;              // 负防御增益
    u16 attack_buff;                    // 攻击增益
}buff_t;




/// @brief 飞机状态
typedef struct __attribute__((packed)) 
{
    u8 airforce_status;             // 空中机器人冷却状态
    u8 time_remain;                 // 此状态剩余时间
}air_support_data_t;



/// @brief 机器人掉血信息
typedef struct __attribute__((packed)) 
{
    u8 armor_id : 4;                        // 装甲板掉血
    u8 HP_deduction_reason : 4;             // 模块异常掉血
}hurt_data_t;




/// @brief 各个增益点的占领情况
typedef struct __attribute__((packed)) 
{
    u32 rfid_status;
}rfid_status_t;



/// @brief 飞镖控制
typedef struct __attribute__((packed))
{
    u8 dart_launch_opening_status;          // 飞镖发射站状态
    u8 reserved;                            // 没用
    u16 target_change_time;                 // 切换击打目标时候的剩余时间
    u16 latest_launch_cmd_time;             // 最后一次操作手发送发射置零的时候的比赛剩余时间
}dart_client_cmd_t;




/// @brief 己方各个机器人的坐标
typedef struct __attribute__((packed)) 
{
    float hero_x;
    float hero_y;
    float engineer_x;
    float engineer_y;
    float standard_3_x;
    float standard_3_y;
    float standard_4_x;
    float standard_4_y;
    float standard_5_x;
    float standard_5_y;
}ground_robot_position_t;




/// @brief 各个机器人被标记的进度 0-120
typedef struct __attribute__((packed))
{
    u8 mark_hero_progress;
    u8 mark_engineer_progress;
    u8 mark_standard_3_progress;
    u8 mark_standard_4_progress;
    u8 mark_standard_5_progress;
    u8 mark_sentry_progress;
}radar_mark_data_t;




/// @brief 对方雷达的状态
typedef struct __attribute__((packed))
{
    u8 radar_info;
} radar_info_t;







void CP_task(void *pvParameters);

void get_robot_id(u16* ID);             // 获取本机器人的ID
void get_robot_fire_heat(u16* heat,u16* limted);   // 获取本机器人的枪口热量与热量限制 17mm
u16 get_chassis_power_buf(void);                    // 获取底盘缓冲功率

#endif
