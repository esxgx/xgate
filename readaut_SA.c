//
// automap.c
//
#include "dwango/automap.h"

#include "types.h"
#include "debug.h"

static AutoHeader *Automap;
static UINT aSiz, aRef;

void *
Automap_Get()
{
	if (!aRef) {
		Automap = (AutoHeader *)mFile_Get(5, 0, aSiz = mFile_Size(5));
		ASSERT(Automap->Magic == 0x2);
		aRef = 1;
	}
	++aRef;
	return Automap;
}

Automap_Put(p)
{
	if (--aRef <= 1) {
		mFile_Put(5, Automap, aSiz);
		aRef = 0;
	}
	return 1;
}

Automap_GetColor(n)
{
	BYTE *a = Automap_Get();
	BYTE s = a[sizeof(AutoHeader) + n];
	Automap_Put(a);
	return s;
}

