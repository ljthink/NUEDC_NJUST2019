recive_flag = 0;
data_num = 10;
 %DATA

 

res = DATA(i);
num2str(res)
hex2dec(ans)
    
    if ( res == 55 && data_num == 10) /* ֡ͷ���� */
    {
      data_num = 1;
      
    }
    if (res == 0x53 && data_num == 1)  /* ŷ�������� */
    {
      data_num = 2;
      recive_flag = 1;
    }
    
    if (data_num > 1 && recive_flag == 1) /* ��̬���� */
    {
      hwt_905buff[data_num] = res;
      data_num ++ ;
    }
  }