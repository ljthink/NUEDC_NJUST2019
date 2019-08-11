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

