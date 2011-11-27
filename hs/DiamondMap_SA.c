//
// 注意: 魔力的地图格式有所不同.
// 假设读入的3X3地图数据顺序为123456789,对应的地图显示:
//     3
//   2   6
// 1   5   9
//   4   8
//     7
// 而不是现在通用的:
//     1
//   2   4
// 3   5   7
//   6   8
//     9
//
// 斜视角地图游标技术:
// 为了绘制的效率和简便性, 先进的斜视角引擎往往采用此类技术.
// 详见: "Introduction to Isometric Engines"
// 这里只给出魔力地图方法:
//      (2,0)
//   (1,0) (2,1)
//(0,0) (1,1) (2,2)
//   (0,1) (1,2)
//      (0,2)
// 直角坐标系向右移一格: x和y值均+1
// 直角坐标系向下移一格:
//   (2,0)		-> 偶数行
//      (2,1)		-> 奇数行(y +1)
//   (1,1)		-> 偶数行(x -1)
//

#include "puk1/graph.h"
#include "puk1/readmap.h"
#include "rect.h"

enum {	// 这是魔力地图砖块的数据
	TileWidth = 64,
	TileHeight = 48 //47
};

//
// 斜视角绘制需要半步的起始点, 而有时这个值
// 不是简单除以2了事的(例如自动地图), 所以
// 需要设定它.
//
enum {
	Tile_hWidth = TileWidth >> 1,
	Tile_hHeight = TileHeight >> 1
};

#include "map/calc.h"

#include "SDL/SDL.h"
#include <stdio.h>

/*
static
DrawTile_SA(SDL_Surface *s, int n, int sx, int sy)
{
	int k;
	BOOL ret = FALSE;
	k = GraphIdx_Get(n);
	if (k > 0) {
		GraphInfo *t = (GraphInfo *)GraphInfo_Get(k);
		if (t != NULL) {
			#define dat Graphic_Get(t->GraphOffset)+sizeof(GraphHeader)
			#define pal Palette_Get(0)
			SDL_Surface *m = (SDL_Surface *)Graph_GetSurface(t->Width, t->Height, dat, pal);
			#undef pal
			#undef dat
			if (m != NULL) {
				SDL_Rect r;
				r.x = sx + t->dx; r.y = sy + t->dy;
				ret = !SDL_BlitSurface(m, NULL, s, &r);
				Graph_FreeSurface(m);
			}
			GraphInfo_Put(k, t);
		}
	}
	return ret;
}
*/

static
__inline__
DrawTile_SA(SDL_Surface *s, int n, int sx, int sy)
{
	int k;
	BOOL ret = FALSE;
	k = GraphIdx_Get(n);
	if (k > 0) {
		GraphInfo *t = (GraphInfo *)GraphInfo_Get(k);
		if (t != NULL) {
			GraphRECT rect;

			rect.x = sx + t->dx;
			rect.y = sy + t->dy;
			rect.w = t->Width;
			rect.h = t->Height;

			#define dat Graphic_Get(t->GraphOffset)+sizeof(GraphHeader)
			#define pal Palette_Get(0)
			ret = !Graph_Decompress(s, &rect, dat, pal);
			#undef pal
			#undef dat
			GraphInfo_Put(k, t);
		}
	}
	return ret;
}

DrawMap_SA(SDL_Surface *s, MAP_Map *p, int x, int y)
{
// 常量
#define myl	(((s->h / TileHeight) <<1)+2)
	int lr;
	for(lr=0; lr<2; ++lr) {
		int ct[2], st[2];
		ct[0] = x; ct[1] = y;
		st[0] = DiamondMap_TileRuler_YFix(0);
		st[1] = -Tile_hHeight;
		int yl = 0;
		while(yl < myl) {
			int pt[2] = {ct[0], ct[1]};
			int xl = s->w / TileWidth+1;
			while(xl) {
				// 这是一个技巧, 等同于 >= 0 且小于某数
				if ((unsigned)pt[0] < p->xsiz && (unsigned)pt[1] < p->ysiz)
						if (p->layer[lr][pt[1]][pt[0]])
							DrawTile_SA(s, p->layer[lr][pt[1]][pt[0]], st[0], st[1]);
				DiamondMap_TileRuler_RW(yl, pt, st);	/* 右移 */
				--xl;
			}
			DiamondMap_TileRuler_FL(yl, ct, st, 0);		/* 切换到下行 */
		}
	}


}

