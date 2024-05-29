#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    unsigned short max;         // 这个fifo的大小
    unsigned short add_num;     // 添加元素序号
    unsigned short get_num;     // 提取元素的序号
    unsigned short num;         // fifo内数据的数量
    unsigned char* queue;       // 存储队列
}FIFO_t;


FIFO_t* Fifo_Init(unsigned short num);
void Fifo_Add(FIFO_t* fifo,unsigned char date);
unsigned char Fifo_Get(FIFO_t* fifo);
void Fifo_AddNum(FIFO_t* fifo,unsigned char* date,unsigned short num);

#endif
