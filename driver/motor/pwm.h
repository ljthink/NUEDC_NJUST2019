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
#ifndef _PWM_H
#define _PWM_H


/*-------------------            OLED硬件接口       -------------------*/

///*-------------- 龙邱母版引脚 --------------*/
/* PWM1 */
#define PWM_OUT1_PINMUX     IOMUXC_GPIO_SD_B1_00_FLEXPWM1_PWMA03        /* L5 */
/* PWM2 */
#define PWM_OUT2_PINMUX     IOMUXC_GPIO_SD_B1_01_FLEXPWM1_PWMB03        /* M5 */
/* PWM3 */
#define PWM_OUT3_PINMUX     IOMUXC_GPIO_B0_06_FLEXPWM2_PWMA00           /* A8 */
/* PWM4 */
#define PWM_OUT4_PINMUX     IOMUXC_GPIO_B0_07_FLEXPWM2_PWMB00           /* A9 */
/* PWM Servo1 */
#define PWM_SERVO1_PINMUX     IOMUXC_GPIO_SD_B1_02_FLEXPWM2_PWMA03
/* PWM Servo2 */
#define PWM_SERVO2_PINMUX     IOMUXC_GPIO_SD_B1_03_FLEXPWM2_PWMB03

/*-------------------------------    改其他地方必出BUG！  ------------------  */



#define DUTY_MAX  10000     //duty最大值


/**
 *  初始化电机、舵机pwm输出
 *  ----------------
 */
void pwm_init(void);

/**
 *  设置电机占空比（速度）
 *  ----------------
 *  duty                占空比             0~10000
 *  ---------------- 
 *  0     前进      5000     后退    10000
 */
static inline void left_motor(short duty)
{
    PWM_UpdateDuty(PWM1, kPWM_Module_3, kPWM_PwmA, duty); 
    PWM_SetPwmLdok(PWM1, 1u<<kPWM_Module_3, true);        //设置pwm的 load ok位
}
static inline void right_motor(short duty)
{
    PWM_UpdateDuty(PWM2, kPWM_Module_0, kPWM_PwmA, duty);  
    PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_0, true);        //设置pwm的 load ok位
}

/**
 *  设置舵机角度
 *  ----------------
 *  duty                占空比             2000-4000
 *  ----------------
 *  舵机中值3000
 */

void servo1(uint16_t highpulse);
void servo2(uint16_t highpulse);
static inline void pwm_servo(uint16_t *highpulse)
{
  PWM_UpdateDuty(PWM2, kPWM_Module_3, kPWM_PwmA, highpulse[0]);
  PWM_UpdateDuty(PWM2, kPWM_Module_3, kPWM_PwmB, highpulse[1]);
  PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_3, true);//设置pwm的 load ok位   如果同时使用subModule 的A B ，可以先设置A的Duty 再设置B的Duty 最后设置pwm的 load ok位  不然pwm容易卡死或者丢失
}

static inline void angle_servo(angle_data_t *angle)
{
  if (angle->Pitch<0)
    angle->Pitch = 0; /* 炮管最低仰角 */
  if (angle->Pitch>55)
    angle->Pitch = 55; /* 炮管最高仰角 */
  if(angle->Yaw>35)
    angle->Yaw = 35;    /* 左限位35° */
  if(angle->Yaw<-35)
    angle->Yaw = -35;   /* 右限位-35° */
  
  PWM_UpdateDuty(PWM2, kPWM_Module_3, kPWM_PwmA, (uint16_t)(1500 - 11.1*angle->Pitch)); /* M3 */
  PWM_UpdateDuty(PWM2, kPWM_Module_3, kPWM_PwmB, (uint16_t)(5.56*angle->Yaw + 1500));   /* M4 */
  PWM_SetPwmLdok(PWM2, 1u<<kPWM_Module_3, true);//设置pwm的 load ok位   如果同时使用subModule 的A B ，可以先设置A的Duty 再设置B的Duty 最后设置pwm的 load ok位  不然pwm容易卡死或者丢失
}

/* 兼容飞卡程序不报错 */
static inline void servo(uint16_t pwm)
{
  uint16_t temp[2] = {pwm,1500};
  pwm_servo(temp);
}


void pwm_test(void);






#endif 



