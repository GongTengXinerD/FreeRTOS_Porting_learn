/*功能*/
//任务一：实现红灯亮
//任务二：按下key1后仅蓝灯亮
//任务三：key2按下后仅绿灯亮
#include "stdio.h"
#include "demo_func.h"
#include "bsp_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal_gpio.h"

/*启动任务参数*/
#define START_TASK_STACKDEPTH 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;

/*led任务参数*/
#define LED1_TASK_STACKDEPTH 128
#define LED1_TASK_PRIORITY 2
TaskHandle_t led1_task_handle;

/*key1任务参数*/
#define LED2_TASK_STACKDEPTH 128
#define LED2_TASK_PRIORITY 3
TaskHandle_t led2_task_handle;

/*key2任务参数*/
#define KEY_TASK_STACKDEPTH 128
#define KEY_TASK_PRIORITY 4
TaskHandle_t key_task_handle;

/*LED状态标志*/
#define LED_STATE_RED    0
#define LED_STATE_BLUE   1
#define LED_STATE_GREEN  2
volatile uint8_t led_state = LED_STATE_RED;  // 初始状态为红灯

/*静态创建参数,仅仅启动函数使用静态创建演示*/

/*启动任务参数*/
/*
#define START_TASK_STATIC_STACKDEPTH 128
#define START_TASK_STATIC_PRIORITY 1
TaskHandle_t start_task_static_handle;
StackType_t start_task_stackbuff[START_TASK_STATIC_STACKDEPTH];
StaticTask_t start_task_pcb;
*/

//创建启动任务并启动调度器
void freertos_start(void)
{
  /*创建启动任务*/
  
  xTaskCreate((TaskFunction_t) task_create,         // 任务函数名   
                            "task_create",          // 任务别名
                            START_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            START_TASK_PRIORITY,
                            &start_task_handle);
  
  /*静态创建数组*/
  /*
  start_task_static_handle = xTaskCreateStatic((TaskHookFunction_t) task_static_create,
                  "task_static_create",
                  START_TASK_STATIC_STACKDEPTH,
                  (void *) NULL,
                  START_TASK_STATIC_PRIORITY,
                  start_task_stackbuff,
                  &start_task_pcb);
  */
  vTaskStartScheduler();
}

void task_create()
{
    taskENTER_CRITICAL();           //进入临界区

    xTaskCreate((TaskFunction_t) task_led,         // 任务函数名   
                            "task_led",          // 任务别名
                            LED1_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            LED1_TASK_PRIORITY,
                            &led1_task_handle);
  
    xTaskCreate((TaskFunction_t) task_key1,         // 任务函数名   
                            "task_key1",          // 任务别名
                            LED2_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            LED2_TASK_PRIORITY,
                            &led2_task_handle);

    xTaskCreate((TaskFunction_t) task_key2,         // 任务函数名   
                            "task_key2",          // 任务别名
                            KEY_TASK_STACKDEPTH,             //任务栈大小
                            (void *) NULL,          //传递给任务的参数
                            KEY_TASK_PRIORITY,
                            &key_task_handle);
    taskEXIT_CRITICAL();                 //退出临界区
    vTaskDelete(NULL);                   //启动任务使命完成，直接删除，上述功能任务创建完成后会直接进行运行
}
//任务一：实现红灯亮
void task_led()
{
    // 初始化：确保初始状态为红灯亮
    taskENTER_CRITICAL();
    led_state = LED_STATE_RED;
    control_led(LEDR_GPIO_PORT, LEDR_PIN, ON);  // 红灯亮
    control_led(LEDB_GPIO_PORT, LEDB_PIN, OFF);
    control_led(LEDG_GPIO_PORT, LEDG_PIN, OFF);
    taskEXIT_CRITICAL();
    
    while(1)
    {
        taskENTER_CRITICAL();  // 进入临界区
        // 只有在RED状态时才让红灯亮，其他状态时保持红灯关闭
        if(led_state == LED_STATE_RED)
        {
            control_led(LEDR_GPIO_PORT, LEDR_PIN, ON);  // 红灯亮
            control_led(LEDB_GPIO_PORT, LEDB_PIN, OFF);
            control_led(LEDG_GPIO_PORT, LEDG_PIN, OFF);
        }
        else
        {
            // 非RED状态时，确保红灯关闭（由其他任务控制LED）
            control_led(LEDR_GPIO_PORT, LEDR_PIN, OFF);
        }
        taskEXIT_CRITICAL();  // 退出临界区
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


//任务二：按下key1后仅蓝灯亮
void task_key1()
{
    // // 启动时延时，等待GPIO状态稳定，避免误判
    // vTaskDelay(pdMS_TO_TICKS(100));
    
    // // 如果启动时按键处于按下状态，等待其释放
    // while(state_key(KEY1_GPIO_PORT, KEY1_PIN) == ON)
    // {
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }
    
    while(1)
    {
        int state;
        state = state_key(KEY1_GPIO_PORT, KEY1_PIN);
        if(state == ON)
        {
            // 按键消抖：延时10ms后再次检测
            vTaskDelay(pdMS_TO_TICKS(10));
            state = state_key(KEY1_GPIO_PORT, KEY1_PIN);
            if(state == ON)
            {
                taskENTER_CRITICAL();  // 进入临界区
                // 按下key1后，关闭所有灯，只让蓝灯亮
                led_state = LED_STATE_BLUE;  // 改变状态标志
                control_led(LEDR_GPIO_PORT, LEDR_PIN, OFF);  // 关闭红灯
                control_led(LEDB_GPIO_PORT, LEDB_PIN, ON);   // 蓝灯亮
                control_led(LEDG_GPIO_PORT, LEDG_PIN, OFF);  // 关闭绿灯
                taskEXIT_CRITICAL();  // 退出临界区
                // 等待按键释放
                while(state_key(KEY1_GPIO_PORT, KEY1_PIN) == ON)
                {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));  // 缩短检测周期，提高响应速度
    }
}

//任务三：key2按下后仅绿灯亮
void task_key2()
{
    // // 启动时延时，等待GPIO状态稳定，避免误判
    // vTaskDelay(pdMS_TO_TICKS(100));
    
    // // 如果启动时按键处于按下状态，等待其释放
    // while(state_key(KEY2_GPIO_PORT, KEY2_PIN) == ON)
    // {
    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }
    
    while(1)
    {
        int state;
        state = state_key(KEY2_GPIO_PORT, KEY2_PIN);
        if(state == ON)
        {
            // 按键消抖：延时10ms后再次检测
            vTaskDelay(pdMS_TO_TICKS(10));
            state = state_key(KEY2_GPIO_PORT, KEY2_PIN);
            if(state == ON)
            {
                taskENTER_CRITICAL();  // 进入临界区
                // 按下key2后，关闭所有灯，只让绿灯亮
                led_state = LED_STATE_GREEN;  // 改变状态标志
                control_led(LEDR_GPIO_PORT, LEDR_PIN, OFF);  // 关闭红灯
                control_led(LEDB_GPIO_PORT, LEDB_PIN, OFF);  // 关闭蓝灯
                control_led(LEDG_GPIO_PORT, LEDG_PIN, ON);   // 绿灯亮
                taskEXIT_CRITICAL();  // 退出临界区
                // 等待按键释放
                while(state_key(KEY2_GPIO_PORT, KEY2_PIN) == ON)
                {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));  // 缩短检测周期，提高响应速度
    }
}
