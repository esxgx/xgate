#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct tagMapFile {
	char *pName;	// 文件名
	unsigned pMode;	// 访问模式
} MapFile;

static MapFile pMpf[64] = {
#define RW	O_RDWR
#define R	O_RDONLY
#define W	O_WRONlY
#include "files.h"
#undef	RW
#undef	R
#undef	W
};

#define nMapFile	(sizeof(pMpf) / sizeof(MapFile))

static char pId[nMapFile];
static unsigned pRef[nMapFile];

mFile_MapFileAdd(pName, pMode)
	char *pName;
{
	switch(pMode) {
		case 1:		/* 写入模式 */
			pMode = O_RDWR | O_CREAT;
			break;
		case 2:		/* 只写不读模式 */
			pMode = O_WRONLY | O_CREAT;
			break;
		default:	/* 只读模式 */
			pMode = O_RDONLY;
			break;
	}
	int n = 0;
	while(n < nMapFile) {
		if (pMpf[n].pName == NULL) {
			pMpf[n].pName = pName;
			pMpf[n].pMode = pMode;
			pRef[n] = 0;
			return n;
		}
		n++;
	}
	return -1;
}


mFile_MapFileRemove(n)
{
	pMpf[n].pName = NULL;
	return 1;
}

#include "debug.h"

static
__mFile_Open(n)
{
	if (pRef[n] <= 0) {
		assert(pMpf[n].pName != NULL);

		pId[n] = open(pMpf[n].pName, pMpf[n].pMode);
		if (pId[n] < 0) return 2;

		pRef[n] = 1;
		return 1;
	}
	return 0;
}

static
__mFile_Close(n)
{
	if (pRef[n] < 1) fprintf(stderr, "%d %d\n", n, pRef[n]);
	ASSERT(pRef[n] >= 1);

	if (pRef[n] <= 1) {
		close(pId[n]);
		pRef[n] = 0;
		return 1;
	}
	return 0;
}

static
void *
__mFile_Map(n, start, len)
{
	int prot;
	switch(pMpf[n].pMode) {
		case O_RDONLY:
			prot = PROT_READ;
			break;
		case O_WRONLY:
			prot = PROT_WRITE;
			break;
		default:
			prot = PROT_READ | PROT_WRITE;
	}

	void *p = mmap(NULL, len, prot, MAP_SHARED, pId[n], start);

	++pRef[n];
	return p;
}

static
__mFile_Unmap(n, p, len)
	void *p;
{
	if (!munmap(p, len)) {
		--pRef[n];
		return 1;
	}
	return 0;
}

void *
mFile_Get(n, start, len)
{
	if (__mFile_Open(n) < 2)
		return __mFile_Map(n, start, len);
	return NULL;
}

mFile_Put(n, p, len)
	void *p;
{
	if (__mFile_Unmap(n,p,len)) {
		if (__mFile_Close(n) < 2)
			return 1;
	}
	return 0;
}

mFile_Size(n)
{
	if (__mFile_Open(n) < 2) {
		int c = lseek(pId[n], 0, 2);
		lseek(pId[n], 0, 1);
		if (__mFile_Close(n) < 2) return c;
	}
	return -1;
}

