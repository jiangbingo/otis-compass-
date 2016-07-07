#ifndef _XY_KEY_SCAN_INCLUDE_
#define _XY_KEY_SCAN_INCLUDE_

#include "XY_Key_Scan_Def.h"
#include "XY_Key_Scan_Func.h"

//保存键盘状态
extern volatile uint8_t c_InputValue[N_SCAN_ROWS];
//键盘状态改变标志
extern volatile uint8_t c_KeyChanged;

#endif
