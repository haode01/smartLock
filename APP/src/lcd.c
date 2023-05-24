#include "lcd.h"
/*
模块：LCD屏
作用：显示功能
协议：SPI
物理线：CS
		MOSI，MISO
		D/C
		SCL
需要实现功能(分析得到):
	1.设置写入区域
	2.设置写入区域颜色
	3.写中文（得查）
	4.写英文
	5.写中英结合
	6.写入图片
		
*/

//协议初始化
/*函数功能：初始化cs，d/c线
函数形参：无
函数返回值：无
备注：
PB10->RESET
PB12->CS
PB14->D/C

PB13->SCLK
PB15->MOSI
*/
void lcd_gpio_cc_init(){
	/*
	PB10->RESET
	PB12->CS
	PB14->D/C
	*/
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_ST;
	//输出
	GPIO_ST.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_ST.GPIO_OType=GPIO_OType_PP;
	GPIO_ST.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_ST.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_ST.GPIO_Speed=GPIO_Low_Speed;
	//初始化
	GPIO_Init(GPIOB,&GPIO_ST);
	/*
	PB13->SCLK
	PB15->MOSI
	*/
	GPIO_InitTypeDef GPIO_ST1;
	GPIO_ST1.GPIO_Mode=GPIO_Mode_AF;
	GPIO_ST1.GPIO_OType=GPIO_OType_PP;
	GPIO_ST1.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
	GPIO_ST1.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_ST1.GPIO_Speed=GPIO_Fast_Speed;
	GPIO_Init(GPIOB,&GPIO_ST1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	
}
/*
函数功能：初始化，mcu中的spi控制器
函数形参：无
函数返回值：无
备注：
*/
void lcd_spi_init(){
	lcd_gpio_cc_init();
	//时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	//初始化spi
	SPI_InitTypeDef SPI_Init_St;
	SPI_Init_St.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	SPI_Init_St.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_Init_St.SPI_CPOL=SPI_CPOL_Low;
	SPI_Init_St.SPI_CRCPolynomial=0x07;
	SPI_Init_St.SPI_DataSize=SPI_DataSize_8b;
	SPI_Init_St.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_Init_St.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_Init_St.SPI_Mode=SPI_Mode_Master;
	SPI_Init_St.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI2,&SPI_Init_St);
	SPI_Cmd(SPI2,ENABLE);
	CS_H;
	RES_H;
	LEDK_L;
	
}
//封装发送或者封装数据帧
/*
函数功能：向DR寄存器写入和读取数据，同步位互换这个概念，
函数形参：所发送8位的数据
函数返回值：无
备注：
*/
u8 lcd_send_recv(u8 data){
	//发送
	SPI_I2S_SendData(SPI2,data);
	//等待发送成功
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)));
	//等待接收成功
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)));
	//返回接收到的信息
	return SPI_I2S_ReceiveData(SPI2);
	
}
/*
函数功能：发送八位数据
函数形参：所发送8位的数据
函数返回值：无
备注：
*/
void lcd_send_data8(u8 data){
	CS_L;
	DC_D;
	lcd_send_recv(data);
	CS_H;
	
}
/*
函数功能：发送16位数据
函数形参：所发送8位的数据
函数返回值：无
备注：
*/
void lcd_send_data16(u16 data){
	CS_L;
	DC_D;
	lcd_send_recv(data>>8);
	lcd_send_recv(data);
	CS_H;
	
}
/*
函数功能：发送8位命令
函数形参：所发送8位的数据
函数返回值：无
备注：
*/
void lcd_send_command(u16 commond){
	CS_L;
	DC_C;
	lcd_send_recv(commond);
	CS_H;
	
}
void lcd_init(){
	lcd_spi_init();
	lcd_send_command(0x11);//Sleep exit 
	systick_delay_ms (120);
		
	
	lcd_send_command(0x36);    //RGB、从左到右、从上向下扫描
	lcd_send_data8(0x00);

	lcd_send_command(0x3A);    //16位色深,RGB(565)
	lcd_send_data8(0x05);

	lcd_send_command(0xB2);    //门廊设置
	lcd_send_data8(0x0C);
	lcd_send_data8(0x0C);
	lcd_send_data8(0x00);
	lcd_send_data8(0x33);
	lcd_send_data8(0x33); 

	lcd_send_command(0xB7); 
	lcd_send_data8(0x35);  

	lcd_send_command(0xBB);
	lcd_send_data8(0x19);

	lcd_send_command(0xC0);
	lcd_send_data8(0x2C);

	lcd_send_command(0xC2);
	lcd_send_data8(0x01);

	lcd_send_command(0xC3);
	lcd_send_data8(0x12);   

	lcd_send_command(0xC4);
	lcd_send_data8(0x20);  

	lcd_send_command(0xC6); 
	lcd_send_data8(0x0F);    

	lcd_send_command(0xD0); 
	lcd_send_data8(0xA4);
	lcd_send_data8(0xA1);

	lcd_send_command(0xE0);
	lcd_send_data8(0xD0);
	lcd_send_data8(0x04);
	lcd_send_data8(0x0D);
	lcd_send_data8(0x11);
	lcd_send_data8(0x13);
	lcd_send_data8(0x2B);
	lcd_send_data8(0x3F);
	lcd_send_data8(0x54);
	lcd_send_data8(0x4C);
	lcd_send_data8(0x18);
	lcd_send_data8(0x0D);
	lcd_send_data8(0x0B);
	lcd_send_data8(0x1F);
	lcd_send_data8(0x23);

	lcd_send_command(0xE1);
	lcd_send_data8(0xD0);
	lcd_send_data8(0x04);
	lcd_send_data8(0x0C);
	lcd_send_data8(0x11);
	lcd_send_data8(0x13);
	lcd_send_data8(0x2C);
	lcd_send_data8(0x3F);
	lcd_send_data8(0x44);
	lcd_send_data8(0x51);
	lcd_send_data8(0x2F);
	lcd_send_data8(0x1F);
	lcd_send_data8(0x1F);
	lcd_send_data8(0x20);
	lcd_send_data8(0x23);

	lcd_send_command(0x21); 
	lcd_send_command(0x11); 
	lcd_send_command(0x29);
	

	//打开背光层
	LEDK_H;
	
}