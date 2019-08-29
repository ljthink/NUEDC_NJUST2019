#ifndef __KS103_H
#define __KS103_H

#include "system.h"

/* SCL-B11-B101 SDA-C11-B102 */
#define SCL(n)    GPIO_PinWrite(GPIO1,16,n)
#define SDA(n)    GPIO_PinWrite(GPIO1,17,n)
#define READ_SDA  GPIO_PinRead(GPIO1,17)


/* 地址定义 */
#define KS103_ADD   0xe8
#define KS103_REG1  0x02
#define KS103_REG2  0x03
#define DISTANCE_5M 0xb0

uint8_t KS103_ReadOneByte(uint8_t address, uint8_t reg);
void KS103_WriteOneByte(uint8_t address,uint8_t  reg,uint8_t  command);


static inline void ks103_get_distance(void)
{
  KS103_WriteOneByte(0xe8,0x02,0xb0);
  delayms(50); 
  target.distance_ks103 = KS103_ReadOneByte(0xe8, 0x02);
  target.distance_ks103 <<= 8;
  target.distance_ks103 += KS103_ReadOneByte(0xe8, 0x03);
  target.distance_ks103 = target.distance_ks103/10 - 30;
}
 
void ks103_init(void);
void ks103_test(void);
void ks103_get_distance(void);



#endif



