#include "rect.h"
#include "puk1/graph.h"
#include "SDL/SDL.h"
#include <stdio.h>

void __temp_mapnoview(SDL_Surface *s, int mapno)
{
	int t = GraphIdx_Get(mapno);
	if (t >= 0) {
		GraphRECT rect;
		int i = 0;
		rect.x = rect.y = 0;
		GraphInfo *p = (GraphInfo *)GraphInfo_Get(t);
		rect.w = p->Width;
		rect.h = p->Height;
		GraphHeader * h = (GraphHeader *)Graphic_Get(p->GraphOffset);
//		BYTE r,g,b,idx;
//		Graph_RGB(Palette_Get(), idx = Automap_GetColor(mapno), &r, &g,&b);

//		fprintf(stderr, "idx = %x, (%u,%u,%u)\n", idx, r, g,b);

//		SDL_FillRect(s, NULL, SDL_MapRGB(s->format, r,g,b));

//		while(rect.y<s->h) {
			rect.x = 0;
//			while(rect.x<s->w) {
				Graph_Decompress(s, &rect, h+1, Palette_Get());
//				rect.x += p->Width;
//			}
//			rect.y+= p->Height;
//		}
		GraphInfo_Put(t, p);
	}
}
