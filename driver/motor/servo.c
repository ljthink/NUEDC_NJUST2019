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

/* 两个舵机的控制数组 */
uint16_t servo_highpulse[2] = {SERVO_MID,SERVO_MID} ; /* 舵机控制变量 */
angle_data_t   ServoAngle = { 
                              .Pitch = 0,
                              .Yaw   = 0,
                              };                 /* 初始偏航、俯仰0° */

/**
 *  舵机测试
 *  ----------------
 *  中值3000 频率200hz
 *  
 */
void servo_test(void)
{
  char txt[16];
  char txtnull[16];
 
  oled.init();
  oled.ops->clear();
  key.init();
  pwm_init();

  sprintf(txt, "->Pich:");
  LCD_P6x8Str(0,0,(uint8_t*)txt); 
  sprintf(txt, "  Yaw :");
  LCD_P6x8Str(0,1,(uint8_t*)txt); 
  sprintf(txtnull, "  ");
   
  uint8_t choose_falg = 0;
  uint8_t pwm_choose = 0;
  while (1)
  {
    switch(key.ops->get(1))  //检测按键
    {
    case no_key:
      break;
    case key_minus:
      pwm_choose == 0 ? ServoAngle.Pitch-- : ServoAngle.Yaw--;
      break; 
    case key_plus:      
      pwm_choose == 0 ? ServoAngle.Pitch++ : ServoAngle.Yaw++;
      break;
    case key_ok: /* 选项切换 */
      choose_falg =  (choose_falg ==0 );
      sprintf(txt, "->");
      if (choose_falg)
      {
        LCD_P6x8Str(0,0,(uint8_t*)txt);
        LCD_P6x8Str(0,1,(uint8_t*)txtnull);
        pwm_choose = 0;
      }
      else
      {
        LCD_P6x8Str(0,0,(uint8_t*)txtnull);
        LCD_P6x8Str(0,1,(uint8_t*)txt); 
        pwm_choose = 1;
      }
      break;
    }
    sprintf(txt,"%4d",ServoAngle.Pitch);
    LCD_P6x8Str(50,0,(uint8_t*)txt);
    sprintf(txt,"%4d",ServoAngle.Yaw);
    LCD_P6x8Str(50,1,(uint8_t*)txt);
    angle_servo(&ServoAngle);
    delayms(100);
  }
}

