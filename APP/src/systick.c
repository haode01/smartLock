#include "systick.h"
//������
void systick_delay_ms(u32 xms){
	while(xms--){
	SysTick->CTRL&=~(1<<2);//10.5Mѡ��ʱ��
	SysTick->LOAD=10500-1;//1ms
	SysTick->VAL=0;
	SysTick->CTRL|=1;//����
	while(!(SysTick->CTRL&1<<16));
	SysTick->CTRL&=~1;//�ر�
}

}
