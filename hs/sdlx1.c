//
// SDL_CreateRGBSurface与SDL_FreeSurface有质量问题.
// 为了兼容性, 我们要加一个包装.
//

#include <stdlib.h>
#include "SDL/SDL.h"

/* 
 * Calculate the pad-aligned scanline width of a surface
 */
Uint16 xSDL_CalculatePitch(w, bpp)
{
	/* Surface should be 4-byte aligned for speed */
	int pitch = w * bpp;
	switch (bpp) {
		case 1:
			pitch = (pitch+7)/8;
			break;
		case 4:
			pitch = (pitch+1)/2;
			break;
		default:
			break;
	}
	pitch = (pitch + 3) & ~3;	/* 4-byte aligning */
	return(pitch);
}

xSDL_FreeSurface (SDL_Surface *s)
{
	if (s->flags & SDL_PREALLOC == SDL_PREALLOC) {
		void *data = s->pixels;
		SDL_FreeSurface(s);
		free(data);
	} else SDL_FreeSurface(s);
}

#include "debug.h"

SDL_Surface *
xSDL_CreateRGBSurface (Uint32 flags, int width, int height, int depth,
	Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	if (flags & SDL_HWSURFACE)
		return SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
	SDL_Surface *surface;

	surface = SDL_CreateRGBSurface(flags, 0, 0, depth,Rmask, Gmask, Bmask, Amask);
	if ( surface != NULL ) {
		surface->flags |= SDL_PREALLOC;
		surface->pitch = xSDL_CalculatePitch(width, depth/8);
		fprintf(stderr, "pitch : %u, height = %u, weigh =%u", surface->pitch, height, width);
		surface->w = width;
		surface->h = height;
		surface->pixels = malloc(surface->pitch * height);
		ASSERT(surface->pixels != NULL);

		fprintf(stderr, " OK!\n");
		SDL_SetClipRect(surface, NULL);
	}
	return(surface);
}
