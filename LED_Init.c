#include "stm32f10x.h"
 //流水灯与按键控制
int main(void)
{
	//____________________________________________________________________________
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//时钟B,控制LED

   GPIO_InitTypeDef GPIO_InitStructure;//定义一个
	
		//配置参数
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//接0，1，2，3四个引脚
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置为推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//控制快慢
	
	
   GPIO_Init(GPIOB, &GPIO_InitStructure);//将自己定义的GPIO_Initstructure与GPIOB完成绑定
//_______________________________________________________________________________________
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟A,控制按键
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//————————————————————————————————————————————————	
	
	
	
	
	//流水灯
   uint16_t pin[4] = { GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3 };//引脚数组
   uint8_t idx = 0;
		GPIO_SetBits(GPIOB,GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	//按键
	  //高低引脚电平
		u8 lstLevel = 1;
		u8 curLever = 1;
	
   while (1)
   {
			
       //点亮当前灯
       //GPIO_ResetBits(GPIOB, pin[idx]);//resetbits为点亮
      // GPIO_SetBits(GPIOB, pin[(idx+1)%4] | pin[(idx+2)%4] | pin[(idx+3)%4]);//setbits为熄灭
       //if(++idx >= 4) idx = 0;
       //for(int i=0; i<1000000; i++)
			
				//检查按键
				//if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12))
					//GPIO_SetBits(GPIOB,GPIO_Pin_0);
				//else
					//GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			
			//K1按键控制四个LED亮灭
			   //按键长安无效
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)//低电平检测
			{
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
				{
					curLever = 0;
				}
				for(int i=0; i<5000; i++);//时间缓冲
			}
			else                                          //高电平检测
			{
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1)
				{
					curLever = 1;
				}
				for(int i=0; i<5000; i++);//时间缓冲
			}
			if(lstLevel==1 &&curLever ==0) //高低电平
			{
				GPIO_ResetBits(GPIOB, pin[idx]);
				GPIO_SetBits(GPIOB, pin[(idx+1)%4] | pin[(idx+2)%4] | pin[(idx+3)%4]);
				if(++idx >= 4) idx = 0;
			}
			lstLevel = curLever;			
   }
}

