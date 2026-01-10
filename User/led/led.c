//实现led1闪烁，间隔500ms
//实现led2闪烁，间隔500ms
//实现按下按键1，led2停止当前状态

#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

/*启动任务参数*/
#define START_TASK_STACKDEPTH 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;

/*led1任务参数*/
#define LED1_TASK_STACKDEPTH 128
#define LED1_TASK_PRIORITY 1
TaskHandle_t led1_task_handle;

/*led2任务参数*/
#define LED2_TASK_STACKDEPTH 128
#define LED2_TASK_PRIORITY 1
TaskHandle_t led2_task_handle;

/*按键任务参数*/
#define KEY_TASK_STACKDEPTH 128
#define KEY_TASK_PRIORITY 1
TaskHandle_t key_task_handle;

/*静态创建参数,仅仅启动函数使用静态创建演示*/

/*启动任务参数*/
#define START_TASK_STATIC_STACKDEPTH 128
#define START_TASK_STATIC_PRIORITY 1
TaskHandle_t start_task_static_handle;
StackType_t start_task_stackbuff[START_TASK_STATIC_STACKDEPTH];
StaticTask_t start_task_pcb;


//创建启动任务并启动调度器
void freertos_start(void)
{
  /*创建启动任务*/
  /*
  xTaskCreate((TaskFunction_t) task_create,         // 任务函数名   
                            "task_create",          // 任务别名
                            START_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            START_TASK_PRIORITY,
                            &start_task_handle);
  */
  /*静态创建数组*/
  start_task_static_handle = xTaskCreateStatic((TaskHookFunction_t) task_static_create,
                  "task_static_create",
                  START_TASK_STATIC_STACKDEPTH,
                  (void *) NULL,
                  START_TASK_STATIC_PRIORITY,
                  start_task_stackbuff,
                  &start_task_pcb);
  vTaskStartScheduler();
}

void task_create()
{
  xTaskCreate((TaskFunction_t) task_led1,         // 任务函数名   
                            "task_create",          // 任务别名
                            LED1_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            LED1_TASK_PRIORITY,
                            &led1_task_handle);

  xTaskCreate((TaskFunction_t) task_led2,         // 任务函数名   
                            "task_create",          // 任务别名
                            LED2_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            LED2_TASK_PRIORITY,
                            &led2_task_handle);

  xTaskCreate((TaskFunction_t) task_key,         // 任务函数名   
                            "task_create",          // 任务别名
                            KEY_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            KEY_TASK_PRIORITY,
                            &key_task_handle);
}

void task_led1()
{
    

}

void task_led2()
{


}

void task_key()
{


}