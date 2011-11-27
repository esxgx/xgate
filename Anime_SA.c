#include "dwango/spr.h"
#include "types.h"

static void *pAnime;
static unsigned aRef, asiz;

#include "debug.h"

static
Anime_Map()
{
	ASSERT(aRef >= 0);
	if (aRef < 1) {
		pAnime = (void *)mFile_Get(3, 0, asiz = mFile_Size(1));
		if (pAnime != NULL) {
			aRef = 1;
			return TRUE;
		}
		return 2;
	}
	return FALSE;
}

static
Anime_Unmap()
{
	ASSERT(aRef >= 1);
	if (aRef <= 1) {
		mFile_Put(3, pAnime, asiz);
		aRef = 0;
	}

}

Anime_Live()
{
	if (Anime_Map() < 2) {
		++aRef;
		return TRUE;
	}
	return FALSE;
}

Anime_Unlive()
{
	--aRef;
	Anime_Unmap();
}

void *
Anime_Get(ofs, len)
{
	if (Anime_Map() < 2) {
		++aRef;
		return pAnime + ofs;
	}
	return NULL;
}

Anime_Put(p)
	void *p;
{
	ASSERT(aRef > 1);		// 侦测Bug
	--aRef;
	Anime_Unmap();
}
