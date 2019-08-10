#ifndef __ELECCONTROL_H
#define __ELECCONTROL_H


typedef struct _target_data
{
    int16_t  distance;    /* cm */
    int16_t  yaw;         /* °  */
    int16_t  voltage;     /* V  */
    int16_t  pitch;       /* °  */
} target_data_t;



void elec_charge_test(void);




#endif


