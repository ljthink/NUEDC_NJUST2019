#include "system.h"


static void elec_output_pin_init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO口时钟使能

  /* 充电、发射控制引脚 */
  /* H2 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_GPIO3_IO16, 0U);  /* 充电，1有效 */
  /* J2 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_GPIO3_IO17, 0U);  /* 发射，0有效 */

  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_GPIO3_IO16, 0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_GPIO3_IO17, 0x10B0u);


  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                        0,                     
                                        kGPIO_NoIntmode        //非中断模式
                                        };

  GPIO_PinInit(GPIO3,16, &GPIO_Output_Config);
  
  GPIO_Output_Config.outputLogic = 1;
  GPIO_PinInit(GPIO3,17, &GPIO_Output_Config);

}


static void elec_input_pin_init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          // IO口时钟使能
//  /* L13 */
//  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26,0U);
//  /* H12 */
//  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28,0U);
//  /* G12 */
//  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30,0U);
//  
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26,0xF080);
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28,0xF080);
//  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30,0xF080);
//  
//  gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
//                               1,                    //高电平
//                               kGPIO_NoIntmode,      //不触发中断
//                               };
//  
//  GPIO_PinInit(GPIO1,26,&GPIO_Input_Config);        // GPIO输入口，非中断
//  GPIO_PinInit(GPIO1,28,&GPIO_Input_Config);        // GPIO输入口，非中断
//  GPIO_PinInit(GPIO1,30,&GPIO_Input_Config);        // GPIO输入口，非中断
}


void elec_gun_pin_init(void)
{
  elec_output_pin_init();
  elec_input_pin_init();
}



void elec_cap_voltage_test(void)
{
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  adc.init();
  char txt[16];
  uint16_t Vc;
  
  while(1)
  {
    Vc = cap_voltage();
    sprintf(txt,"Uc: %3d V",Vc); 
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    delayms(50);
    
  }
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
  uint16_t SetVc = 30;
  uint8_t flag = 1;
  
  sprintf(txt,"Set: %3d V",SetVc); 
  LCD_P6x8Str(0,0,(uint8_t*)txt);
  sprintf(txt,"Uc:  %3d V",cap_voltage()); 
  LCD_P6x8Str(0,1,(uint8_t*)txt);   
  
  while(1)
  {
    while(flag)
    {
      switch (key.ops->get(0))
      {
      case key_minus : SetVc = SetVc - 1; break;
      case key_plus  : SetVc = SetVc + 1; break;
      case key_ok    : flag = 0   ; break;
      default : break;
      }
      sprintf(txt,"Set: %3d V",SetVc); 
      LCD_P6x8Str(0,0,(uint8_t*)txt);
      sprintf(txt,"Uc:  %3d V",cap_voltage()); 
      LCD_P6x8Str(0,1,(uint8_t*)txt);
      delayms(10);
    }
    
    /* 发射提示 */
    oled.ops->clear();
    
    sprintf(txt,"charging..."); 
    LCD_P6x8Str(0,0,(uint8_t*)txt); 
    
    cap_charge(SetVc);
       
    sprintf(txt,"fire"); 
    LCD_P6x8Str(0,1,(uint8_t*)txt);
    elec_fire();
    oled.ops->clear();
    flag = 1;
  }

}




