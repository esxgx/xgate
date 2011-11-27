#include "types.h"
#include <malloc.h>
#include <stdlib.h>

#define I_CHAP	0x1000

typedef struct tagGraphChap {
	int n;
	unsigned *page;
	struct tagGraphChap *forw;
} GraphChap;

GraphChap GrapBuk;

GraphChap *
GraphIdx_GetChapter(int n)
{
	GraphChap *p = &GrapBuk;
	while(p->forw) {
		if ((p->forw)->n == n)
			return p->forw;
		p = p->forw;
	}
	GraphChap *nju = (GraphChap *)malloc(sizeof(GraphChap));
	if (nju) {
		nju->forw = NULL;
		nju->n = n;
		nju->page = malloc(I_CHAP * sizeof(unsigned));
		UINT i = 0;
		while(i < I_CHAP)
			nju->page[i++] = (unsigned)-1;
		if (nju->page) {
			p->forw = nju;
			return nju;
		}
		free(nju);
	}
	return NULL;
}

GraphIdx_BurnChapter(int n)
{
	GraphChap *p = &GrapBuk, *f;
	while(f = p->forw) {
		if (f->n == n) {
			p->forw = f->forw;
			free((void *)f->page);
			free(f);
			return TRUE;
		}
		p = f;
	}
	return FALSE;
}

GraphIdx_BurnBook()
{
	GraphChap *p, *f;
	p = GrapBuk.forw;
	while(p) {
		f = p->forw;
		free((void *)p->page);
		free(p);
		p = f;
	}
	GrapBuk.forw = NULL;
}

#include "puk1/graph.h"

GraphIdx_Search(mapno, f)
	GraphChap *f;
{
	unsigned i, total;
	GraphInfo *ph;

	total = GraphInfo_Total();
	for(i=0; i<total; ++i) {
		ph = (GraphInfo *)GraphInfo_Get(i);
		if (ph) {
			int n = ph->MapNo;
			if ((!f) || f->n != n/I_CHAP)
				f = GraphIdx_GetChapter(n/I_CHAP);
//			if (f->page[n % I_CHAP] == (unsigned)-1)
				f->page[n % I_CHAP] = i;
			GraphInfo_Put(i, ph);
			if (n != mapno) continue;
		}
		break;
	}
	if (ph && i<total) return i;
	return -1;
}

GraphIdx_Get(mapno)
{
	GraphChap *ch = GraphIdx_GetChapter(mapno/I_CHAP);
	unsigned page = mapno%I_CHAP;
	if (ch->page[page] != (unsigned)-1)
		return ch->page[page];
	return GraphIdx_Search(mapno, ch);
}

