#pragma once

#include "types.h"

// 以下的命名习惯来自于JSSM源代码和StoneAge服务器端源代码

typedef struct tagGraphInfo {
	DWORD	Index;
	DWORD	GraphOffset;
	DWORD	BlockLength;
	DWORD	dx, dy;		// 
				// 根据反汇编的结果, 目前
	DWORD	Width, Height;	// 数据不超过2字节(即dx,dy,width,height < 65536)

	// 跟据反汇编结果, Eca只有三个字节有效..
	BYTE	Eca[8];		// 图片特性(一般用于地图)
	// 其实就是mapset.txt里的前8组数据, 而石器不同是因为
	// 它的数据更多而已.
	// [0] 占地面积-东
	// [1] 占地面积-南
	// [2] 是否阻挡

	DWORD	MapNo;
} GraphInfo;

typedef struct tagGraphHeader {
	BYTE	Magic[2];	// 'RD'
//	WORD	Type;		// 图块类型
	BYTE	Version;	// 图片版本(奇数压缩, 偶数未压缩)
	BYTE	Type;		// 图片类型
	DWORD	Width, Height;
	DWORD	BlockLength;
} GraphHeader;
