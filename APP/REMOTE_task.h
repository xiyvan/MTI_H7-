#ifndef _REMOTE_TASK_H_
#define _REMOTE_TASK_H_

#include "main.h"

#define REMOTE_RECEVER_LEN 18       /*遥控器一帧数据长度*/

#define REMOTE_MID_DATA 1024
#define REMOTE_MAX_DATA 1684
#define REMOTE_MIN_DATA 364


#define KEY_PRESSED_OFFSET_W            ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R            ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F            ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G            ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z            ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X            ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C            ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V            ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B            ((uint16_t)1 << 15)

#define REMOVE_S_DOWN 2
#define REMOVE_S_MID 3
#define REMOVE_S_UP 1


typedef struct 
{
    s16 channel_0;
    s16 channel_1;
    s16 channel_2;
    s16 channel_3;
    s16 channel_4;
    u8 S1;
    u8 S2;

    s16 mouse_x;
    s16 mouse_y;
    s16 mouse_z;
    u8 mouse_left;
    u8 mouse_right;
    u16 key;
}Remote_type_t;



Remote_type_t* get_remote_msg_p(void);
void REMOTE_TASK(u16 len,u8* date);

#endif
