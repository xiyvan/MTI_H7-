/******************************************
 * @author 韩昂轩
 * 创建时间： 24.5.2
 * 
 *   遥控器解码任务（中断任务）
******************************************
*/


#include "REMOTE_task.h"
#include "string.h"
#include "usart.h"

#include "CK_Timeout_task.h"

/// @brief 遥控器解码函数
static void Remote_decode(u8* usart3_rx_data_buf);


Remote_type_t remote_main;



/// @brief 遥控器任务
/// @param len 本次接受数据的长度
/// @param date 数据指针
void REMOTE_TASK(u16 len,u8* date)
{
	// 当长度相当的时候才进行解析
	if(len == REMOTE_RECEVER_LEN)
	{
		// 遥控器信息解码
		Remote_decode(date);

		// 掉线检测回调
		CkTime_DriverTimeNew(TIMEOUT_REMOTE_TIMEOUT);
	}
}





/// @brief 遥控器解码函数
static void Remote_decode(u8* usart3_rx_data_buf)
{
    remote_main.channel_0 = (usart3_rx_data_buf[0] | (usart3_rx_data_buf[1] << 8)) & 0x07ff;        //!< Channel 0
    remote_main.channel_1 = ((usart3_rx_data_buf[1] >> 3) | (usart3_rx_data_buf[2] << 5)) & 0x07ff; //!< Channel 1
    remote_main.channel_2 = ((usart3_rx_data_buf[2] >> 6) | (usart3_rx_data_buf[3] << 2) |          //!< Channel 2
                            (usart3_rx_data_buf[4] << 10)) &0x07ff;
    remote_main.channel_3 = ((usart3_rx_data_buf[4] >> 1) | (usart3_rx_data_buf[5] << 7)) & 0x07ff; //!< Channel 3
    remote_main.S1 = ((usart3_rx_data_buf[5] >> 4) & 0x0003);                  //!< Switch left
    remote_main.S2 = ((usart3_rx_data_buf[5] >> 4) & 0x000C) >> 2;                       //!< Switch right
    remote_main.mouse_x = usart3_rx_data_buf[6] | (usart3_rx_data_buf[7] << 8);                    //!< Mouse X axis
    remote_main.mouse_y = usart3_rx_data_buf[8] | (usart3_rx_data_buf[9] << 8);                    //!< Mouse Y axis
    remote_main.mouse_z = usart3_rx_data_buf[10] | (usart3_rx_data_buf[11] << 8);                  //!< Mouse Z axis
    remote_main.mouse_left = usart3_rx_data_buf[12];                                  //!< Mouse Left Is Press ?
    remote_main.mouse_right = usart3_rx_data_buf[13];                                  //!< Mouse Right Is Press ?
    remote_main.key = usart3_rx_data_buf[14] | (usart3_rx_data_buf[15] << 8);                    //!< KeyBoard value
    remote_main.channel_4 = usart3_rx_data_buf[16] | (usart3_rx_data_buf[17] << 8);                 //NULL

    remote_main.channel_0 = remote_main.channel_0 - REMOTE_MID_DATA;
    remote_main.channel_1 = remote_main.channel_1 - REMOTE_MID_DATA;
    remote_main.channel_2 = remote_main.channel_2 - REMOTE_MID_DATA;
    remote_main.channel_3 = remote_main.channel_3 - REMOTE_MID_DATA;
    remote_main.channel_4 = remote_main.channel_4 - REMOTE_MID_DATA;
}


/// @brief 遥控器信息指针获取
/// @return 遥控器信息指针
Remote_type_t* get_remote_msg_p(void)
{
    return &remote_main;
}

