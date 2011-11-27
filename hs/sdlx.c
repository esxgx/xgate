#include "SDL/SDL.h"

xSDL_FreeSurface (SDL_Surface *s)
{
	SDL_FreeSurface(s);
}

SDL_Surface *
xSDL_CreateRGBSurface (Uint32 flags, int width, int height, int depth,
	Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
	return SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
}
