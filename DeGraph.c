#include "SDL/SDL.h"
#include "types.h"
#include "xsurf.h"

int Graph_Decompress(s, length, data, pal)
	XSurface *s;
	DWORD length;
	BYTE *data;
{
	BYTE by, f;
	WORD idx;
	BYTE *ptr, *end;
	UINT w;

	ptr = s->pixels;
	end = data + length;

	while(data < end) {
		by = *data++;

		// 其实魔力格式设计的不好, 我们还要多一个判断.
		f = (by >> 6);
		if (!f) idx = 0x100;
		else if (f == 2) idx = (BYTE)*data++;
		else if (f == 3) idx = 0x101;
		else break;

		f = (by >> 4) & 0x3;

		DWORD L;

		switch(f) {
			case 0:
				L = by & 0xF;
				break;
			case 1:
				L = (by & 0xF) * 0x100 + (*data++);
				break;
			case 2:
				L = (by & 0xF) * 0x10000 + ((BYTE)(*data++)) * 0x100;
				L += ((BYTE)(*data++));
				break;
		}
		
		// 下面就是绘制了
		if (idx < 0x100)		// 绘制定点色
			memset(ptr, idx, L);
		else if (idx == 0x100){		// 绘制原色串
			memcpy(ptr, data, L);
			data += L;
		}
		// 绘制背景色的代码彻底淘汰.
		ptr+=L;
	}

	BYTE tmpl[s->width * (s->bpp>>3)];
	UINT y;
	for(y=0; y < s->height>>1; ++y) {
		memcpy(tmpl, &s->pixels[s->width * (s->bpp>>3) *y], s->width * (s->bpp>>3));
		memcpy(&s->pixels[s->width * (s->bpp>>3) *y],
			&s->pixels[s->width * (s->bpp>>3) *(s->height-1-y)],
			s->width * (s->bpp>>3));
		memcpy(&s->pixels[s->width * (s->bpp>>3) *(s->height-1-y)], tmpl,
			s->width * (s->bpp>>3));
	}
	return 0;
}
