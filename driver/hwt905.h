#ifndef __HWT905_H
#define __HWT905_H


typedef struct _attitude_data
{
  float Roll;
  float Pitch;
  float Yaw;
} attitude_data_t;


extern uint8_t hwt_905buff[11];





#endif

