#include "system.h"

/* ---------------------------- 方法声明 ------------------------------------ */
static uint8_t key_read(uint8_t mode);
static void key_init(void);


/* ---------------------------- 外部接口 ------------------------------------ */
const key_operations_t key_ops = {
    .get = key_read,
};


const key_device_t key = {
    .init = key_init,
    .ops = &key_ops,
};



/* ---------------------------- 方法实现 ------------------------------------ */
static void key_init(void)
{  
  CLOCK_EnableClock(kCLOCK_Iomuxc);          // IO口时钟使能
  /* 母板上的按键 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_GPIO2_IO27,   0U);   //-  
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_GPIO2_IO30,   0U);   //+ 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0U);   //OK
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_GPIO2_IO27,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_GPIO2_IO30,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_04_GPIO3_IO04,0xF080);
  
  gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
                               1,                    //高电平
                               kGPIO_NoIntmode,      //不触发中断
                               };
  
  GPIO_PinInit(GPIO2,27,&GPIO_Input_Config);        // GPIO输入口，非中断
  GPIO_PinInit(GPIO2,30,&GPIO_Input_Config);        // GPIO输入口，非中断
  GPIO_PinInit(GPIO3,04,&GPIO_Input_Config);        // GPIO输入口，非中断
  
  /* 电容按键 */
  //   8 | M13 J13 H11 J14 | 2
  //   7 | H13 L13 H12 G12 | 1
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30,0U);
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31,0U);
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_GPIO1_IO25,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26,0xF080);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29,0xF080);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30,0xF080);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31,0xF080);
  
  
  GPIO_Input_Config.outputLogic = 0;
  
  GPIO_PinInit(GPIO1,24,&GPIO_Input_Config);  
  GPIO_PinInit(GPIO1,25,&GPIO_Input_Config);
  GPIO_PinInit(GPIO1,26,&GPIO_Input_Config);
  GPIO_PinInit(GPIO1,27,&GPIO_Input_Config);  
  GPIO_PinInit(GPIO1,28,&GPIO_Input_Config);
  GPIO_PinInit(GPIO1,29,&GPIO_Input_Config);
  GPIO_PinInit(GPIO1,30,&GPIO_Input_Config);  
  GPIO_PinInit(GPIO1,31,&GPIO_Input_Config);
}
  

/**
 *  读按键
 *  ----------------
 *  mode                模式设置             1连续按    0单次按
 *  ----------------
 *  
 */
static uint8_t key_read(uint8_t mode)
{
  static uint8_t key_up = 1;
  
  if(mode == 1) 
    key_up = 1;
  if(key_up && ANY_KEY_PRESS)
  {
    delayms(10);   //消抖
    key_up = 0;
    if(GPIO_PinRead(GPIO1,24)==1)
      return key1;
    else if (GPIO_PinRead(GPIO1,25)==1)
      return key2;
    else if (GPIO_PinRead(GPIO1,26)==1)
      return key3;
    else if (GPIO_PinRead(GPIO1,27)==1)
      return key4;
    else if (GPIO_PinRead(GPIO1,28)==1)
      return key5;
    else if (GPIO_PinRead(GPIO1,29)==1)
      return key6;
    else if (GPIO_PinRead(GPIO1,30)==1)
      return key7;
    else if (GPIO_PinRead(GPIO1,31)==1)
      return key8;
    else if(GPIO_PinRead(GPIO2,27)==0)      
      return key_minus;
    else if(GPIO_PinRead(GPIO2,30)==0) 
      return key_plus;
    else if(GPIO_PinRead(GPIO3,04)==0) 
      return key_ok;
  }
  if(NO_KEY_PRESS) 
    key_up = 1;
  return no_key;   //无按键按下
} 



void key_test(void)
{
  key.init();
  oled.init();
  char txt[16];
  
  while(1)
  {
    sprintf(txt,"%2d",key.ops->get(0));
    LCD_P6x8Str(0,0,(uint8_t*)txt);
    delayms(100);
  }
}


