#include "system.h"


static void elec_output_pin_init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);           // IO口时钟使能

  /* 三级发射指示灯 */
  /* J12 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0U); 
  /* K10 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0U);
  /* J11 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0U);

  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_06_GPIO1_IO22, 0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_07_GPIO1_IO23, 0x10B0u);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_GPIO1_IO16, 0x10B0u);


  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                        1,                     //高电平
                                        kGPIO_NoIntmode        //非中断模式
                                        };

  GPIO_PinInit(GPIO1,22, &GPIO_Output_Config);
  GPIO_PinInit(GPIO1,23, &GPIO_Output_Config);
  GPIO_PinInit(GPIO1,16, &GPIO_Output_Config);
}


static void elec_input_pin_init(void)
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);          // IO口时钟使能
  /* L12 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20,0U);
  /* K11 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0U);
  /* M13 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25,0U);
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_04_GPIO1_IO20,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_GPIO1_IO17,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25,0xF080);
  
  gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
                               1,                    //高电平
                               kGPIO_NoIntmode,      //不触发中断
                               };
  
  GPIO_PinInit(GPIO1,20,&GPIO_Input_Config);        // GPIO输入口，非中断
  GPIO_PinInit(GPIO1,17,&GPIO_Input_Config);        // GPIO输入口，非中断
  GPIO_PinInit(GPIO1,25,&GPIO_Input_Config);        // GPIO输入口，非中断
  
}



void elec_signalled_test(void)
{
  elec_output_pin_init();
  //elec_input_pin_init();
  key.init();
  oled.init();
  char txt[16];
  
  while(1)
  {
//    if(key.ops->get(0))
//    {
//      LEVEL1(OFF);
//      LEVEL2(OFF);
//      LEVEL3(OFF);
//      delayms(300);
//    }
    
//    if (GPIO_PinRead(GPIO1,25)==0)
//      LEVEL1(ON);
//    if (GPIO_PinRead(GPIO1,17)==0)
//      LEVEL2(ON);    
//    if (GPIO_PinRead(GPIO1,20)==0)
//      LEVEL3(ON);  
    delayms(200);
    LEVEL1(ON);
    delayms(200);
    LEVEL2(ON);    
    delayms(200);
    LEVEL3(ON);  
    delayms(200);
    LEVEL1(OFF);
    delayms(200);
    LEVEL2(OFF);    
    delayms(200);
    LEVEL3(OFF);     
    
  }
  
  

}

