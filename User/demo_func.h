#ifndef __DEMO_FUNC_H
#define __DEMO_FUNC_H




/*函数定义*/
void freertos_start(void);     //FreeRTOS启动函数
void task_create(void);        //启动任务
void task_led();        //led1任务
void task_key1();        //led2任务
void task_key2();        //key任务
//void task_static_create(void);


#endif