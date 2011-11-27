#include "SDL/SDL.h"
#include "types.h"
#include "rect.h"
#include "debug.h"

/*

SDL_Surface *ImCa_s[100];
#define _IMCA	(sizeof(ImCa)/sizeof(SDL_Surface *))
unsigned ImCa_n[_IMCA];
void *ImCa_pal[_IMCA];

unsigned ImCp[_IMCA + 1];

Graph_GetSDLSurface(n, pal)
	void *pal;
{
	unsigned p = _IMCA, e;
	while(ImCp[p]) {
		e = ImCp[p] - 1;
		// 如果找到图像, 则从链条中删除
		if (ImCa_n[e] == n && ImCa_pal == pal) {
			ImCp[p] = ImCp[e];
			break;
		}
	}
	if (!ImCp[p]) {
	

	}

}

*/

Graph_FreeSurface(SDL_Surface *s)
{
	ASSERT(s != NULL);
	SDL_FreeSurface(s);
}

SDL_Surface *
Graph_GetSurface(UINT w, UINT h, void *data, void *pal)
{
	SDL_Surface *scr = SDL_GetVideoSurface();
	SDL_Surface *dst = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		w,
		h,
		scr->format->BitsPerPixel,
		scr->format->Rmask,
		scr->format->Gmask,
		scr->format->Bmask,
		scr->format->Amask
	);
	if (dst != NULL) {
		SDL_SetColorKey(dst, SDL_SRCCOLORKEY, dst->format->colorkey);
		GraphRECT rect;
		rect.x = 0; rect.y = 0; rect.w = w; rect.h = h;

		if (!Graph_Decompress(dst, &rect, data, pal)) {
			return dst;
		}
		SDL_FreeSurface(dst);
	}
	return NULL;
}
