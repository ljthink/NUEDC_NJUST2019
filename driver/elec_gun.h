#ifndef __ELEC_GUN
#define __ELEC_GUN


static inline float cap_voltage(void)
{
  return (float)(adc.ops->get(3)*0.074f);
}

static inline void start_charge(void)
{
  GPIO_PinWrite(GPIO3,16, 1U);        /* H2高电平，继电器吸合 */
}

static inline void stop_charge(void)
{
  GPIO_PinWrite(GPIO3,16, 0U);        /* 充电结束 */
}

/* 等待充电 */
static inline void cap_charge(float voltage)
{
  start_charge();
  while ( cap_voltage() <= voltage);  /* 等待 */
  stop_charge();
}

static inline void elec_fire(void)
{
  delayms(100);
  GPIO_PinWrite(GPIO3,17, 0U);        /* J2低电平，继电器吸合 */
  delayms(50);                       
  GPIO_PinWrite(GPIO3,17, 1U);        /* 发射完成 */ 
}



void elec_cap_voltage_test(void);
void elec_gun_pin_init(void);

#endif



