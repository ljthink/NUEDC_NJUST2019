#ifndef _KEY_H
#define _KEY_H

#include "system.h"


#define DISTANCE_SWITCH_MUX            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)


#define  ANY_KEY_PRESS                 (GPIO_PinRead(GPIO1,24)==1 ||\
                                        GPIO_PinRead(GPIO1,25)==1 ||\
                                        GPIO_PinRead(GPIO1,26)==1 ||\
                                        GPIO_PinRead(GPIO1,27)==1 ||\
                                        GPIO_PinRead(GPIO1,28)==1 ||\
                                        GPIO_PinRead(GPIO1,29)==1 ||\
                                        GPIO_PinRead(GPIO1,30)==1 ||\
                                        GPIO_PinRead(GPIO1,31)==1 ||\
                                        GPIO_PinRead(GPIO2,27)==0 ||\
                                        GPIO_PinRead(GPIO2,30)==0 ||\
                                        GPIO_PinRead(GPIO3,04)==0)

#define  NO_KEY_PRESS                  (GPIO_PinRead(GPIO1,24)==0 &&\
                                        GPIO_PinRead(GPIO1,25)==0 &&\
                                        GPIO_PinRead(GPIO1,26)==0 &&\
                                        GPIO_PinRead(GPIO1,27)==0 &&\
                                        GPIO_PinRead(GPIO1,28)==0 &&\
                                        GPIO_PinRead(GPIO1,29)==0 &&\
                                        GPIO_PinRead(GPIO1,30)==0 &&\
                                        GPIO_PinRead(GPIO1,31)==0 &&\
                                        GPIO_PinRead(GPIO2,27)==1 &&\
                                        GPIO_PinRead(GPIO2,30)==1 &&\
                                        GPIO_PinRead(GPIO3,04)==1)

typedef enum _key_name_t
{
  no_key      = 0U,
  key_m10     = 1U,
  key_p10     = 2U,
  key_m1      = 3U,
  key_p1      = 4U,
  key_mf      = 5U,
  key_pf      = 6U,
  key_ok      = 7U,
  key_sw      = 8U,    
  key_minus   = 9U,
  key_plus    = 10U,
  key_ok_o    = 11U,
}key_name_t;



//操作类型定义
typedef struct _key_operations key_operations_t;  

struct _key_operations
{
    uint8_t (*get)(uint8_t mode);
};


//器件类型定义，器件一般包含数据操作
typedef struct _key_device key_device_t;

struct _key_device
{
    void (*init)(void);
    void (*interrupt_init)(void);
    void (*barrier_check)(void);
    const key_operations_t *ops;
};


extern const key_device_t key;
void key_test(void);

#endif 
