#include "systick.h"
//有问题
void systick_delay_ms(u32 xms){
	while(xms--){
	SysTick->CTRL&=~(1<<2);//10.5M选择时钟
	SysTick->LOAD=10500-1;//1ms
	SysTick->VAL=0;
	SysTick->CTRL|=1;//开启
	while(!(SysTick->CTRL&1<<16));
	SysTick->CTRL&=~1;//关闭
}

}
