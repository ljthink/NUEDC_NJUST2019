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
  case key1: *val = *val - 10;break;
  case key2: *val = *val + 10;break;
  case key3: *val = *val - 1;break;
  case key4: *val = *val + 1;break;
  case key5: return 1;
  case key6: return -1;
  default: return 0;
  }
  return 0;
}

void UI_debugsetting(void)
{
  oled.ops->clear();
  
  
}

