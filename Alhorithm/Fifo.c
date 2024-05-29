/******************************************
    2024/3/2                              
    作者：韩昂轩                       
    1.0  添加了覆盖式FIFO算法       24.3.2

    @note 经过测试，在unbutu中init函数可以正常使用，在stm32中测试，不能正常使用
            其主要原因是，malloc函数不能正常使用，要在stm32中使用的话，需要手动初始化并手动申请空间。
            其他函数可以正常使用。

    FIFO算法包
*******************************************
*/


#include "Fifo.h"


/// @brief 创建一个新的FIFO结构体
/// @param num fifo缓存队列的大小
/// @return 创建的fifo结构体指针
FIFO_t* Fifo_Init(unsigned short num)
{
    FIFO_t* temp = (FIFO_t*)malloc(sizeof(FIFO_t));             // 为其申请内存空间爱你
    if(temp == NULL) return NULL;                               // 判断有没有申请成功
    temp->max = num;
    unsigned char* date = (unsigned char*)malloc(temp->max);    // 为队列申请内存空间
    if(date == NULL)    return NULL;
    temp->queue = date;                                         // 将队列装入结构体当中
    temp->num = 0;                                              // 初始化结构体数据
    temp->add_num = 0;
    temp->get_num = 0;
    return temp;                            // 返回申请的结构体
}



/// @brief 向fifo中添加一个元素
/// @param fifo fifo结构体指针
/// @param date 要加入的数据
void Fifo_Add(FIFO_t* fifo,unsigned char date)
{
    if(fifo == NULL) return;
    fifo->queue[fifo->add_num] = date;          // 将元素放入队列中
    fifo->add_num ++;                           // 将添加数组序号加一
    if(fifo->add_num > (fifo->max -1))          // 如果添加数组序号超过了最大的
    {   
        fifo->add_num = 0;                      // 就让其从零开始
    }
    fifo->num ++;                               // 加入成功让数组中的元素加一
    if(fifo->num > fifo->max) fifo->num = fifo->num;    // 保证数组中的元素数量超不过最大值
}



/// @brief 取出fifo中的一个元素
/// @param fifo fifo结构体指针
/// @return 返回提取出来的数据
unsigned char Fifo_Get(FIFO_t* fifo)
{
    unsigned char temp;
    if(fifo == NULL) return 0;

    if(fifo->num)                               // 如果fifo中的元素不是0
    {
        fifo->num --;
        temp = fifo->queue[fifo->get_num];      // 就提取出来一个元素
        fifo->get_num ++;
        if(fifo->get_num > (fifo->max-1))
        {
            fifo->get_num = 0;
        }
        return temp;
    }
    return 0;
}



/// @brief 向fifo中压入多个数据
/// @param fifo fifo结构体指针
/// @param date 数组
/// @param num 压入个数
void Fifo_AddNum(FIFO_t* fifo,unsigned char* date,unsigned short num)
{
    if(date == NULL) return;
    for(int i = 0;i < num;i++)
    {
        Fifo_Add(fifo,date[i]);
    }
}
