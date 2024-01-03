#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "NRF24L01.h"

/*测试需要两个NRF24L01,一个接收一个发送*/
/*此代码为接收端*/

int main(void)
{
	OLED_Init();//OLED初始化
	
	NRF24L01_Init();//NRF24L01初始化
	//检查NRF24L01是否存在
	OLED_ShowString(4, 1, "OFF");
	if(NRF24L01_Check() == NRF24L01_EXIST)
	{
		OLED_ShowString(4, 1, "   ");
		OLED_ShowString(4, 1, "OK");
	}
	
	
	NRF24L01_Rx_Mode_Init(5);//初始化为接收模式
	
	while(1)
	{
		OLED_ShowNum(1, 1, GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1), 2);//显示中断引脚的电平状态
		
		/*显示接收到的数据*///等待接收即可
		OLED_ShowHexNum(2, 1, RxBuf[0], 2);
		OLED_ShowHexNum(2, 4, RxBuf[1], 2);
		OLED_ShowHexNum(2, 7, RxBuf[2], 2);
		OLED_ShowHexNum(2, 10, RxBuf[3], 2);
		OLED_ShowHexNum(2, 13, RxBuf[4], 2);
	}
}
