#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t TimeArray[6] = {2023, 12, 26, 14, 00, 00};

/*
* 函数名:MyRTC_Init
* 功能:MyRTC初始化
* 参数:无
* 返回值:无
* 备注:无
*/
void MyRTC_Init(void)
{
	//1.使能RTC,PWR,BKP的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);
	//2.使能PWR_DR的DBP位
	PWR_BackupAccessCmd(ENABLE);
	//3.判断系统里面是否彻底掉电过,VBAT的电源也断开过
	if(BKP_ReadBackupRegister(BKP_DR1) == 0X7777)//没有掉电过
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		return;//返回不执行下面的初始化代码
	}
	//4.先写BKP_DR1寄存器的值为0X7777
	BKP_WriteBackupRegister(BKP_DR1, 0X7777);
	//5.开启外部低速时钟。
	RCC_LSEConfig(RCC_LSE_ON);
	//6.等待外部低速时钟启动完成
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//7.配置RTC使用外部低速时钟
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	//8.使能RTC,外部低速时钟
	RCC_RTCCLKCmd(ENABLE);
	//9.等待同步
	RTC_WaitForSynchro();
	//10.等待上一次操作完成
	RTC_WaitForLastTask();
	//11.配置时钟分频系数,由于使用外部时钟,而且RTC时钟频率要求1Hz,所以配置为32768 - 1,注意:调用这个写入和设置的函数时都要先进入配置模式才可以,但是这里我们不用进入,因为函数内部已经进入了配置模式,也带有退出配置模式的操作
	RTC_SetPrescaler(32768 - 1);//减一是因为计数器包含0
	//12.等待操作完成,可以在写入前调用,或者写入后立即调用.只要有写入设置的操作就要调用这个函数.
	RTC_WaitForLastTask();
}

/*
* 函数名:MyRTC_SetTime
* 功能:MyRTC设置时间
* 参数:无
* 返回值:无
* 备注:无
*/
void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	//将要设置的时间填写到time_date结构体中
	time_date.tm_year = TimeArray[0] - 1900;
	time_date.tm_mon = TimeArray[1] - 1;
	time_date.tm_mday = TimeArray[2];
	time_date.tm_hour = TimeArray[3];
	time_date.tm_min = TimeArray[4];
	time_date.tm_sec = TimeArray[5];
	//使用mktime函数将结构体的时间,转换为时间戳的秒.
	time_cnt = mktime(&time_date) - 8 * 60 * 60;
	//设置这个时间到RTC计数器
	RTC_SetCounter(time_cnt);
	//等待操作完成
	RTC_WaitForLastTask();
}

/*
* 函数名:MyRTC_Get_ChinaTime
* 功能:MyRTC转换为中国时区的一种笨方法
* 参数:无
* 返回值:无
* 备注:未验证,未验证,未验证
*/
void MyRTC_Get_ChinaTime(void)
{
	TimeArray[3] += 8;//中国是东8区,所以伦敦时间加8小时是国内时间
	if(TimeArray[3] >= 24)//如果加了8小时之后大于等于24,那就是说明一天过去了
	{
		TimeArray[3] -= 24;//减去24,得到一天过去的小时
		TimeArray[2] += 1;//日 加1
		if(TimeArray[2] > 28)//判断 日 加一之后是不是大于28
		{
			if((TimeArray[1] == 2) && (((TimeArray[0] % 4 == 0) && (TimeArray[0] % 100 != 0)) || (TimeArray[0] % 400 == 0)))//判断是不是闰年2月
			{
				TimeArray[2] -= 28;//闰年闰月,此时日 超过了28,那么就减去 28,进入下一月
				TimeArray[1] += 1;//月 加1
			}
			else if(TimeArray[1] == 2 || TimeArray[1] == 4 || TimeArray[1] == 6 || TimeArray[1] == 9 || TimeArray[1] == 11)//如果此时是30天一月
			{
				if(TimeArray[2] > 30)//如果大于30天就说明已经超过了一个月的天数
				{
					TimeArray[2] -= 30;//减去三十
					TimeArray[1] += 1;//月份加一
//					if(TimeArray[1] > 12)//判断一下月份加一之后是否大于12月,这个可以不判断的,因为30天里面最大的十一月,十一月加1也才12月,不会大与12月
//					{
//						TimeArray[1] -= 12;
//						TimeArray[0] += 1;
//					}
				}
			}
			else//剩下的就是31天为一个月的情况
			{
				if(TimeArray[2] > 31)//大于三十一天
				{
					TimeArray[2] -= 31;//减31,剩余超过一个月的天数
					TimeArray[1] += 1;//月份加1
					if(TimeArray[1] > 12)//判断月份此时是不是大于12
					{
						TimeArray[1] -= 12;//大于就减去12
						TimeArray[0] += 1;//年数加一
					}
				}
			}
		}
	}
}

/*
* 函数名:MyRTC_GetTime
* 功能:MyRTC获取时间
* 参数:无
* 返回值:无
* 备注:无
*/
void MyRTC_GetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	//获取当前计数器的值
	time_cnt = RTC_GetCounter() /* + 8 * 60 * 60*/;//偏移8小时进入东八区时间
	//调用时间戳转时间结构体的函数
	time_date = *localtime(&time_cnt);
	//将结构体里面的值赋值给数组
	TimeArray[0] = time_date.tm_year + 1900;
	TimeArray[1] = time_date.tm_mon + 1;
	TimeArray[2] = time_date.tm_mday;
	TimeArray[3] = time_date.tm_hour;
	TimeArray[4] = time_date.tm_min;
	TimeArray[5] = time_date.tm_sec;
	//将时间转换为中国时间,也可以直接在上面获取的时间戳秒计数器加8个小时的秒数即可.
	MyRTC_Get_ChinaTime();
	
}

