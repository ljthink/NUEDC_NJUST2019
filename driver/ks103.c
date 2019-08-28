#include "system.h"



void IIC_Init(void)
{
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_GPIO2_IO17,   0U);/* SCL-B11-B101 */
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_GPIO2_IO18,   0U);/* SDA-C11-B102 */
  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_GPIO2_IO17, 0x10B0u); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_GPIO2_IO18, 0x10B0u);
  
  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                      1,                      //高电平
                                      kGPIO_NoIntmode         //非中断模式
                                      };
  
  GPIO_PinInit(GPIO2,17, &GPIO_Output_Config);
  GPIO_PinInit(GPIO2,18, &GPIO_Output_Config);  
}

void SDA_IN(void)
{
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_GPIO2_IO18,0U);/*SDA-C11*/
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_GPIO2_IO18,0xF080);  
  gpio_pin_config_t GPIO_Input_Config = {kGPIO_DigitalInput,    //GPIO为输入方向
                               1,                               //高电平
                               kGPIO_NoIntmode,                 //不触发中断
                               };  
  GPIO_PinInit(GPIO2,18, &GPIO_Input_Config);
}
void SDA_OUT(void)
{
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_GPIO2_IO18,0U);/*SDA-C11*/
  IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_GPIO2_IO18,0x10B0u);
  gpio_pin_config_t GPIO_Output_Config = {kGPIO_DigitalOutput, //GPIO为输出方向
                                      1,                      //高电平
                                      kGPIO_NoIntmode         //非中断模式
                                      };
  GPIO_PinInit(GPIO2,18, &GPIO_Output_Config);
}


void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	SDA(1);	  	  
	SCL(1);
	delayus(10);
 	SDA(0);//START:when CLK is high,DATA change form high to low 
	delayus(10);
	SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  

void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	SCL(0);
	SDA(0);//STOP:when CLK is high DATA change form low to high
 	delayus(10);
	SCL(1); 
	SDA(1);//发送I2C总线结束信号
	delayus(10);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	SDA(1);
  delayus(6);	   
	SCL(1);
  delayus(6);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	SCL(0);
	SDA_OUT();
	SDA(0);
	delayus(10);
	SCL(1);
	delayus(10);
	SCL(0);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	SCL(0);
	SDA_OUT();
	SDA(1);
	delayus(10);
	SCL(1);
	delayus(10);
	SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   
	SDA_OUT(); 	    
  SCL(0);//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    SDA((txd&0x80)>>7);
    txd<<=1; 	  
		delayus(10);   
		SCL(1);
		delayus(10); 
		SCL(0);	
		delayus(10);
  }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SCL(0); 
        delayus(10);
		SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;   
		delayus(5); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


void KS103_WriteOneByte(uint8_t address,uint8_t  reg,uint8_t  command)
{				   	  	    																 
  IIC_Start();   
	IIC_Send_Byte(address);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();	   
  IIC_Send_Byte(command);
	IIC_Wait_Ack(); 	 										  		   
  IIC_Stop();
}

uint8_t KS103_ReadOneByte(uint8_t address, uint8_t reg)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start();  
  IIC_Send_Byte(address);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack();	 
	IIC_Send_Byte(reg);   //·¢ËÍµÍµØÖ·
	IIC_Wait_Ack();	   
	IIC_Start();  	 	   
	IIC_Send_Byte(address + 1);           //½øÈë½ÓÊÕÄ£Ê½			   
	IIC_Wait_Ack();	 
  
	delayus(50);	   //Ôö¼Ó´Ë´úÂëÍ¨ÐÅ³É¹¦£¡£¡£¡
  temp=IIC_Read_Byte(0);	  //¶Á¼Ä´æÆ÷3	   
  IIC_Stop();//²úÉúÒ»¸öÍ£Ö¹Ìõ¼þ	    
	return temp;
}

void ks103_test(void)
{	
  oled.init();
  IIC_Init();
  uint16_t range;
  float distance;
  char txt[16];
	while(1)
	{
    KS103_WriteOneByte(0xe8,0x02,0x1f);
    delayms(100); 
    range = KS103_ReadOneByte(0xe8, 0x02);
    range <<= 8;
    range += KS103_ReadOneByte(0xe8, 0x03);
    distance = range*340/2/10000;
    sprintf(txt,"%3.1f",distance);
    oled.ops->word(0,0,txt);
  }
}
