/**************************************************
 * @brief 裁判系统串口中间包 用来配置裁判系统所需要的串口 
 * @author 韩昂轩 (Han Angxvan)
 * 
 * 
 * @note 1.0.0 添加了裁判系统串口的初始化函数  24.3.30
 * 
****************************************************
*/


#include "cp_usart.h"
#include "Fifo.h"
#include "usart.h"



extern FIFO_t CP_Fifo;





/// @brief 用来对串口接收过来的信号进行运算（此处把接受过来的信息全部放入fifo中等待串口任务解析）
/// @param len 串口接收信号长度
/// @param date 接受数据
void CP_usart_task(u16 len,u8* date)
{
    // 将串口中接受的数据压入fifo中等待解析
    Fifo_AddNum(&CP_Fifo,date,len);
}




