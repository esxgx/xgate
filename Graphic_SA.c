#include "puk1/graph.h"
#include "types.h"

static int gRef, gsiz;
static void *pGraph;

#include "debug.h"

static
Graphic_Map()
{
	ASSERT(gRef >= 0);
	if (gRef < 1) {
		pGraph = (void *)mFile_Get(1, 0, gsiz = mFile_Size(1));
		if (pGraph != NULL) {
			gRef = 1;
			return TRUE;
		}
		return 2;
	}
	return FALSE;
}

static
Graphic_Unmap()
{
	ASSERT(gRef >= 1);
	if (gRef <= 1) {
//		mFile_Put(0, pGraph, gsiz);		// BUG行!
		mFile_Put(1, pGraph, gsiz);
		gRef = 0;
	}

}

Graphic_Live()
{
	if (Graphic_Map() < 2) {
		++gRef;
		return TRUE;
	}
	return FALSE;
}

Graphic_Unlive()
{
	--gRef;
	Graphic_Unmap();
}

void *
Graphic_Get(ofs, len)
{
	if (Graphic_Map() < 2) {
		++gRef;
		return pGraph + ofs;
	}
	return NULL;
}

#include "debug.h"

Graphic_Put(p)
	GraphInfo *p;
{
	ASSERT(gRef > 1);		// 侦测Bug
	--gRef;
	Graphic_Unmap();
}
