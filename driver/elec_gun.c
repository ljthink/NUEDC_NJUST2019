#include "system.h"

void elec_gun_pin_init(void)
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


void elec_cap_voltage_test(void)
{
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  adc.init();
  char txt[16];
  float Vc;
  
  while(1)
  {
    Vc = cap_voltage();
    sprintf(txt,"Uc: %3.1f V",Vc); 
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    delayms(50);
    
  }
}

