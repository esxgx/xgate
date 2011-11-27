#include "SDL/SDL.h"
#include "gamecf.h"
#include "rect.h"


SDL_Surface *screen;

int main(int argc, char *argv[])
{
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	sbrk(0x1000000);

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_WM_SetCaption(SCREEN_TITLE, NULL);



	GraphInfo_Live();
	Graphic_Live();

	//GraphIdx_BurnBook();
#include "puk1/readmap.h"
// $0x19263
	MAP_Map map;
	MAP_readMapOne(&map, argv[1]);

	printf("showstring : %s\nxsiz: %ld, ysiz:%ld\n", map.string, map.xsiz, map.ysiz);

	int t = atoi(argv[2]),d=atoi(argv[3]);
_ttt:	SDL_FillRect(screen,NULL,0);
	DrawMap_SA(screen, &map, d,t);

	GraphRECT rect = {screen->w-100, 0, 100, 100};
	SDL_Rect sr = {screen->w-100, 0, 100, 100};
	SDL_FillRect(screen,&sr, 0);
	AutoMap_Draw(screen, &map, rect, d + 20, t - 30); 

	SDL_Flip(screen);

	// 循环等待关闭事件
	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) break;
		else if(event.type == SDL_KEYDOWN){
			t++;  goto _ttt;
		}else if(event.type == SDL_MOUSEBUTTONDOWN) {
			d++; goto _ttt;
		}

	}
	exit(0);
}

