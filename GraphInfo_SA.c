#include "puk1/graph.h"
#include "types.h"

static unsigned iRef, isiz;
static GraphInfo *GrpInfo;

__GraphInfo_Size()
{
	if (!iRef) {
		isiz = mFile_Size(0);
		iRef = 1;
	}
	return isiz;
}

GraphInfo_Map()
{
	if (iRef <= 1) {
		GrpInfo = (GraphInfo *)mFile_Get(0, 0, __GraphInfo_Size());
		if (GrpInfo != NULL) {
			iRef = 2;	/* isiz = 1, malloc = 2 */
			return TRUE;
		}
		return 2;
	}
	return FALSE;
}

GraphInfo_Live()
{
	if (GraphInfo_Map() < 2) {
		++iRef;
		return TRUE;
	}
	return FALSE;
}

GraphInfo_Unmap()
{
	if (iRef == 2) {
		mFile_Put(0, GrpInfo, __GraphInfo_Size());
		iRef = 1;
	}
}

#include "debug.h"

GraphInfo_UnLive()
{
	--iRef;
	GraphInfo_Unmap();
}

GraphInfo *GraphInfo_Get(idx)
{
	if (GraphInfo_Map() < 2) {
//		if (GrpInfo[idx].Index == idx) {
			++iRef;
			return &GrpInfo[idx];
//		}
	}
	return NULL;
}

GraphInfo_Put(idx, p)
	GraphInfo *p;
{
	--iRef;
	GraphInfo_Unmap();
	return 1;
}

#include <stdio.h>

GraphInfo_Total()
{
	return __GraphInfo_Size() / sizeof(GraphInfo);
}

