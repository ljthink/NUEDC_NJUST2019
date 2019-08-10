#ifndef __ELEC_GUN
#define __ELEC_GUN



static inline uint16_t cap_voltage(void)
{
  return (uint16_t)(adc.ops->get(3)*0.074f);
}

static inline void cap_charge(uint16_t voltage)
{
  GPIO_PinWrite(GPIO3,16, 1U);        /* H2高电平，继电器吸合 */
  while ( cap_voltage() <= voltage);  /* 等待 */
  GPIO_PinWrite(GPIO3,16, 0U);        /* 充电结束 */
}

static inline void elec_fire(void)
{
  delayms(300);
  GPIO_PinWrite(GPIO3,17, 0U);        /* J2低电平，继电器吸合 */
  delayms(300);                       
  GPIO_PinWrite(GPIO3,17, 1U);        /* 发射完成 */ 
}



void elec_cap_voltage_test(void);
void elec_gun_pin_init(void);

#endif



