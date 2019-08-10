#ifndef __ELECCONTROL_H
#define __ELECCONTROL_H


typedef struct _target_data
{
    int16_t  distance;    /* cm */
    int16_t  yaw;         /* °  */
    int16_t  voltage;     /* V  */
    int16_t  pitch;       /* °  */
} target_data_t;

typedef struct _elecgun_operations elecgun_operations_t;  

struct _elecgun_operations
{
    int16_t (*voltage)(void);
    int16_t (*pitch)(void);
};


typedef struct _elec_device elec_device_t;

struct _elec_device
{
  void (*init)(void);
  void (*mode1)(void);
  void (*mode2)(void);
  void (*mode3)(void);
  void (*mode4)(void);
  void (*mode5)(void);
  const elecgun_operations_t *distance;
};




extern target_data_t target;
extern const elec_device_t elec_gun;

void elec_charge_test(void);




#endif


