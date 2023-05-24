#include "led.h"
/*********************************************************************
* @������   : led_gpio_init
* @�������� : ���ӵƵ�gpio��ʼ��
* @�������� : ��
* @����ֵ   : ��
* @��ע     : PB8,PB9�͵�ƽ����
**********************************************************************/
void led_gpio_init(){
//ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//��ʼ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Low_Speed;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}