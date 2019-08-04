/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com:He0L1w  NJUST >
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "system.h"

int main(void)
{
  /* ---------------------      硬件初始化         -------------------------- */
  system_init();        /* MCU初始化 */
  //servo_test();
  //pwm_test();    /* 单个功能测试函数位置 */
  lpuart1_init(115200);         /* 蓝牙发送串口启动 */
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */

  //ExInt_Init();                 /* 中断启动 */
  char txt[16];

  motor.init();         /* 车速PID控制初始化.包含ENC,PWM,PID参数初始化 */ 
  
  UI_debugsetting();
  
  pit_init(kPIT_Chnl_0, 5000); /* 5000us中断 */
  pit_init(kPIT_Chnl_1, 100000); /* 100ms中断 */ 

  while(1)
  {
	  /* ch0中断置位？5ms  */
	  while (status.interrupt_ch0 == 0)
	  {
	  }

    /* 5ms控制一次两个电机转速控制 */
    motor.pidcontrol(&motor_speed);

    /* 100ms更新一次 */
    if (status.interrupt_ch1 == 1 )
    {
      sprintf(txt,"ENC1: %6d ",motor_speed.enc_left); 
      LCD_P6x8Str(0,0,(uint8_t*)txt);
      
      sprintf(txt,"ENC2: %6d ",motor_speed.enc_right);
      LCD_P6x8Str(0,1,(uint8_t*)txt);
      status.interrupt_ch1 = 0; /* 中断复位 */
    }
    
    /* 中断复位 */
    status.interrupt_ch0 = 0;
  }
}

/*
encoder_position_test();
pit_test();
servo_test();
adc.circle_test();
adc.test();
img.roadtype_test();
motor.pidtest();       //电机闭环测试，matlab plot画数据
单个功能调试，函数内自带硬件初始化，都为死循环，复制到预留位置运行即可。

Test_GPIO_ExInt();   //测试GPIO输入及外部中断              按键、中断检测功能    
Test_ANO_DT();       //匿名上位机观察数据波形  不使用DMA模式，请将宏定义#define USE_DMA  0 
encoder_test(); 

*/
