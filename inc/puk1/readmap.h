#pragma once

#include "types.h"

typedef struct tagMAP_Map
{
	DWORD id;
	DWORD xsiz, ysiz;
	char string[64];
	WORD **layer[3];
//	MAP_Objlink** olink;
	UINT v2map;
	DWORD unkrown[2];
}MAP_Map;

