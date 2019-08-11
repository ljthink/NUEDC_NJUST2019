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



void openmv_data_refresh(void)
{  
  uint8_t i;
  uint16_t distance_queue[5];
  uint16_t pix_x_queue[5];
  uint16_t temp;  
  
  /* 取5次 */
  for (i=0;i<5;i++)
  {
    while(mv_buff_ready == 0) /* 等待数据 */
    {}
    if(mv_buff[0] == '[' && mv_buff[4] == ',')
      pix_x_queue[i] = (mv_buff[1] - '0')*100 + (mv_buff[2] - '0')*10 + (mv_buff[3] - '0');
    if(  mv_buff[8] == ',' && mv_buff[12] == ']')
      distance_queue[i] = (mv_buff[9] - '0')*100 + (mv_buff[10] - '0')*10 + (mv_buff[11] - '0');
  }  
  
  while(mv_buff_ready == 0);
  
  /* 像素位置取平均值 */
  target_pix_x = (pix_x_queue[0] + pix_x_queue[1] + pix_x_queue[2] + pix_x_queue[3] + pix_x_queue[4])/5;
  /* 距离排序后取中间值 */
  for (i = 0; i < 4; i++) 
  {
    for (uint8_t j = i + 1; j <= 4; j++) 
    {// 第i个和第j个比较j可以取到最后一位，所以要用j<=array.length-1
      if (distance_queue[i] > distance_queue[j]) {// 注意和冒泡排序的区别，这里是i和j比较。
        temp = distance_queue[i];
        distance_queue[i] = distance_queue[j];
        distance_queue[j] = temp;
      }
    }  
  }
  target.distance = (float)(distance_queue[1] - 50); 
}



void openmv_data_test(void)
{
  elec_gun.init();      /* 电磁炮初始化 */
  key.init();
  pwm_init();
  oled.init();
  adc.init();
  lpuart1_init(115200);
  char txt[16];
  while(1)
  {
    openmv_data_refresh();
    
    sprintf(txt,"loc:%3d",target_pix_x);
    oled.ops->word(0,1,txt);
    sprintf(txt,"dis:%4.1f",target.distance);
    oled.ops->word(0,2,txt); 
  }
  
}
