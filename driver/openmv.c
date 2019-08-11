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

uint8_t mv_buff[15];
uint8_t mv_buff_ready = 0;  /* 数据就绪标志位 */
uint16_t target_pix_x = 0;      /* 目标像素位置 */

void openmv_ascii_test(void)
{
  key.init();
  pwm_init();
  oled.init();
  adc.init();
  lpuart1_init(115200);

  uint8_t i;
  char txt[17];
  
  sprintf(txt,"test");
  oled.ops->word(0,0,(uint8_t*)txt);
  
  while(1)
  {
    if (mv_buff_ready)
    {
      oled.ops->word(0,1,(uint8_t*)mv_buff);    
    }
    delayms(10);
  }  
}





