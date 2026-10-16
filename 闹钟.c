#include "stm32f10x.h"
	//定时闹钟
static void Delay_us(uint32_t us)
{
   volatile uint32_t i = us * 8;
   while(i--);
}

int main(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   // 时钟使能
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
   
   // 禁用JTAG，释放PA13/PA14作为普通GPIO
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
   
   // LED初始化 PB0~PB3
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); // 初始熄灭
   
   //按键初始化 PA12~PA15 上拉输入
   GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   // 数码管段选 PC8~PC15
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
   GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   
   //数码管位选 PB12~PB14 (74LS138 C/B/A)
   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   
   // 共阴极段码表 0-9
   const uint8_t seg[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
   
   uint16_t set_time  = 0;     // 设定闹钟时间
   uint16_t cur_time  = 0;     // 当前计时
   uint8_t  set_mode  = 0;     // 0:正常模式 1:设置模式
   uint8_t  start_flag = 0;    // 定时启动标志
   uint32_t time_cnt  = 0;     // 软件计时计数器
   uint8_t  alarm_flag = 0;    // 闹钟到达标志
   
   uint8_t key_val = 0;
   uint8_t buf[4];             // 4位数码缓存
   
   //按键扫描宏
   #define SCAN_KEY(pin, val) do {                          \
       if(GPIO_ReadInputDataBit(GPIOA, pin) == 0)           \
       {                                                    \
           Delay_us(50);                                   \
           if(GPIO_ReadInputDataBit(GPIOA, pin) == 0)       \
           {                                                \
               key_val = (val);                             \
               while(GPIO_ReadInputDataBit(GPIOA, pin) == 0);\
           }                                                \
       }                                                    \
   } while(0)
   
   while(1)
   {
       //按键扫描
       key_val = 0;
       SCAN_KEY(GPIO_Pin_12, 1); // K1
       SCAN_KEY(GPIO_Pin_13, 2); // K2
       SCAN_KEY(GPIO_Pin_14, 3); // K3
       SCAN_KEY(GPIO_Pin_15, 4); // K4
       
       //按键逻辑
       if(key_val == 1) // K1：进入/退出设置模式
       {
           set_mode = !set_mode;
           start_flag = 0;
           alarm_flag = 0;
       }
       
       if(set_mode == 1) // 设置模式
       {
           if(key_val == 3 && set_time < 9999) set_time++; // K3 +1
           if(key_val == 4 && set_time > 0)    set_time--; // K4 -1
       }
       else // 正常模式
       {
           if(key_val == 2) // 启动定时 / 清除报警
           {
               if(alarm_flag)
               {
                   alarm_flag = 0; // 清除报警
               }
               else
               {
                   start_flag = 1;
                   cur_time   = 0;
                   time_cnt   = 0;
                   alarm_flag = 0;
               }
           }
           
           if(start_flag == 1)
           {
               time_cnt++;
               if(time_cnt >= 450) // 软件计数模拟1秒
               {
                   time_cnt = 0;
                   cur_time++;
               }
               if(cur_time >= set_time && set_time > 0) // 计时到达
               {
                   start_flag = 0;
                   alarm_flag = 1;
               }
           }
       }
       
       // 数码管动态显示
       uint16_t disp_dat = set_mode ? set_time : cur_time;
       
       buf[0] = disp_dat / 1000;
       buf[1] = disp_dat % 1000 / 100;
       buf[2] = disp_dat % 100 / 10;
       buf[3] = disp_dat % 10;
       
       for(uint8_t i = 0; i < 4; i++)
       {
           //消隐
           GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
           
           // 输出段码
           GPIOC->ODR = (GPIOC->ODR & 0x00FF) | ((uint16_t)seg[buf[i]] << 8);
           
           // 输出位选地址到74LS138
           if(i & 0x04) GPIO_SetBits(GPIOB, GPIO_Pin_14);   else GPIO_ResetBits(GPIOB, GPIO_Pin_14); // C
           if(i & 0x02) GPIO_SetBits(GPIOB, GPIO_Pin_13);   else GPIO_ResetBits(GPIOB, GPIO_Pin_13); // B
           if(i & 0x01) GPIO_SetBits(GPIOB, GPIO_Pin_12);   else GPIO_ResetBits(GPIOB, GPIO_Pin_12); // A
           
           // 短延时维持显示
           Delay_us(50);
       }
       
       //扫描结束后消隐
       GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
       
       //闹钟LED闪烁
       if(alarm_flag == 1)
       {
           GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
           Delay_us(5000);
           GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
           Delay_us(5000);
       }
   }
}

