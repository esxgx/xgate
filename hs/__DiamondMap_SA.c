// Isometric Diamond (SA/XG) Map Functions

#include "puk1/graph.h"
#include "puk1/readmap.h"
#include "rect.h"

enum {	// 这是魔力地图砖块的数据
	TileWidth = 64,
	TileHeight = 48 //47
};

// 最重要的就是这两组啦!
#define DiamondMap_TilePlotter_X(x, y) (((x) - (y)) * TileWidth / 2)
#define DiamondMap_TilePlotter_Y(x, y) (((x) + (y)) * TileHeight / 2)

#include "SDL/SDL.h"
#include <stdio.h>

static
DrawTile_SA(SDL_Surface *s, int mapno, int sx, int sy)
{
	int k = GraphIdx_Get(mapno);
	if (k > 0) {
		GraphInfo *t = (GraphInfo *)GraphInfo_Get(k);
		if (t != NULL) {
			#define dat Graphic_Get(t->GraphOffset)+sizeof(GraphHeader)
			#define pal Palette_Get()
			SDL_Surface *m = (SDL_Surface *)Graph_GetSurface(t->Width, t->Height, dat, pal);
			if (m) {
				#undef pal
				#undef dat
				SDL_Rect r;
				//fprintf(stderr, "%d %d (%d %d)\n", t->dx, t->dy, TileWidth - t->Width, TileHeight - t->Height);

				r.x = sx; r.y = sy;
				int ret = !SDL_BlitSurface(m, NULL, s, &r);
				Graph_FreeSurface(m);
				return ret;
			}
		}
	}
	return FALSE;
}

DrawMap_SA(SDL_Surface *s, MAP_Map *p, int x, int y)
{
	#define myl	(((s->h / TileHeight) <<1)+1)
	int yl, xl;
	int cx, cy;
	int sx, sy;
	int pt[2];
	int lr;

	for(lr = 0; lr<2; ++lr) {
		pt[0] = x; pt[1] = y;
		sy = -TileHeight/2;
		for(yl = -1; yl<myl-1; ++yl) {
			xl = s->w / TileWidth+1;
			cx = pt[0]; cy = pt[1];
			sx = (yl & 1) * (TileWidth/2)-TileWidth;
			while(xl) {
				if (cx>=0 && cy>=0 && cx<p->xsiz && cy<p->ysiz) {
						if (p->layer[lr][cy][cx]) {
//							fprintf(stderr, "DDD: %d %d (%x, %x) %d\n", sx, sy, cx, cy, p->layer[lr][cy][cx]);
							DrawTile_SA(s, p->layer[lr][cy][cx], sx, sy);
						}
				}
				++cx; --cy;
				sx+=TileWidth;
				--xl;
			}
			sy+=TileHeight>>1;
			pt[yl & 1]++;
		}
	}
}
/*
DrawMap_SA(SDL_Surface *s, MAP_Map *p, int pt[])
{
	int sx, sy;
	for(sy = 0; sy<s->h; ++sy) {
		for(sx = 0; sx<s->w; ++sx) {
			fprintf(stderr, "%d %d\n", sy, sx);
			DrawTile_SA(s, 1000, sx, sy);
		}
	}
}
*/

