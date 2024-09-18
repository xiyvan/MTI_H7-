/*************************
 * 创建时间 24.04.5
 * @author 韩昂轩(Han Angxvan)
 * 
 * @note 1) 添加了自动获取机器人ID的方法自动匹配客户端
 * 
 *      UI任务
 * ************************
*/


#include "UI_task.h"
#include "string.h"
#include "cmsis_os.h"
#include "cp.h"

///****************************************************  函数声明  **********************************************///

static void UI_Task_Init(void);




///*******************************************************  变量定义  *********************************************///

u16 Robot_ID = 0;       // 机器人ID
u16 Cilent_ID = 0;      // 客户端ID

Graph_Data G1,G2,G3,G4,G5;
Graph_Data ShootNum;        // 发单量指示
uint8_t shoot_Num_b = 45;            // debug
///********************************************************** endl  ***************************************************///


void UI_task(void *pvParameters)
{
    memset(&G1,0,sizeof(G1));
    memset(&G2,0,sizeof(G2));
    memset(&G3,0,sizeof(G3));
    memset(&G4,0,sizeof(G4));
    memset(&G5,0,sizeof(G5));		
    osDelay(1000);               // 确保已经读取到本机器人的ID
    UI_Task_Init();

    // 发单量指示
    Arc_Draw(&G1,"002",UI_Graph_ADD,8,UI_Color_Purplish_red,45,135,1,960,540,395,395);             //  外框
    Arc_Draw(&G2,"003",UI_Graph_ADD,8,UI_Color_Purplish_red,45,135,1,960,540,380,380);
    Line_Draw(&G3,"004",UI_Graph_ADD,8,UI_Color_Purplish_red,1,1230,270,1240,260);
    Arc_Draw(&ShootNum,"005",UI_Graph_ADD,8,UI_Color_Purplish_red,90,135,15,960,540,387,387);
    Line_Draw(&G5,"006",UI_Graph_ADD,8,UI_Color_Purplish_red,1,1230,808,1240,818);
    UI_ReFresh(5,G1,G2,G3,ShootNum,G5);
    memset(&G1,0,sizeof(G1));
    memset(&G2,0,sizeof(G2));
    memset(&G3,0,sizeof(G3));
    memset(&G4,0,sizeof(G4));
    memset(&G5,0,sizeof(G5));
    Rectangle_Draw(&G1,"001",UI_Graph_ADD,0,UI_Color_Green,1,930,537,945,527);
    Line_Draw(&G2,"091",UI_Graph_ADD,9,UI_Color_Purplish_red,1,937,330,937,620); //最后四个 开始XY endXY
    Line_Draw(&G3,"092",UI_Graph_ADD,9,UI_Color_Purplish_red,1,910,470,965,470);
    Line_Draw(&G4,"093",UI_Graph_ADD,9,UI_Color_Purplish_red,1,910,450,965,450);
    Line_Draw(&G5,"094",UI_Graph_ADD,9,UI_Color_Purplish_red,1,910,400,965,400);
    vTaskDelay(50);
    UI_ReFresh(5,G1,G2,G3,G4,G5);
    while (1)
    {
        static uint8_t state;
        Arc_Draw(&ShootNum,"005",UI_Graph_Change,8,UI_Color_Purplish_red,shoot_Num_b,135,15,960,540,387,387);
        UI_ReFresh(1,ShootNum);
        vTaskDelay(100);
        if(shoot_Num_b <= 45)
            state = 1;
        else if(shoot_Num_b >= 134)
            state = 0;
        
        if(state == 1)
            shoot_Num_b ++;
        else if(state == 0)
            shoot_Num_b --;
    }
}













/// @brief UI任务初始化
static void UI_Task_Init(void)
{

///***************************************************  自动设置UI的ID  *********************************///
    get_robot_id(&Robot_ID);        // 机器人ID获取
    switch (Robot_ID)
    {
        case 1:Cilent_ID = 0x0101; break;   // 红1英雄
        case 2:Cilent_ID = 0x0102; break;   // 红2 工程
        case 3:Cilent_ID = 0x0103; break;   // 红3 步兵
        case 4:Cilent_ID = 0x0104; break;   // 红4 步兵
        case 5:Cilent_ID = 0x0105; break;   // 红5 步兵
        case 6:Cilent_ID = 0x0106; break;   // 红6 飞机

        case 101:Cilent_ID = 0x0165; break; // 蓝 1 英雄
        case 102:Cilent_ID = 0x0166; break; // 蓝2 工程
        case 103:Cilent_ID = 0x0167; break; // L 3
        case 104:Cilent_ID = 0x0168; break; // L 4
        case 105:Cilent_ID = 0x0169; break; // L 5
        case 106:Cilent_ID = 0x016A; break; // 蓝 飞机
    }

///**************************************************** endl  ************************************************///


}

