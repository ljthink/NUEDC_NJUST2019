#include "system.h"

uint8_t hwt_905buff[11];
attitude_data_t attitude;

void hwt_905_calculate(uint8_t buff[])
{
		if(buff[0] == 0x55 && buff[1] == 0x53)
		{	//校验
			if( (uint8_t)(0x55 + 0x53 + buff[2] + buff[3] + buff[4] + buff[5] + 
				buff[6] + buff[7] + buff[8] + buff[9]) == buff[10] )
			{	//姿态解算
				attitude.Roll  = ((float)((signed short int)((((unsigned short int)buff[3])<<8) | buff[2]))) / 32768.0*180.0;
				attitude.Pitch = ((float)((signed short int)((((unsigned short int)buff[5])<<8) | buff[4]))) / 32768.0*180.0;
				attitude.Yaw   = ((float)((signed short int)((((unsigned short int)buff[7])<<8) | buff[6]))) / 32768.0*180.0;
			}
		}			
}







