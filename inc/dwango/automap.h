#pragma once

#include "types.h"

#pragma pack(push)
#pragma pack(2)
typedef struct tagAutoHeader {
	// 0x0002 是一个阶段号，我们读取时恒等于2
	WORD Magic;
	DWORD reserved[3];	// 一个filename指针(相对CG.exe) 某一版本固定
				// 两个chuck_data
} AutoHeader;

#pragma pack(pop)
