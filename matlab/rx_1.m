recive_flag = 0;
data_num = 10;
 %DATA

 

res = DATA(i);
num2str(res)
hex2dec(ans)
    
    if ( res == 55 && data_num == 10) /* 帧头条件 */
    {
      data_num = 1;
      
    }
    if (res == 0x53 && data_num == 1)  /* 欧拉角条件 */
    {
      data_num = 2;
      recive_flag = 1;
    }
    
    if (data_num > 1 && recive_flag == 1) /* 姿态数据 */
    {
      hwt_905buff[data_num] = res;
      data_num ++ ;
    }
  }