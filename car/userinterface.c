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

/* 变量键盘操作 
    返回 -1   +1   0
*/
int8_t keyboard_vlaue_ops(int16_t *val)
{
  switch (key.ops->get(0))
  {
  case key_m10: *val = *val - 10;break;
  case key_p10: *val = *val + 10;break;
  case key_m1 : *val = *val - 1;break;
  case key_p1 : *val = *val + 1;break;
  case key_mf : return 1;
  case key_pf : return -1;
  default: return 0;
  }
  return 0;
}

void UI_debugsetting(void)
{
  oled.ops->clear();
  
  
}


uint8_t ui_elec_gun_mode(void)
{
  char txt[20];
  uint8_t getkey = 0;
  
  /*----------- 提示信息显示 -----------------*/
  sprintf(txt,"+10 -> base mode");
  oled.ops->word(0,0,(uint8_t*)txt);
  
  sprintf(txt,"-10 -> static fire");
  oled.ops->word(0,1,(uint8_t*)txt);  
  
  sprintf(txt,"-1  -> dynamic fire");
  oled.ops->word(0,2,(uint8_t*)txt);
  
  sprintf(txt,"+1  -> ");
  oled.ops->word(0,3,(uint8_t*)txt);  
  
  sprintf(txt,"-0.1-> voltage mode");
  oled.ops->word(0,4,(uint8_t*)txt);   
  /*------------------------------------------*/
  
  while(1)
  {
    getkey = key.ops->get(0); /* 按键单次获取 */
    if (getkey == key_p10)
      return 1;
    else if(getkey == key_m10)
      return 2;
    else if(getkey == key_m1)
      return 3;
    else if(getkey == key_p1)
      return 4;   
    else if(getkey == key_mf)
      return 5;   
    delayms(10);
  }
}
             
