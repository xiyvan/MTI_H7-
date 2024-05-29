#ifndef _PID_H_
#define _PID_H_

#include "main.h"







typedef struct 
{
    float kp;
    float ki;
    float kd;

    float max_out;
    float maxi_out;

    float p_out;
    float i_out;
    float d_out;
    float out;
    float erro[3];
    float d_buffer[3];

    float set;
    float fdb;

}PID_type_def;



float PID_cale(PID_type_def* pid_calc,float set,float feekbak);
void PID_Init(PID_type_def* pid_init,float* param,float MAX_OUT,float MAXI_OUT);

#endif
