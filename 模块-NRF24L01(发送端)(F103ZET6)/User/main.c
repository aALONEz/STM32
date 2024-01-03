#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "NRF24L01.h"

/*测试需要两个NRF24L01,一个接收一个发送*/

int main(void)
{
	/*由于SPI2还链接了W25Q128为了防止发生不必要的错误,这里将W25Q128的片选引脚给初始化为不选中*/
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推完输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//3.设置为高电平
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	
	
	OLED_Init();//OLED初始化
	
	NRF24L01_Init();//NRF24L01初始化
	//检查NRF24L01是否存在
	OLED_ShowString(4, 1, "OFF");
	if(NRF24L01_Check() == NRF24L01_EXIST)
	{
		OLED_ShowString(4, 1, "   ");
		OLED_ShowString(4, 1, "OK");
	}
	
	//初始化为发送模式
	NRF24L01_Tx_Mode_Init();
	
	TxBuf[0] = 0X11;
	TxBuf[1] = 0X12;
	TxBuf[2] = 0X13;
	TxBuf[3] = 0X14;
	TxBuf[4] = 0X15;
	
	while(1)
	{
		OLED_ShowHexNum(1, 1, NRF24L01_TxPacket(TxBuf, 5), 2);//循环发送,TxBuf中的五个字节
	}
}
