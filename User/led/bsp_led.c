#include "bsp_led.h"

//控制LED的启闭
void control_led(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

 //检测KEY是否被按下
int state_key(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  int state;
  state = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
  return state;
}

void closeAllLed(void)
{
  control_led(LEDR_GPIO_PORT, LEDR_PIN, OFF);
  control_led(LEDG_GPIO_PORT, LEDG_PIN, OFF);
  control_led(LEDB_GPIO_PORT, LEDB_PIN, OFF);
}

//LED的GPIO硬件配置
void LED_GPIO_Config(void)
{
  //定义结构体
  GPIO_InitTypeDef LED_GPIO_Initstruct;
  //启动相关GPIO时钟
  __GPIOF_CLK_ENABLE();
  //设置推免输出
  LED_GPIO_Initstruct.Mode = GPIO_MODE_OUTPUT_PP;
  //设置上拉模式
  LED_GPIO_Initstruct.Pull = GPIO_PULLUP;
  //引脚速率为高速
  LED_GPIO_Initstruct.Speed = GPIO_SPEED_HIGH;
  //配置红灯引脚
  LED_GPIO_Initstruct.Pin = LEDR_PIN;
  HAL_GPIO_Init(LEDR_GPIO_PORT, &LED_GPIO_Initstruct);
  //配置绿灯引脚
  LED_GPIO_Initstruct.Pin = LEDG_PIN;
  HAL_GPIO_Init(LEDG_GPIO_PORT, &LED_GPIO_Initstruct);
  //配置蓝灯引脚
  LED_GPIO_Initstruct.Pin = LEDB_PIN;
  HAL_GPIO_Init(LEDB_GPIO_PORT, &LED_GPIO_Initstruct);
  //关闭LED，保证led灯初始状态为关闭，因为配置完成后的状态是不定的
  closeAllLed();
}

//KEY的GPIO硬件配置
void KEY_GPIO_Config(void)
{
  //定义结构体
  GPIO_InitTypeDef KEY_GPIO_Initstruct;
  //启动相关GPIO时钟
  __GPIOA_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  //设置推免输出
  KEY_GPIO_Initstruct.Mode = GPIO_MODE_INPUT;
  //设置上拉模式
  KEY_GPIO_Initstruct.Pull = GPIO_NOPULL;
  //引脚速率为高速
  KEY_GPIO_Initstruct.Speed = GPIO_SPEED_HIGH;
  //配置KEY1
  KEY_GPIO_Initstruct.Pin = KEY1_PIN;
  HAL_GPIO_Init(KEY1_GPIO_PORT, &KEY_GPIO_Initstruct);
  //配置KEY2
  KEY_GPIO_Initstruct.Pin = KEY2_PIN;
  HAL_GPIO_Init(KEY2_GPIO_PORT, &KEY_GPIO_Initstruct);
}
