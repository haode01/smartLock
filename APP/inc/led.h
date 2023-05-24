#ifndef _LED_H
#define _LED_H
#include "stm32f4xx.h"

#define LED1_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define LED1_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define LED2_ON GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define LED2_OFF GPIO_SetBits(GPIOB,GPIO_Pin_9)
void led_gpio_init();
#endif 