#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx.h"

/*LED的PIN*/
#define LEDR_PIN GPIO_PIN_6
#define LEDR_GPIO_PORT GPIOF

#define LEDG_PIN GPIO_PIN_7
#define LEDG_GPIO_PORT GPIOF

#define LEDB_PIN GPIO_PIN_8
#define LEDB_GPIO_PORT GPIOF

/*KEY的PIN*/
#define KEY1_PIN GPIO_PIN_0
#define KEY1_GPIO_PORT GPIOA

#define KEY2_PIN GPIO_PIN_13
#define KEY2_GPIO_PORT GPIOC

/*LED开关*/
#define ON GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/*GPIO硬件配置*/

void LED_GPIO_Config(void);
void KEY_GPIO_Config(void);



/*open led*/
void control_led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
int state_key(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/*关闭所有灯*/
void closeAllLed(void);






#endif