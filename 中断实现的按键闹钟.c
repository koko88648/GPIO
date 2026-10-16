#include "stm32f10x.h"
void GPIO_InIt()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	//LED初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//按键初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//数码管初始化
	  //位选初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	  //段选初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
}
volatile int num =0;
volatile int time= 0;
volatile int state=0;
volatile int flag=0;
volatile uint8_t key2_pressed = 0;
int val[8]={0};
uint8_t seg[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int poscon=0;
int idx=0;
int con =0;
int led_tick = 0; 


void UpdateDisplayBuf(int value) 
{
   val[7] = value % 10;
   val[6] = (value / 10) % 10;
   val[5] = (value / 100) % 10;
   val[4] = (value / 1000) % 10;
   val[3] = (value / 10000) % 10;
   val[2] = (value / 100000) % 10;
   val[1] = (value / 1000000) % 10;
   val[0] = (value / 10000000) % 10;
}
void ScanSegDisplay(void) 
{
   if (++poscon > 200) 
		{
       poscon = 0;
       GPIO_ResetBits(GPIOC, (u16)(0xFF00));
       GPIO_ResetBits(GPIOB, (u16)(0x7000));
       GPIO_SetBits(GPIOB, (u16)(idx << 12));
       GPIO_SetBits(GPIOC, (u16)(seg[val[idx]] << 8));
       if (++idx > 7) idx = 0;
   }
}

int main()
{
	GPIO_InIt();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//k1按键
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	//k2按键
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	//k3按键
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	//k4按键
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	UpdateDisplayBuf(num);
	
	while(1)
   {
       ScanSegDisplay();
       if (key2_pressed && state == 0) 
				{
           key2_pressed = 0;
           time = 0;
           flag = 1;
       }
				//计时显示模式
       if (flag == 1) 
				{
           if (++con > 4500) 
						{
               con = 0;
               time++;
               UpdateDisplayBuf(time);
               
               if (time >= num)
								{
                   flag =2;      
                   led_tick =0;
               }
           }
       }
       //正常显示模式
       if (flag == 0) 
				{
           if (++con > 200) 
						{
               con = 0;
               UpdateDisplayBuf(num);
           }
       }
       //LED闪烁
       if (flag == 2) 
				{
           led_tick++;
           if (led_tick < 50000) 
						{
               GPIO_ResetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
           } 
						else if (led_tick < 100000) 
						{
               GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
           } 
						else 
						{
               led_tick = 0;
           }
       }
   }
}

void EXTI15_10_IRQHandler()
{
		//k1按键
   if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
		{
       state = !state;
       EXTI_ClearITPendingBit(EXTI_Line12);
   }
   //k2按键
   if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
		{
       key2_pressed = 1;
       EXTI_ClearITPendingBit(EXTI_Line13);
   }
   //k3按键
   if(EXTI_GetITStatus(EXTI_Line14) != RESET)
		{
       if(state == 1) num++;
       EXTI_ClearITPendingBit(EXTI_Line14);
   }
   //k4按键
   if(EXTI_GetITStatus(EXTI_Line15) != RESET) 
		{
       if(state == 1) num--;
       EXTI_ClearITPendingBit(EXTI_Line15);
   }
}
