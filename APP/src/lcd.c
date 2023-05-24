#include "lcd.h"
/*
ģ�飺LCD��
���ã���ʾ����
Э�飺SPI
�����ߣ�CS
		MOSI��MISO
		D/C
		SCL
��Ҫʵ�ֹ���(�����õ�):
	1.����д������
	2.����д��������ɫ
	3.д���ģ��ò飩
	4.дӢ��
	5.д��Ӣ���
	6.д��ͼƬ
		
*/

//Э���ʼ��
/*�������ܣ���ʼ��cs��d/c��
�����βΣ���
��������ֵ����
��ע��
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
	//ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_ST;
	//���
	GPIO_ST.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_ST.GPIO_OType=GPIO_OType_PP;
	GPIO_ST.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_ST.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_ST.GPIO_Speed=GPIO_Low_Speed;
	//��ʼ��
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
�������ܣ���ʼ����mcu�е�spi������
�����βΣ���
��������ֵ����
��ע��
*/
void lcd_spi_init(){
	lcd_gpio_cc_init();
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	//��ʼ��spi
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
//��װ���ͻ��߷�װ����֡
/*
�������ܣ���DR�Ĵ���д��Ͷ�ȡ���ݣ�ͬ��λ����������
�����βΣ�������8λ������
��������ֵ����
��ע��
*/
u8 lcd_send_recv(u8 data){
	//����
	SPI_I2S_SendData(SPI2,data);
	//�ȴ����ͳɹ�
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)));
	//�ȴ����ճɹ�
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)));
	//���ؽ��յ�����Ϣ
	return SPI_I2S_ReceiveData(SPI2);
	
}
/*
�������ܣ����Ͱ�λ����
�����βΣ�������8λ������
��������ֵ����
��ע��
*/
void lcd_send_data8(u8 data){
	CS_L;
	DC_D;
	lcd_send_recv(data);
	CS_H;
	
}
/*
�������ܣ�����16λ����
�����βΣ�������8λ������
��������ֵ����
��ע��
*/
void lcd_send_data16(u16 data){
	CS_L;
	DC_D;
	lcd_send_recv(data>>8);
	lcd_send_recv(data);
	CS_H;
	
}
/*
�������ܣ�����8λ����
�����βΣ�������8λ������
��������ֵ����
��ע��
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
		
	
	lcd_send_command(0x36);    //RGB�������ҡ���������ɨ��
	lcd_send_data8(0x00);

	lcd_send_command(0x3A);    //16λɫ��,RGB(565)
	lcd_send_data8(0x05);

	lcd_send_command(0xB2);    //��������
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
	

	//�򿪱����
	LEDK_H;
	
}