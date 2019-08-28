#ifndef __KS103_H
#define __KS103_H

#include "system.h"

/* SCL-B11-B101 SDA-C11-B102 */
#define SCL(n)    GPIO_PinWrite(GPIO2,17,n)
#define SDA(n)    GPIO_PinWrite(GPIO2,18,n)
#define READ_SDA  GPIO_PinRead(GPIO2,18)


/* 地址定义 */
#define KS103_ADD   0xe8
#define KS103_REG1  0x02
#define KS103_REG2  0x03
#define DISTANCE_5M 0xb0


void ks103_test(void);

#endif



