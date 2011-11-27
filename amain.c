#include <SDL/SDL.h>
#include "dwango/spr.h"

main()
{
	int i,c = AnimeInfo_Total();
	for(i=0; i<c; ++i) {
		SprAdrn *s = (SprAdrn *)AnimeInfo_Get(i);
		printf("[%u] %u %x 0x%x 0x%x\n", i, s->Index, s->SprOffset, s->Count , s->Unknown);
	}
	return 0;
}
