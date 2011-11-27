#pragma once
#include "types.h"

typedef struct tagXSurface {
	BYTE *pixels;
	/* used for converting to SDL_Surface */
	WORD bpp, flags;
	DWORD colorkey;
	UINT width, height;
	union {
		void *palette;
		struct {
			unsigned mask[4];
		};
	};
} XSurface;
