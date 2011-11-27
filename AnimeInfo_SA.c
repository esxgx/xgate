#include "dwango/spr.h"

static unsigned iRef, isiz;
static SprAdrn *SprA;

__AnimeInfo_Size()
{
	if (!iRef) {
		isiz = mFile_Size(2);
		iRef = 1;
	}
	return isiz;
}

static
AnimeInfo_Map()
{
	if (iRef <= 1) {
		SprA = (SprAdrn *)mFile_Get(2, 0, __AnimeInfo_Size());
		if (SprA != NULL) {
			iRef = 2;	/* isiz = 1, malloc = 2 */
			return TRUE;
		}
		return 2;
	}
	return FALSE;
}

AnimeInfo_Live()
{
	if (AnimeInfo_Map() < 2) {
		++iRef;
		return TRUE;
	}
	return FALSE;
}

static
AnimeInfo_Unmap()
{
	if (iRef == 2) {
		mFile_Put(2, SprA, __AnimeInfo_Size());
		iRef = 1;
	}
}

#include "debug.h"

AnimeInfo_UnLive()
{
	--iRef;
	AnimeInfo_Unmap();
}

SprAdrn *AnimeInfo_Get(idx)
{
	if (AnimeInfo_Map() < 2) {
//		if (SprA[idx].Index == idx) {
			++iRef;
			return &SprA[idx];
//		}
	}
	return NULL;
}

AnimeInfo_Put(idx, p)
	SprAdrn *p;
{
	--iRef;
	AnimeInfo_Unmap();
	return 1;
}

#include <stdio.h>

AnimeInfo_Total()
{
	return __AnimeInfo_Size() / sizeof(SprAdrn);
}
