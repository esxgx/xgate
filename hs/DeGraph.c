#include "SDL/SDL.h"
#include "types.h"
#include "rect.h"

#include <stdio.h>

Graph_RGB(pal, i, r, g, b)
	DWORD *pal;
	BYTE i;
	BYTE *r, *g, *b;
{
	*r = pal[i] & 0xFF;
	*g = pal[i] >> 8;
	*b = pal[i] >> 16;
}

// #define Graph_RGB(pal,i,r,g,b) do {	\
//	r = ((char *)pal)[(i) * 4];	\
//	g = ((char *)pal)[(i) * 4 + 1];	\
//	b = ((char *)pal)[(i) * 4 + 2];	\
// } while(0)

//
// 真令我意想不到, BUG竟然在这里, 下面两行有一个
// SDL_PutPixel(s,x+rect->x,(rect->h+rect->y-y-1),i);
// 而原来是
// SDL_PutPixel(s,x+rect->x,(rect->h+rect->y-y),i);
// 错误就是这样了, 大家都明白了吧!!!!
//
#define Graph_PixelLoop(s,rect,x,y,i) do {			\
	SDL_PutPixel(s,x+rect->x,(rect->h+rect->y-y-1),i);	\
	if ((++x)>=rect->w) {					\
		x = 0; y++;					\
	}							\
} while(0)

int Graph_Decompress(s, rect, data, pal)
	SDL_Surface *s;
	GraphRECT *rect;
	BYTE *data;
	BYTE *pal;
{
	BYTE by, f;
	WORD idx;
	DWORD x = 0,y = 0;

	SDL_LockSurface(s);

	while(y<rect->h) {
		by = *data++;

		// 其实魔力格式设计的不好, 我们还要多一个判断.
		f = (by >> 6);
		if (!f) idx = 0x100;
		else if (f == 2) idx = (BYTE)*data++;
		else if (f == 3) idx = 0x101;
		else break;

		f = (by >> 4) & 0x3;

/*
		if (f>=0 && f<=2) idx = 0x100;
		else if (f>=8 && f<=0xA) {
			idx = (BYTE)*data++;
			f-=8;
		} else if (f>=0xC && f<=0xE) {
			idx = 0x101;
			f-=0xC;
		} else break;
*/

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

		BYTE r,g,b;
		
		// 下面就是绘制了
		if (idx < 0x100) {	// 绘制定点色
			Graph_RGB(pal, idx, &r, &g, &b);

			DWORD v = SDL_MapRGB(s->format, r,g,b);
			while(L) {
				Graph_PixelLoop(s,rect,x,y,v);
				--L;
			}
		} else if (idx == 0x100) {	// 绘制原色串
			while(L) {
				Graph_RGB(pal, *data, &r, &g, &b);
				data++;
				Graph_PixelLoop(s,rect,x,y,SDL_MapRGB(s->format, r,g,b));
				--L;
			}

		} else {		// 绘制背景色
			// 数学算法优化
			y += (x + L) / rect->w;
			x = (x + L) % rect->w;
		}
	}
	SDL_UnlockSurface(s);
	return 0;
}
