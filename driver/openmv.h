#ifndef __OPENMV_H
#define __OPENMV_H




extern uint8_t mv_buff[15];
extern uint8_t mv_buff_ready;
extern uint16_t target_pix_x;


void openmv_data_refresh(void);
void openmv_data_test(void);


#endif



