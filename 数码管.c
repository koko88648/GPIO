#include "stm32f10x.h"
//共阴极数码管段码 0~9
uint8_t seg[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

int main(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   uint8_t idx = 0;    //显示数字下标0~9
   int count = 0;      //软件计数延时

   //==== 1. PB12 PB13 PB14 位选初始化（74HC138）====
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   //==== 2. PC8~PC15 段选初始化【重点】====
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11
                               |GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   while(1)
   {
       //消隐：PC8~PC15全部清零，熄灭数码管
       GPIO_ResetBits(GPIOC, 0xFF00);

       //位选 PB12 PB13 PB14 = 000，选中Y0第0个数码管
       GPIO_ResetBits(GPIOB, 0x7000);

       //段码左移8位，输出到PC8~PC15
       GPIO_Write(GPIOC, (GPIO_ReadOutputData(GPIOC)&~0xFF00) | (seg[idx] << 8));

       //计数延时，切换数字
       if(++count > 50000)
       {
           count = 0;
           if(++idx >9) idx =0;
       }
   }
}
