#ifndef _LCD_H
#define _LCD_H
#include "stm32f4xx.h"
#include "systick.h"
#define CS_H GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define CS_L GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define DC_C	GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define DC_D	GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define RES_L	GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define RES_H	GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define LEDK_H GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LEDK_L GPIO_ResetBits(GPIOB,GPIO_Pin_1)
void lcd_gpio_cc_init();
void lcd_spi_init();
u8 lcd_send_recv(u8 data);
void lcd_send_data8(u8 data);
void lcd_send_data16(u16 data);
void lcd_send_command(u16 commond);
void lcd_init();
#endif 