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
 
 openmv_data_test();

 elec_gun.init();      /* 电磁炮初始化 */
 key.init();
 pwm_init();
 oled.init();
 adc.init();
 lpuart1_init(115200);

 angle_servo(&target);
 
 uint8_t gun_mode;
 
 while(1)
 {  
   gun_mode = ui_elec_gun_mode();  /* 按键选择电磁炮运行模式 */  
   oled.ops->clear();              /* 清除屏幕内容 */
   switch (gun_mode)
   {
   case 1:  elec_gun.mode1(); break;
   case 2:  elec_gun.mode2(); break;
   case 3:  elec_gun.mode3(); break;
   case 4:  elec_gun.mode4(); break;
   case 5:  elec_gun.mode5(); break;
   default : break;
   }
 }
}




/*
 //key_test();
 //elec_charge_test();
 //elec_cap_voltage_test();
 //encoder_distance_test();
 //hwt_905_test();
 //encoder_test();
 //motor.pidtest();
 //servo_test();
 //pwm_test();

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
