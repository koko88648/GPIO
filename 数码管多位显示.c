#include "stm32f10x.h"

//数码管
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC,ENABLE);
	//段选
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//位选
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
	uint8_t seg[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	u8 val[8] ={0,0,0,0,0,0,0,0};
	u8 idx=0;
	int con=0;
	u8 num = 0;
	u8 poscon = 0;
	while(1)
	{
		//一位显示
//		GPIO_ResetBits(GPIOC,(uint16_t)(0xFF00));//将gpioc的高八位置零
//		GPIO_ResetBits(GPIOB,(uint16_t)(0x7000));//选择14，13，12
//		GPIO_SetBits(GPIOC,(u16)(seg[idx]<<8));//将其送到高八位上，因为高八位是8~15的引脚
//		while(++con>45000)
//		{
//			con=0;
//			if(++idx>9)idx=0;
//		}
		//多位同步显示
//		if(++poscon>100)
//		{
//			poscon = 0;
//			GPIO_ResetBits(GPIOC,(u16)(0xFF00));
//			GPIO_ResetBits(GPIOB,(u16)(0x7000));
//			GPIO_SetBits(GPIOB,(u16)(idx)<<12);
//			GPIO_SetBits(GPIOC,(u16)(seg[num]<<8));
//			if(++idx>7)idx=0;
//		}
//		if(++con>20000)
//		{
//			con=0;
//		if(++num>9)num=0;
		//}
		//多位异步显示
		if(++poscon>200)
		{
			poscon=0;
			GPIO_ResetBits(GPIOC,(u16)(0xFF00));
			GPIO_ResetBits(GPIOB,(u16)(0x7000));
			GPIO_SetBits(GPIOB,(u16)(idx<<12));
			GPIO_SetBits(GPIOC,(u16)(seg[val[idx]]<<8));
			if(++idx>7)idx=0;
			
		}
		if(++con>200000)
		{
			con=0;
			++num;
			val[7]=num%10;
			val[6]=(num/10)%10;
			val[5]=(num/100)%10;
			val[4]=(num/1000)%10;
			val[3]=(num/10000)%10;
			val[2]=(num/100000)%10;
			val[1]=(num/1000000)%10;
			val[0]=(num/10000000)%10;
		}
	}
	
	
}