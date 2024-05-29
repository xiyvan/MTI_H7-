
#ifndef _SERVOS_TASK_
#define _SERVOS_TASK_

#include "REMOTE_task.h"
#include "tim.h"

#define SERVOS_ON_PE    1700
#define SERVOS_OFF_PE   2100


typedef struct 
{
    const Remote_type_t* remote_msg;
}servos_type_t;


void Servos_task(void *pvParameters);



#endif
