#ifndef __NRF24L01_H
#define __NRF24L01_H

extern uint8_t TxBuf[];
extern uint8_t RxBuf[];


//IRQ
#define NRF24L01_IRQ_RCC RCC_APB2Periph_GPIOG
#define NRF24L01_IRQ_GPIO GPIOG
#define NRF24L01_IRQ_Pin GPIO_Pin_6
//CE
#define NRF24L01_CE_RCC RCC_APB2Periph_GPIOG
#define NRF24L01_CE_GPIO GPIOG
#define NRF24L01_CE_Pin GPIO_Pin_8

//NRF24L01指令
#define NRF24L01_Write 		0X20//使用时请寄存器地址或上这个值
#define NRF24L01_Read 		0X00//使用时请寄存器地址或上这个值
#define NRF24L01_Read_RX 	0X61//接收模式下读取RX_FIFO寄存器,1-32个字节,读取后自动清除RX_FIFO寄存器,接收模式下此指令有效
#define NRF24L01_Write_TX 	0XA0//发送模式下写入TX_FIFO寄存器,1-32个字节,发送模式下此指令有效
#define NRF24L01_Clear_TX 	0XE1//清除TX_FIFO寄存器,发送模式下此指令有效
#define NRF24L01_Clear_RX 	0XE2//清除RX_FIFO寄存器,接收模式下此指令有效
#define NRF24L01_Reuse_TX 	0XE3//重新使用上一包数据发送,CE电平状态为高(1)时数据包被不断发送,发送数据包的过程中必须要禁止数据包重利用功能,发送模式下此指令有效
#define NRF24L01_NOP 		0XFF//空操作,用来接收数据的

//发送地址
#define Tx_Address 0X5A4B4D0000
//通信频率
#define NRF24L01_RATE 119

//常用寄存器
#define NRF24L01_Tx_ADDR 0X10//发送地址
#define NRF24L01_Rx_ADDR_P0 0X0A//接收通道0地址
#define NRF24L01_EN_AA 0X01//使能每个通道的自动应答
#define NRF24L01_EN_RXADDR 0X02//使能每个通道的接收地址允许
#define NRF24L01_SETUP_RETR 0X04//设置自动重发延时和自动重发计数,高4位决定自动重发延时,低4位决定自动重发次数
#define NRF24L01_RF_CH 0X05//配置通信频率,0~126,寄存器低6位可用
#define NRF24L01_RF_SETUP 0X06//配置发射参数,高3位保留,位4配置为0,位3配置传输速率(0:1Mbps 1:2Mbps) 位2~位1配置发射功率 位0配置是否开启低噪声放大器增益
#define NRF24L01_Rx_Pw_P0 0X11//配置接收数据通道0有效数据宽度,低6位配置 范围:1~32
#define NRF24L01_CONFIG 0X00//配置NRF24L01基本工作参数和工作模式,最高位保留
#define NRF24L01_STATUS 0X07//状态寄存器


//发送错误信息
#define NRF24L01_Tx_OK 0XF0
#define NRF24L01_Tx_UNKNOWN 0XFF//未知发送错误
#define NRF24L01_Tx_ERROR_MAXRT 0XF1//发送错误,已超过最大重发次数

//接收错误信息
#define NRF24L01_Rx_OK 0X00
#define NRF24L01_Rx_NULL 0X01//未收到数据

//不存在设备
#define NRF24L01_ABSENT 0X00//NRF24L01不存在,或者不在对应线路上
#define NRF24L01_EXIST 0X01//NRF24L01存在

void NRF24L01_Init(void);
void NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value);
uint8_t NRF24L01_Read_Reg(uint8_t Reg);
void NRF24L01_Read_Buf(uint8_t Reg, uint8_t* Buf, uint8_t Count);
void NRF24L01_Write_Buf(uint8_t Reg, uint8_t* Buf, uint8_t Count);
void NRF24L01_Tx_Mode_Init(void);
uint8_t NRF24L01_TxPacket(uint8_t* Buf, uint8_t Count);
void NRF24L01_Rx_Mode_Init(void);
uint8_t NRF24L01_Check(void);

#endif
