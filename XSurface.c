#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "xsurf.h"

XSurface_Set(
	XSurface *x,
	UINT w, UINT h,
	BYTE bpp,
	DWORD flags,
	DWORD ck,
	void *pal
){
	x->pixels = malloc((bpp>>3) * w *h);
	if (x->pixels != NULL) {
		x->width = w;
		x->height = h;
		x->bpp = bpp;
		x->flags = flags;
		if (flags & 2) {
			x->colorkey = ck;
			memset(x->pixels, ck, (bpp>>3) * w *h);
		} else memset(x->pixels, 0, (bpp>>3) * w *h);
		if (flags & 1) x->palette = pal;
		return TRUE;
	}
	return FALSE;
}

XSurface_Put(x)
	XSurface *x;
{
	if (x->pixels != NULL) {
		free(x->pixels);
		x->width = x->height = 0;
		return TRUE;
	}
	return FALSE;
}

#include <SDL/SDL.h>

SDL_Surface *
XSurface_ConvertToSDLSurface(XSurface *x)
{
	SDL_Surface *s;
	if (x->flags & 1) {
		s = SDL_CreateRGBSurfaceFrom(x->pixels,
			x->width,
			x->height,
			x->bpp,
			x->width * (x->bpp >> 3),
			0,0,0,0);
		SDL_SetPalette(s, SDL_LOGPAL|SDL_PHYSPAL, (SDL_Color *)x->palette, 0, 256);
	} else {
		s = SDL_CreateRGBSurfaceFrom(x->pixels,
				x->width,
				x->height,
				x->bpp,
				x->width * (x->bpp >> 3),
				x->mask[0],
				x->mask[1],
				x->mask[2],
				x->mask[3]
		);
	}
	if (x->flags & 2)
		SDL_SetColorKey(s, SDL_SRCCOLORKEY, x->colorkey);
	return s;
}
