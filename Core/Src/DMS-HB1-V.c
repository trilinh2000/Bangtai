#include "DMS-HB1-V.h"
uint8_t Convert_bit(void)
{
	return ((HAL_GPIO_ReadPin(X05_GPIO_Port, X05_Pin))|(HAL_GPIO_ReadPin(X06_GPIO_Port, X06_Pin)<<1))&0xff;//0x00000001,0x00000010,0x00000011,0x00000000
}
void Convert_io(uint8_t bit)
{
	if(HAL_GPIO_ReadPin(X04_GPIO_Port,X04_Pin)==1)//k co hang o cambien 2
	{
		if(bit==0)
		{
			//confirm cho phep nhan
			HAL_GPIO_WritePin(Y03_GPIO_Port,Y03_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(Y04_GPIO_Port,Y04_Pin, GPIO_PIN_RESET);//00
			while(Convert_bit()!=3)
			{
				if(bit==1)//co hang tren xe bit truyen nhan
					{
						HAL_GPIO_WritePin(Y03_GPIO_Port,Y03_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(Y04_GPIO_Port,Y04_Pin, GPIO_PIN_RESET);//01
						while(HAL_GPIO_ReadPin(X03_GPIO_Port,X03_Pin)==1||HAL_GPIO_ReadPin(X04_GPIO_Port,X04_Pin)==0)//co tin hieu cam bien 2 va k co tin hieu cb1
						{
							Motor_Run();
						}
						Motor_Stop();
						//confirm success
						HAL_GPIO_WritePin(Y03_GPIO_Port,Y03_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(Y04_GPIO_Port,Y04_Pin, GPIO_PIN_SET);//11
					}
				if(bit==2)//tren xe k co han
				{
					Motor_Stop();
					HAL_GPIO_WritePin(Y03_GPIO_Port,Y03_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(Y04_GPIO_Port,Y04_Pin, GPIO_PIN_SET);//11
				}
			}
			HAL_GPIO_WritePin(Y03_GPIO_Port,Y03_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(Y04_GPIO_Port,Y04_Pin, GPIO_PIN_SET);
		}

	}
	else {
		while(HAL_GPIO_ReadPin(X03_GPIO_Port,X03_Pin)==1)//k co hang o cam bien 1
		{
			Motor_Run();
		}
		Motor_Stop();
	}

}

