#include "system.h"

/* 电磁炮控制的全局变量 */
target_data_t target = {
              .distance     = 100,  /* 目标距离cm */
              .yaw          = 0,    /* 目标偏角° */
              .voltage      = 100,   /* 目标对应的发射电压V */
              .pitch        = 3,    /* 目标对应的炮管仰角° */
};


/*-------------------------- 方法声明 ---------------------------*/
static int16_t distance2voltage(void);
static int16_t distance2pitch(void);

static void elec_gun_mode1(void);
static void elec_gun_mode2(void);
static void elec_gun_mode3(void);
static void elec_gun_mode4(void);
static void elec_gun_mode5(void);

/*---------------------- 接口定义 --------------------------------*/
const elecgun_operations_t distance_ops = {
                       .voltage = distance2voltage,
                       .pitch = distance2pitch,
};

const elec_device_t elec_gun = {
                      .init  = elec_gun_pin_init,
                      .mode1 = elec_gun_mode1,
                      .mode2 = elec_gun_mode2,
                      .mode3 = elec_gun_mode3,
                      .mode4 = elec_gun_mode4,
                      .mode5 = elec_gun_mode5,
                      .distance = &distance_ops,
};

/*--------------------- 控制量计算 ------------------------------*/

/* 根据距离求解控制数据，实验拟合 */
static int16_t distance2voltage(void)
{
  
  
}
static int16_t distance2pitch(void)
{

}

/*---------------------------- 四种模式 -------------------------*/

/* 基本模式，按键设置 */
static void elec_gun_mode1(void)
{
  char txt[17];
  char txtnull[5];
  
  /*-------------------------- 模式显示 ----------------------*/
  sprintf(txt,"mode1");
  oled.ops->word(0,0,(uint8_t*)txt);
  
  /*---------------------- 调整内容显示 ----------------------*/
  sprintf(txt, "->dis:    U:    V");
  LCD_P6x8Str(0,1,(uint8_t*)txt); 
  sprintf(txt, "  yaw:    Pich:  ");
  LCD_P6x8Str(0,2,(uint8_t*)txt); 
  sprintf(txtnull, "  ");
  
  /*-------------------------- 按键读取调整 ------------------*/
  uint8_t key_flag = 1,choose_flag = 1;
  //uint8_t c;
  int16_t *temp;
  temp = &target.distance; /**/
  while(key_flag)
  {
    switch (key.ops->get(0))
    {
    case key_m10:  *temp = *temp - 10; break;
    case key_p10:  *temp = *temp + 10; break;
    case key_m1 :  *temp = *temp - 1;  break;
    case key_p1 :  *temp = *temp + 1;  break;
    case key_sw : choose_flag = (choose_flag ==0 );  break;
    case key_ok : key_flag = 0   ;                   break;
    default : break;
    }
    /* 选项显示状态更新 */
    sprintf(txt, "->");
    if (choose_flag)
    {
      LCD_P6x8Str(0,1,(uint8_t*)txt);
      LCD_P6x8Str(0,2,(uint8_t*)txtnull);
      temp = &target.distance;
    }
    else
    {
      LCD_P6x8Str(0,1,(uint8_t*)txtnull);
      LCD_P6x8Str(0,2,(uint8_t*)txt); 
      temp = &target.yaw;
    }
    
    /* 电压解算 */
    target.voltage = (int16_t)(-0.0004f*target.distance*target.distance + 0.297f*target.distance + 59.757);
    sprintf(txt,"%3d",target.voltage);    /* 电压显示 */
    LCD_P6x8Str(72,1,(uint8_t*)txt);
    sprintf(txt,"%3d",target.distance);   /* 距离显示 */
    LCD_P6x8Str(36,1,(uint8_t*)txt);
    sprintf(txt,"%3d",target.yaw);        /* 偏角显示 */
    LCD_P6x8Str(36,2,(uint8_t*)txt);
    sprintf(txt,"%3d",target.pitch);      /* 仰角显示 */
    LCD_P6x8Str(72,2,(uint8_t*)txt);    
  }
  
  /*----------------------------- 发射流程 --------------------------------*/
  target.pitch = 45;
  angle_servo(&target); /* 瞄准 */
  
  /* 舵机响应时间 */
  delayms(300);
  
  sprintf(txt,"charging..."); 
  LCD_P6x8Str(0,4,(uint8_t*)txt); 
  cap_charge(target.voltage);          /* 发射电压控制 */
  sprintf(txt,"fire"); 
  LCD_P6x8Str(0,5,(uint8_t*)txt);
  elec_fire();
  oled.ops->clear();
  /*------------------------- 发射完成 -------------------------*/
}

static void elec_gun_mode2(void)
{
  char txt[16];
  sprintf(txt,"mode2");
  oled.ops->word(0,0,(uint8_t*)txt);
  delayms(1000);  
}

static void elec_gun_mode3(void)
{
  char txt[16];
  sprintf(txt,"mode3");
  oled.ops->word(0,0,(uint8_t*)txt);
  delayms(1000);  
}

static void elec_gun_mode4(void)
{
  char txt[16];
  sprintf(txt,"mode4");
  oled.ops->word(0,0,(uint8_t*)txt);
  delayms(1000);  
}

static void elec_gun_mode5(void)
{
  char txt[17];
  char txtnull[5];
  sprintf(txt,"mode5");
  oled.ops->word(0,0,(uint8_t*)txt);
  /*---------------------- 调整内容显示 ----------------------*/
  sprintf(txt, "->dis:    U:    V");
  LCD_P6x8Str(0,1,(uint8_t*)txt); 
  sprintf(txt, "  yaw:    Pich:  ");
  LCD_P6x8Str(0,2,(uint8_t*)txt); 
  sprintf(txtnull, "  ");
  
  /*-------------------------- 按键读取调整 ------------------*/
  uint8_t key_flag = 1,choose_flag = 1;
  int16_t *temp;
  temp = &target.voltage; /**/
  while(key_flag)
  {
    switch (key.ops->get(0))
    {
    case key_m10:  *temp = *temp - 10; break;
    case key_p10:  *temp = *temp + 10; break;
    case key_m1 :  *temp = *temp - 1;  break;
    case key_p1 :  *temp = *temp + 1;  break;
    case key_sw : choose_flag = (choose_flag ==0 );  break;
    case key_ok : key_flag = 0   ;                   break;
    default : break;
    }
    /* 选项显示状态更新 */
    sprintf(txt, "->");
    if (choose_flag)
    {
      LCD_P6x8Str(0,1,(uint8_t*)txt);
      LCD_P6x8Str(0,2,(uint8_t*)txtnull);
      temp = &target.voltage;
    }
    else
    {
      LCD_P6x8Str(0,1,(uint8_t*)txtnull);
      LCD_P6x8Str(0,2,(uint8_t*)txt); 
      temp = &target.yaw;
    }
    
    /* 电压解算 */
    sprintf(txt,"%3d",target.voltage);    /* 电压显示 */
    LCD_P6x8Str(72,1,(uint8_t*)txt);
    sprintf(txt,"%3d",target.distance);   /* 距离显示 */
    LCD_P6x8Str(36,1,(uint8_t*)txt);
    sprintf(txt,"%3d",target.yaw);        /* 偏角显示 */
    LCD_P6x8Str(36,2,(uint8_t*)txt);
    sprintf(txt,"%3d",target.pitch);      /* 仰角显示 */
    LCD_P6x8Str(72,2,(uint8_t*)txt);    
  }
  
  /*----------------------------- 发射流程 --------------------------------*/
  target.pitch = 45;
  angle_servo(&target); /* 瞄准 */
  
  /* 舵机响应时间 */
  delayms(300);
  
  sprintf(txt,"charging..."); 
  LCD_P6x8Str(0,4,(uint8_t*)txt); 
  cap_charge(target.voltage);          /* 发射电压控制 */
  sprintf(txt,"fire"); 
  LCD_P6x8Str(0,5,(uint8_t*)txt);
  elec_fire();
  oled.ops->clear();
  /*------------------------- 发射完成 -------------------------*/  


  
}

void elec_charge_test(void)
{
  key.init();                   /* 按键启动 */
  led.init();                   /* 指示灯启动 */
  NVIC_SetPriorityGrouping(2);  /* 2: 4个抢占优先级 4个子优先级*/
  oled.init();                   /* LCD启动 */
  adc.init();
  elec_gun.init();
  
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
      case key_m10:  target.voltage = target.voltage - 10; break;
      case key_p10:  target.voltage = target.voltage + 10; break;
      case key_m1 :  target.voltage = target.voltage - 1;  break;
      case key_p1 :  target.voltage = target.voltage + 1;  break;
      case key_ok : flag = 0   ; break;
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

