#include "system.h"

/* 30字节的缓冲区 */
uint8_t hwt_905buff[30] = {0};
attitude_data_t attitude;

void hwt_905_calculate(uint8_t buff[])
{
  if( (uint8_t)(0x55 + 0x53 + buff[2] + buff[3] + buff[4] + buff[5] + 
                buff[6] + buff[7] + buff[8] + buff[9]) == buff[10] )
  {	//姿态解算
    attitude.Roll  = ((float)((signed short int)((((unsigned short int)buff[3])<<8) | buff[2]))) / 32768.0*180.0;
    attitude.Pitch = ((float)((signed short int)((((unsigned short int)buff[5])<<8) | buff[4]))) / 32768.0*180.0;
    attitude.Yaw   = ((float)((signed short int)((((unsigned short int)buff[7])<<8) | buff[6]))) / 32768.0*180.0;
  }
}

void hwt_905_test(void)
{
  char txt[16];
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  motor.init();
  pit_init(kPIT_Chnl_1, 100000); /* 100ms中断 */
  lpuart1_init(9600);         /* 蓝牙发送串口启动 */
  while(1)
  {
	  /* ch0中断置位？5ms  */
	  while (status.interrupt_ch1 == 0)
	  {
	  }
    hwt_905_calculate(hwt_905buff);
    sprintf(txt,"Y: %5.2f",attitude.Pitch); 
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    
    sprintf(txt,"X: %5.2f",attitude.Roll);
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    
    sprintf(txt,"Z: %5.2f",attitude.Yaw);
    LCD_P6x8Str(0,2,(uint8_t*)txt);   
    
    // -90 ~ +90   ->  500 ~ 2500
    servo_highpulse[0] = 11.1*attitude.Roll + 1500;
    servo_highpulse[1] = 5.56*attitude.Yaw + 1500;
    pwm_servo(servo_highpulse);
    
    status.interrupt_ch1 = 0; /* 中断复位 */
  }  
}




