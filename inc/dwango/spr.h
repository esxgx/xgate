#pragma once

#include "types.h"

typedef struct tagSprAdrn {
	DWORD	Index;
	DWORD	SprOffset;
	WORD	Count;
	WORD	Unknown;
} SprAdrn;

typedef struct tagSpr {
	BYTE	Unknown[8];
	DWORD	Frames;
} Spr;

typedef struct tagSprFrame {
	DWORD	Index;
	BYTE	Unknown[6];
} SprFrame;
