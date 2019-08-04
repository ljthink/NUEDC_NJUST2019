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

/*--------------------- 电机转速闭环使用到的的全局参数 -----------------------*/
short speedvalue;

/* 单个电机的速度PID参数 */
_motor_pid_t motor_leftpid;
_motor_pid_t motor_rightpid;

/* 电机PID参数 */
motor_pid_t pid = {
  .left = &motor_leftpid,
  .right = &motor_rightpid,    
};

/* 目标电机速度，初始为0 */
motor_speed_t motor_speed = {
    .left = 35,
    .right = 35,
};

/* ---------------------------- 方法声明 ------------------------------------ */
static void motor_pid_device_init(void);
static void motor_pid_clear(motor_pid_t* base);
static void motor_pid_change(motor_pid_t* base,float p,float i,float d);
static void motor_pid_control(motor_speed_t *speed);
static void motor_pid_test(void);


/* ---------------------------- 外部接口 ------------------------------------ */

const mpid_operations_t motor = {
    .init       = motor_pid_device_init,
    .pidclear   = motor_pid_clear,
    .pidchange  = motor_pid_change,
    .pidcontrol = motor_pid_control,
    .pidtest    = motor_pid_test,
};


/* ---------------------------- 方法实现 ------------------------------------ */

static void motor_pid_device_init(void)
{
  enc_init();
  pwm_init();
  motor.pidchange(&pid,1,0,0);
}

static void motor_pid_clear(motor_pid_t* base)
{
  base->left->err = 0;
  base->left->err1 = 0;
  base->left->err2 = 0;
  base->left->int_err = 0;
  base->left->ut = 0;
  
  base->right->err = 0;
  base->right->err1 = 0;
  base->right->err2 = 0;
  base->right->int_err = 0;
  base->right->ut = 0;
}

static void motor_pid_change(motor_pid_t* base,float p,float i,float d)
{
  base->kp = p;
  base->ki = i;
  base->kd = d;
}

/* 电机PI控制 */
static void motor_pid_control(motor_speed_t *speed)
{
  float pwm1,pwm2;
  /* =后面为编码器读到的数字 */
  speed->enc_left  = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //得到编码器微分值
  speed->enc_right = (int16_t)ENC_GetPositionDifferenceValue(ENC2);  //得到编码器微分值

  /* 获得偏差 */
  pid.left->err  = speed->left  - speed->enc_left;
  pid.right->err = speed->right - speed->enc_right;

  /* 增量式PI控制，输出为编码器的控制值 */
  pid.left->ut  += pid.kp*(pid.left->err  - pid.left->err1)  + pid.ki*pid.left->err;           
  pid.right->ut += pid.kp*(pid.right->err - pid.right->err1) + pid.ki*pid.right->err;

  /* 编码器对应到PWM */
  if (pid.left->ut > 0 )
    pwm1 = 6.7749*pid.left->ut + 5310;
  else
    pwm1 = 6.9122*pid.left->ut + 4898;
  if (pid.right->ut > 0)
    pwm2 = 6.8252*pid.right->ut + 5290;
  else
    pwm2 = 7.2176*pid.right->ut + 4890;
  /* 偏差保存 */
  pid.left->err1  = pid.left->err;
  pid.right->err1 = pid.right->err;

  /* 输出限幅 */
  if(pwm1 > 9000)
    pwm1 = 9000.0f;
  else if (pwm1 < -9000)
    pwm1 = -9000.0f;

  if(pwm2 > 9000)
    pwm2 = 9000.0f;
  else if (pwm2 < -9000)
    pwm2 = -9000.0f;  

  /* pwm输出 */
  left_motor((uint16_t)(pwm1));
  right_motor((uint16_t)(pwm2));
  
}



static void motor_pid_test(void)
{
  lpuart1_init(115200);         /* 蓝牙发送串口启动 */
  key.init();
  motor.init();
  oled.init();
  NVIC_SetPriorityGrouping(2);
  
  UI_debugsetting();

  oled.ops->clear();
 
  pit_init(kPIT_Chnl_0, 10000);
  
  while (1)
  {        
	  while (status.interrupt_ch0 == 0)
	  {
		  /* 遥控中断给出调试标志位 */
//		  if(status.debug_mode == 1)
//			  UI_debugsetting();
	  }
    
    motor_pid_control(&motor_speed);
    
    printf("%d\n",motor_speed.enc_left);
   
//    sprintf(txt,"ENC1:  %5d ",motor_speed.enc_left); 
//    LCD_P6x8Str(0,0,(uint8_t*)txt);
//
//    sprintf(txt,"ENC2:  %5d ",motor_speed.enc_right); 
//    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    /* 中断复位 */
    status.interrupt_ch0 = 0;
  }
}