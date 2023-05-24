#include "led.h"
/*********************************************************************
* @函数名   : led_gpio_init
* @函数功能 : 连接灯的gpio初始化
* @参数含义 : 无
* @返回值   : 无
* @备注     : PB8,PB9低电平开启
**********************************************************************/
void led_gpio_init(){
//使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Low_Speed;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}