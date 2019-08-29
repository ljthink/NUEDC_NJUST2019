#include "system.h"

/* 电磁炮控制的全局变量 */
target_data_t target = {
              .distance       = 200,  /* 目标距离cm */
              .distance_ks103 = 200,  /* 超声波测距cm */
              .yaw            = 0,    /* 目标偏角° */
              .voltage        = 80,  /* 目标对应的发射电压V */
              .pitch          = 3,    /* 目标对应的炮管仰角° */
};

/*-------------------------- 方法声明 ---------------------------*/
static inline float distance2voltage(void);
static int16_t distance2pitch(void);

static void elec_gun_mode1(void);
static void elec_gun_mode2(void);
static void elec_gun_mode3(void);
static void elec_gun_mode4(void);
static void elec_gun_mode5(void);
static void elec_gun_mode6(void);

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
                      .mode6 = elec_gun_mode6,
                      .distance = &distance_ops,
};

/*--------------------- 控制量计算 ------------------------------*/

/* 根据距离求解控制数据，实验拟合 */
static inline float distance2voltage(void)
{
  ks103_get_distance();
  return (0.1243f*target.distance_ks103 + 47.055f);
}
static inline int16_t distance2pitch(void)
{
  return 0;
}

/*---------------------------- 四种模式 -------------------------*/

/*---------------------------- 1.基本模式，按键设置 ---------------*/
/*
   按键设置距离和偏角
*/
static void elec_gun_mode1(void)
{
  char txt[21];
  char txtnull[5];
  
  /*----------- 模式显示 --------------*/
  sprintf(txt,"MODE - 1");
  oled.ops->word(33,0,txt);
  
  /*------------ 调整内容显示 ---------*/
  sprintf(txt, "->dis:     U:     V");
  LCD_P6x8Str(0,1,txt); 
  sprintf(txt, "  yaw:     Pich:    ");
  LCD_P6x8Str(0,2,txt); 
  sprintf(txtnull, "  ");
  
  /*------------ 按键读取调整 ---------*/
  uint8_t key_flag = 1,choose_flag = 1;
  float *temp;
  temp = &target.distance; /**/
  while(key_flag)
  {
    switch (key.ops->get(0))
    {
    case key_m10:  *temp = *temp - 10; break;
    case key_p10:  *temp = *temp + 10; break;
    case key_m1 :  *temp = *temp - 1;  break;
    case key_p1 :  *temp = *temp + 1;  break;
    case key_mf :  *temp = *temp - 0.1;  break;
    case key_pf :  *temp = *temp + 0.1;  break;
    case key_sw : choose_flag = (choose_flag ==0 );  break;
    case key_ok : key_flag = 0   ;                   break;
    default : break;
    }
    /* 选项显示状态更新 */
    sprintf(txt, "->");
    if (choose_flag)
    {
      oled.ops->word(0,1,txt);
      oled.ops->word(0,2,txtnull);
      temp = &target.distance;
    }
    else
    {
      oled.ops->word(0,1,txtnull);
      oled.ops->word(0,2,txt); 
      temp = &target.yaw;
    }
    
    /* 电压解算 */
    target.voltage = distance2voltage();
    sprintf(txt,"%4.1f",target.voltage);    /* 电压显示 */
    oled.ops->word(78,1,txt);
    sprintf(txt,"%3.0f",target.distance);   /* 距离显示 */
    oled.ops->word(36,1,txt);
    sprintf(txt,"%3.1f",target.yaw);        /* 偏角显示 */
    oled.ops->word(36,2,txt);
    sprintf(txt,"%3.1f",target.pitch);      /* 仰角显示 */
    oled.ops->word(96,2,txt);    
  }
  
  /*----------------------------- 发射流程 --------------------------------*/
  target.pitch = 45;
  angle_servo(&target); /* 瞄准 */
  
  /* 舵机响应时间 */
  delayms(200);
  
  sprintf(txt,"charging..."); 
  oled.ops->word(0,4,txt); 
  cap_charge(target.voltage);          /* 发射电压控制 */
  sprintf(txt,"fire"); 
  oled.ops->word(0,5,txt);
  elec_fire();
  oled.ops->clear();
  /*------------------------- 发射完成 -------------------------*/
}



/*---------------------------- 2.静止射击模式 --------------------*/
static void elec_gun_mode2(void)
{
  char txt[16];
  sprintf(txt,"MODE - 2");
  oled.ops->word(33,0,txt);
  
  /* 炮管初始姿态设定 */
  target.pitch = 3;
  target.yaw = -30;
  angle_servo(&target);
  
  sprintf(txt,"press ok to start");
  oled.ops->word(0,1,txt);

  /* 按键启动 */
  while( key.ops->get(0) != key_ok);  
 
  /* 转动查找模式 */
  /* 开始查找 */
  float delta_yaw = 0.25f;
  while(1)
  {
    /* 刷新一次数据 */
    openmv_data_refresh();
    /* 320像素宽，中点160 */
    if ( target_pix_x>158 && target_pix_x<162)              /* 159,160,161 */
      break; /* 推出循环准备发射 */
    else
    {/* 更新位置重新寻找目标 */
      target.yaw = target.yaw + delta_yaw;
      angle_servo(&target); /* 炮管偏角更新 */
      
      if (target.yaw == 30.0f)
        delta_yaw = -0.25f;
      if (target.yaw == -30.0f)
        delta_yaw = 0.25f;
    }
    delayms(30);
  }

  
  target.pitch = 45;  /* 仰角更新 */
  target.voltage = distance2voltage(); /* 发射电压计算 */
  angle_servo(&target);           /* 瞄准 */
  cap_charge(target.voltage);     /* 充电 */
  elec_fire();                    /* 发射 */
  oled.ops->clear();
}



/*-------------------------- 3.转动射击模式 ------------------------*/
static void elec_gun_mode3(void)
{
  char txt[20];
  sprintf(txt,"MODE - 3");
  oled.ops->word(33,0,txt);
  
  /* 炮管初始姿态设定4° */
  target.pitch = 4;
  target.yaw = -30;
  angle_servo(&target);
  
  sprintf(txt,"press ok to start");
  oled.ops->word(0,1,txt);

  /* 按键启动 */
  while( key.ops->get(0) != key_ok);
  
  uint8_t fire_already = 1;
  float delta_yaw = 0.5f;
  float fire_angle = 0;  
  float charged_ok = 0;  
  
  /* 开始查找 */
  start_charge(); /* 电容开始充电 */
  while(1)
  {
    /* 刷新一次数据 */
    openmv_data_refresh();
    

    /* 有发射角记录，且充电完成，发射 */
    if (fire_already == 3 && target.yaw == fire_angle && charged_ok==1)
    {
      GPIO_PinWrite(GPIO3,17, 0U);      /* J2低电平，继电器吸合 */
      delayms(50);                       
      GPIO_PinWrite(GPIO3,17, 1U);      /* 发射完成 */
      fire_already = 0;                 /* 发射完成 不再进入此函数 */
    }
    
    /* 148-153 精度海星*/
    if (target_pix_x>151 && target_pix_x<156 && fire_already==1)    /* 目标在范围内发射，从-30往左找，选中间偏右位置，提前量。 */
    {
      fire_angle = target.yaw + 0.5; /* 储存发射角 */
      target.pitch = 45;       /* 炮管升起 */
      fire_already = 2;        /* 下次不进入此函数 */
    }
    
    /* 跳过第一次相等 */
    if(fire_already == 2 && target.yaw == fire_angle)
    {
      fire_already = 3;
    }    
    
    target.yaw = target.yaw + delta_yaw;
    angle_servo(&target); /* 炮管偏角更新 */
    
    if (target.yaw == 30.0f)
      delta_yaw = -0.5f;
    if (target.yaw == -30.0f)
      delta_yaw = 0.5f;    
    
    /* 50ms延时中检测充电电压 */
    GPT_StartTimer(GPT2);
    while(GPT_GetCurrentTimerCount(GPT2) < 500000 - 7)
    {
      if (cap_voltage() >= 79.5 )
      {
        stop_charge();
        charged_ok = 1;
      }
    }
    GPT_StopTimer(GPT2);
  }
}

/*-------------------------- 4.跟随射击模式 ------------------------*/
static void elec_gun_mode4(void)
{
  char txt[20];
  sprintf(txt,"MODE - 4");
  oled.ops->word(33,0,txt); 
 
  int err,sum = 0;
  err = 160 - target_pix_x;
  
      /* 刷新一次数据 */
//  while(1)
//  {
//    openmv_data_refresh();
//    /* 像素PI控制 */
//    err = 160 - target_pix_x;
//    sum += err;
//    target.yaw = 0.1*err + 0.05*sum;
//    angle_servo(&target); /* 炮管偏角更新 */
//    
//    if(err<3 && err>-3)/* 调整完成 */
//      break;
//    delayms(50);        
//  }
//  
//  sprintf(txt,"locked");
//  oled.ops->word(0,1,txt);
  
  /* 炮管初始姿态设定 */
  target.pitch = 3;
  target.yaw = -30;
  angle_servo(&target);
  
  sprintf(txt,"press ok to start");
  oled.ops->word(0,1,txt);

  /* 按键启动 */
  while( key.ops->get(0) != key_ok);  
  
  /* 开始查找 */
  float delta_yaw = 0.5f;
  while(1)
  {
    /* 刷新一次数据 */
    openmv_data_refresh();
    /* 320像素宽，中点160 */
    if ( target_pix_x>158 && target_pix_x<162)              /* 159,160,161 */
      break; /* 推出循环准备发射 */
    else
    {/* 更新位置重新寻找目标 */
      target.yaw = target.yaw + delta_yaw;
      angle_servo(&target); /* 炮管偏角更新 */
      
      if (target.yaw == 30.0f)
        delta_yaw = -0.5f;
      if (target.yaw == -30.0f)
        delta_yaw = 0.5f;
    }
    delayms(50);
  }
  
  target.pitch = 45;  /* 仰角更新 */
  target.voltage = distance2voltage(); /* 发射电压计算 */
  angle_servo(&target);           /* 瞄准 */
  cap_charge(target.voltage);     /* 充电 */
  elec_fire();                    /* 发射 */
  oled.ops->clear();
  
}

/*-------------------------- 5.直接电压控制 ------------------------*/
static void elec_gun_mode5(void)
{
  char txt[21];
  char txtnull[5];
  
  /*-------------------------- 模式显示 ----------------------*/
  sprintf(txt,"MODE - 5");
  oled.ops->word(33,0,txt);
  
  /*---------------------- 调整内容显示 ----------------------*/
  sprintf(txt, "->U:     V");
  LCD_P6x8Str(0,1,txt); 
  sprintf(txt, "  Pitch:     ");
  LCD_P6x8Str(0,2,txt);
  sprintf(txtnull, "  ");
  
  target.pitch = 45;        /* 炮管升起 */
  angle_servo(&target);     /* 炮管偏角更新 */
  
  /*-------------------------- 按键读取调整 ------------------*/
  uint8_t key_flag = 1,choose_flag = 1;
  float *temp;
  temp = &target.voltage; /**/
  while(1)
  {
    key_flag = 1;
    while(key_flag)
    {
      switch (key.ops->get(0))
      {
      case key_m10:  *temp = *temp - 10; break;
      case key_p10:  *temp = *temp + 10; break;
      case key_m1 :  *temp = *temp - 1;  break;
      case key_p1 :  *temp = *temp + 1;  break;
      case key_mf :  *temp = *temp - 0.1;  break;
      case key_pf :  *temp = *temp + 0.1;  break;
      case key_sw : choose_flag = (choose_flag ==0 );  break;
      case key_ok : key_flag = 0   ;                   break;
      default : break;
      }
      /* 选项显示状态更新 */
      sprintf(txt, "->");
      if (choose_flag)
      {
        oled.ops->word(0,1,txt);
        oled.ops->word(0,2,txtnull);
        temp = &target.voltage;
      }
      else
      {
        oled.ops->word(0,1,txtnull);
        oled.ops->word(0,2,txt); 
        temp = &target.pitch;
      }
      
      /* 电压解算 */
      sprintf(txt,"%4.1f",target.voltage);
      LCD_P6x8Str(24,1,txt);
      sprintf(txt,"%4.1f",target.pitch);
      LCD_P6x8Str(48,2,txt);
    }
    /*--------------------------  开始查找  --------------------------*/
    
    start_charge();
    while ( cap_voltage() <= target.voltage)
    {
      sprintf(txt,"%4.1f",cap_voltage());
      oled.ops->word(24,1,txt);  
    }  /* 等待 */
    stop_charge();
    
    elec_fire();
    oled.ops->clear();
    /*------------------------- 发射完成 -------------------------*/ 
  }
}


/*-------------------------- 6.超声波测距测试 ------------------------*/
static void elec_gun_mode6(void)
{
  char txt[16];
  oled.ops->clear();
	while(1)
	{
    ks103_get_distance();
    sprintf(txt,"%4d mm",target.distance_ks103);
    oled.ops->word(0,0,txt); 
  }
}


void elec_charge_test(void)
{
  elec_gun.init();      /* 电磁炮初始化 */
  key.init();
  pwm_init();
  oled.init();
  adc.init();
  ks103_init();
  lpuart1_init(115200);
  
  char txt[16];
  
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
      sprintf(txt,"Set: %4.1f V",target.voltage); 
      LCD_P6x8Str(0,0,txt);
      sprintf(txt,"Uc:  %4.1f V",cap_voltage()); 
      LCD_P6x8Str(0,1,txt);
      delayms(10);
    }
    /*------------------------------------------------------------*/
    
    /*------------------------- 发射流程 -------------------------*/
    oled.ops->clear();
    sprintf(txt,"charging..."); 
    LCD_P6x8Str(0,0,txt); 
    cap_charge(target.voltage);          /* 发射电压控制 */
    sprintf(txt,"fire"); 
    LCD_P6x8Str(0,1,txt);
    elec_fire();
    oled.ops->clear();
    /*------------------------- 发射完成 -------------------------*/
    
    flag = 1;
  }

}

