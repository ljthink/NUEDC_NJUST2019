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
  oled.init();
  adc.init();
  char txt[16];
  uint16_t Vc;
  while(1)
  {
    Vc = cap_voltage();
    sprintf(txt,"%3d",Vc);
    oled.ops->word(0,0,(uint8_t*)txt);
    delayms(50);
  
  }

}






