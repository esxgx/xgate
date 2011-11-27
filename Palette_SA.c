#include "types.h"

static DWORD Palette[16][256];
static WORD vPal;

#define is_vaildpal(x)	(vPal & (1<<(x)))
#define vaild_pal(x) (vPal |= (1<<(x)))

static DWORD REPal[] = {
// 反汇编后的调色盘:
	// 前16份调色板
	0x000000, 0x000080, 0x008000, 0x008080, 0x800000,
	0x800080, 0x808000, 0xC0C0C0, 0xC0DCC0, 0xF0CAA6,
	0x0000DE, 0x005FFF, 0xA0FFFF, 0xD25F00, 0xFFD250,	/* 0xF */
	0x28E128,
	// 后16份调色板
	0x96C3F5, 0x5FA0E1, 0x467DC3, 0x1E559B, 0x374146,
	0x1E2328, 0xF0FBFF, 0xA4A0A0, 0x808080, 0x0000FF,
	0x00FF00, 0x00FFFF, 0xFF0000, 0xFF00FF, 0xFFFF00,
	0xFFFFFF						/* 0xFF */
};

#include <stdio.h>
#include <fcntl.h>

void *
Palette_Get(n)
{
	char path[256];
	UINT fd;
	if (!is_vaildpal(n)) {
		sprintf(path, "bin/pal/palet_%02d.cgp", n);
		fd = open(path, O_RDONLY);
		
		if (fd >= 0) {
//			Palette[n] = malloc(sizeof(DWORD) * 256);
			int i;
			for(i = 0; i < 16; ++i)
				Palette[n][i] = REPal[i];
			while(i < 240) {
				read(fd, ((BYTE *)(Palette[n] + i)) + 2, 1);
				read(fd, ((BYTE *)(Palette[n] + i)) + 1, 1);
				read(fd, Palette[n] + i, 1);
				++i;
			}
			while(i < 256) {
				Palette[n][i] = REPal[i-240+16];
				++i;
			}
			vaild_pal(n);
			return Palette[n];
		}
		return NULL;
	}
	return Palette[n];
}

Palette_Put(p)
{
	return 1;
}

Graph_RGB(pal, i, r, g, b)
	DWORD *pal;
	BYTE i;
	BYTE *r, *g, *b;
{
	*r = pal[i] & 0xFF;
	*g = (pal[i] >> 8) & 0xFF;
	*b = pal[i] >> 16;
}

// #define Graph_RGB(pal,i,r,g,b) do {	\
//	r = ((char *)pal)[(i) * 4];	\
//	g = ((char *)pal)[(i) * 4 + 1];	\
//	b = ((char *)pal)[(i) * 4 + 2];	\
// } while(0)

