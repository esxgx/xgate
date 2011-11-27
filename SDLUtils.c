#include "SDL/SDL.h"
#include "debug.h"

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
static
__inline__
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

/*
	switch(bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
*/
		*(Uint16 *)p = pixel;
/*
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		} else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
*/
}

SDL_PutPixel(SDL_Surface *s, int x, int y, Uint32 pixel)
{
	if ((unsigned)x < s->w && (unsigned)y < s->h)
		putpixel(s, x, y, pixel);
}


