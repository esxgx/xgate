#include "types.h"

//
// 注意: 我们的基点是
// x□
// □□□
//

enum {		// 图块的位置定义
	TileWidth = 3,
	TileHeight = 2
};

enum {		// 半步的位置定义
	Tile_hWidth = 2,
	Tile_hHeight = 1
};


// 游标技术的头文件
#include "map/calc.h"
#include "puk1/graph.h"
#include <SDL/SDL.h>


// 绘制一个物件
Automap_DrawTile(SDL_Surface *s, unsigned n, int c[])
{
	int k;
	BOOL ret = FALSE;
	k = GraphIdx_Get(n);
	if (k > 0) {
		GraphInfo *t = (GraphInfo *)GraphInfo_Get(k);
		if (t != NULL) {
//			GraphHeader * h = (GraphHeader *)Graphic_Get(t->GraphOffset);
			unsigned col;
			BYTE sl;
			sl = Automap_GetColor(n);
			if (sl) {
				// 子过程——转换色彩
				{
					BYTE r,g,b;
					Graph_RGB(Palette_Get(0), sl, &r, &g,&b);
					col = SDL_MapRGB(s->format, r,g,b);
				}
				int sc[2];
				sc[0] = c[0]; sc[1] = c[1];
				unsigned cy,cx;
				cy = t->Eca[1];
				while(cy) {
					int p[2];
					p[0] = sc[0]; p[1] = sc[1];
					cx = t->Eca[0];
					while(cx) {
						// 绘制四个点
						SDL_PutPixel(s, p[0] + 1, p[1], col);
						SDL_PutPixel(s, p[0], p[1] + 1, col);
						SDL_PutPixel(s, p[0] + 1, p[1] + 1, col);
						SDL_PutPixel(s, p[0] + 2, p[1] + 1, col);
	
						DiamondMap_TileRuler_W(1, p); 	// 左画
						--cx;
					}
					DiamondMap_TileRuler_N(1, sc);		// 上画
					--cy;
				}
				ret = TRUE;
			}
		}
	}
	return ret;
}

#include "puk1/readmap.h"
#include "rect.h"

AutoMap_Draw(SDL_Surface *s, MAP_Map *p, GraphRECT sr, int x, int y)
{
	// 常量
	#define myl	(((sr.h / TileHeight) <<1)+2)
	int lr;
	for(lr=0; lr<2; ++lr) {
		int ct[2], st[2];
		ct[0] = x; ct[1] = y;
		st[0] = DiamondMap_TileRuler_YFix(0) + sr.x;
		st[1] = sr.y - Tile_hHeight;
		unsigned yl = 0;
		while(yl < myl) {
			int pt[2] = {ct[0], ct[1]};
			int xl = sr.w / TileWidth+1;
			while(xl) {
				// 这是一个技巧, 等同于 >= 0 且小于某数
				if ((unsigned)pt[0] < p->xsiz && (unsigned)pt[1] < p->ysiz)
						if (p->layer[lr][pt[1]][pt[0]])
							Automap_DrawTile(s, p->layer[lr][pt[1]][pt[0]], st);
				DiamondMap_TileRuler_RW(yl, pt, st);	/* 右移 */
				--xl;
			}
			DiamondMap_TileRuler_FL(yl, ct, st, sr.x);	/* 切换到下行 */
		}
	}
}


