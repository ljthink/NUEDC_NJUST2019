#ifndef __ELEC_GUN
#define __ELEC_GUN



static inline uint16_t cap_voltage(void)
{
  return (uint16_t)(adc.ops->get(3)*0.074f);
}



void elec_cap_voltage_test(void);



#endif



