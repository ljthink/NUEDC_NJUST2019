#ifndef __ELEC_GUN
#define __ELEC_GUN

#define ON  0
#define OFF 1 
#define LEVEL1(x) GPIO_PinWrite(GPIO1,22, x)
#define LEVEL2(x) GPIO_PinWrite(GPIO1,23, x)
#define LEVEL3(x) GPIO_PinWrite(GPIO1,16, x)

void elec_signalled_test(void);




#endif



