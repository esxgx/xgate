#include "SDL/SDL.h"
#include "gamecf.h"
#include "rect.h"

#include "types.h"


SDL_Surface *screen;

Palette_Gen()
{
	BYTE r,g,b;
	int i = 0;
	while(i<256) {
		Graph_RGB(Palette_Get(), i++, &r, &g,&b);
		SDL_PutPixel(screen, (i*4) %64, (i*4) /64 , SDL_MapRGB(screen->format, r,g,b));
		SDL_PutPixel(screen, ((i*4) + 1) %64, ((i*4) + 1) /64 , SDL_MapRGB(screen->format, r,g,b));
		SDL_PutPixel(screen, ((i*4) + 2) %64, ((i*4) + 2) /64 , SDL_MapRGB(screen->format, r,g,b));
		SDL_PutPixel(screen, ((i*4) + 3) %64, ((i*4) + 3) /64 , SDL_MapRGB(screen->format, r,g,b));
	}

}


int main(int argc, char *argv[])
{
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_WM_SetCaption(SCREEN_TITLE, NULL);
	GraphInfo_Live();
	int pt[] = {300, 300};

	

	// 1xxxx NPC图档
	// 2xxxx 物品图档
//	__temp_mapnoview(screen, atoi(argv[1]));
//	Automap_DrawTile(screen, atoi(argv[1]), pt);

	Palette_Gen();

	SDL_Flip(screen);
	// 循环等待关闭事件
	while (SDL_WaitEvent(&event)) {
		if (event.type == SDL_QUIT) break;
	}
	exit(0);
}
