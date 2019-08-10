#include "system.h"

/* 电磁炮控制的全局变量 */
target_data_t target = {
              .distance     = 100,  /* 目标距离cm */
              .yaw          = 0,    /* 目标偏角° */
              .voltage      = 80,   /* 目标对应的发射电压V */
              .pitch        = 0,    /* 目标对应的炮管仰角° */
};

/* 根据距离求解控制数据，实验拟合 */
int16_t distance2voltage(void)
{
  
}
int16_t distance2pitch(void)
{

}




void elec_charge_test(void)
{
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  adc.init();
  elec_gun_pin_init();
  
  char txt[16];
  
  sprintf(txt,"Set: %3d V",target.voltage);
  LCD_P6x8Str(0,0,(uint8_t*)txt);
  sprintf(txt,"Uc:  %3d V",cap_voltage()); 
  LCD_P6x8Str(0,1,(uint8_t*)txt); 

  uint8_t flag = 1;
  
  while(1)
  {
    
    /*-------------- 按键控制变量，需要1外部全局标志变量 ----------*/
    while(flag)
    {
      switch (key.ops->get(0))
      {
      case key1:  target.voltage = target.voltage - 10;break;
      case key2:  target.voltage = target.voltage + 10;break;
      case key3:  target.voltage = target.voltage - 1;break;
      case key4:  target.voltage = target.voltage + 1;break;
      case key_ok    : flag = 0   ; break;
      default : break;
      }
      sprintf(txt,"Set: %3d V",target.voltage); 
      LCD_P6x8Str(0,0,(uint8_t*)txt);
      sprintf(txt,"Uc:  %3d V",cap_voltage()); 
      LCD_P6x8Str(0,1,(uint8_t*)txt);
      delayms(10);
    }
    /*------------------------------------------------------------*/
    
    /*------------------------- 发射流程 -------------------------*/
    oled.ops->clear();
    sprintf(txt,"charging..."); 
    LCD_P6x8Str(0,0,(uint8_t*)txt); 
    cap_charge(target.voltage);          /* 发射电压控制 */
    sprintf(txt,"fire"); 
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    elec_fire();
    oled.ops->clear();
    /*------------------------- 发射完成 -------------------------*/
    
    flag = 1;
  }

}

