#include <lpc11xx.h>
#include "XY_Key_Scan_Def.h"
#include "XY_Key_Scan_Func.h"

#include "../uart/uart0.h"

volatile uint8_t c_KeyChanged;
volatile uint8_t c_InputValue[N_SCAN_ROWS];		
static uint8_t c_InputLast[N_SCAN_ROWS];
static uint8_t c_InputCurrent[N_SCAN_ROWS];
static uint8_t scan_time;

// KeyScanInit - 键盘扫描初始化
void KeyScanInit(void)
{
	uint8_t i;
	c_KeyChanged = 0;
	scan_time = 0;	  
	LPC_GPIO0->DIR &= ~(0x1F << 3);
	LPC_GPIO0->DIR &= ~(0x07 << 8);	
	LPC_GPIO0->DATA &= ~(0x1F << 3);

	for(i=0; i<N_SCAN_ROWS; i++)
	{
		c_InputValue[i] = 0x07;
		c_InputLast[i] = 0x07;
		c_InputCurrent[i] = 0x07;
	}
}

// XY_Key_Scan - 键盘扫描函数
void XY_Key_Scan(void)
{	 	
	c_InputCurrent[scan_time] = ((LPC_GPIO0->DATA >> 8) & 0x07);	

	if(ClearShakeDo(&c_InputValue[scan_time], &c_InputLast[scan_time], c_InputCurrent[scan_time]))
	{
		c_KeyChanged = 1;  
	}
	scan_time++;
	if(scan_time >= 5)
	{
		scan_time = 0;
	}				 
	LPC_GPIO0->DIR &= ~(0x1F << 3);
	LPC_GPIO0->DIR |= (0x01 << (3 + scan_time));
	LPC_GPIO0->DATA &= ~(0x1F << 3);
}

// ClearShakeDo - 消抖函数
// 参数	:	cValue - 结果
//			cLast - 上次的按键状态
//			cCurrent - 当前按键状态
uint8_t ClearShakeDo(uint8_t *cValue, uint8_t *cLast, uint8_t cCurrent)
{
	uint8_t cKeyAND,cKeyOR,cKeyChange,cKeyValue;

	cKeyAND = cCurrent & *cLast;	//求与中间值
	cKeyOR = cCurrent | *cLast;	//求或中间值
	cKeyValue = (*cValue | cKeyAND) & cKeyOR;	//新的按键值
	cKeyChange = *cValue ^ cKeyValue;	//按键变更值

	*cLast = cCurrent;	//保存新状态
	*cValue = cKeyValue;	//保存新值

	return	cKeyChange;	//返回
}
