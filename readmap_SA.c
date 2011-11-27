#include <stdlib.h>
#include <malloc.h>
#include "types.h"

// 石器、魔力服务器地图档 魔数
#define MAP_MAGIC  "LS2MAP"

__MAP_CheckMagic(char *buf)
{
	char *r = MAP_MAGIC;
	while(*r) {
		if (*r != *buf) break;
		++r; ++buf;
	}
	return !(*r);
}

#include <fcntl.h>

// 是否为SA/CG地图文件
static BOOL MAP_IsMapFile(char* filename)
{
	int fd,ret;
	char magic[sizeof(MAP_MAGIC)];

	ret = FALSE;

	fd = open(filename, O_RDONLY);
	if (fd >= 0) {
		if (read(fd, &magic, sizeof(MAP_MAGIC)) == sizeof(MAP_MAGIC))
			ret = __MAP_CheckMagic(magic);
		close(fd);
	}
	return ret;
}

MAP_getImageInt(imagenumber, element)
{	

}

BOOL IsValidImagenumber(int imagenumber)
{
}

//
// 0x00		6	'LS2MAP'
// 0x06		2	地图ID 或 0xFFFF
// 0x08		32	地图名称
// 0x28		2/4	地图长/宽
// 当0x06为0xFFFF时, 有如下扩展数据:
// 0x3C		4	扩展数据大小(包括自己, 即下面的是-4)
// 0x40		4	新地图ID
// 0x44		4	未知数据1(是一个地图图像编号)
// 0x48		4	未知数据2(是一个地图图像编号)
//
// TILE层
// OBJ层
// 当flags不为0或4时, 有如下的数据:
// 0x00		2	Dungeon CfgId
// 0x02		2	Dungeon Seqno
// 当flags为1时, 有如下一层:
// xsiz * ysiz * 1
//
#include "puk1/readmap.h"

#define __S(x,y,z,a,b)	do{(x) = ntoh##y(*((z *)a));(a)+=(b);}while(0)

#include <string.h>

#ifndef ntohs
#include <netinet/in.h>
#endif

BOOL MAP_readMapOne(MAP_Map *map, char *filename)
{
	int msiz;
	char lb[24];

	int n = mFile_MapFileAdd(filename, 0);
	if (n >= 0) {
		char *buf = (char *)mFile_Get(n, 0, msiz = mFile_Size(n));
		if (buf != NULL) {
			char *p = buf;
			if (__MAP_CheckMagic(p)) {
				p += sizeof(MAP_MAGIC) - 1;
	
				__S(map->id, s, WORD, p, 2);
	
				strcpy(map->string, p); p += 32;
	
				__S(map->xsiz, s, short, p, 2);
				__S(map->ysiz, s, short, p, 2);
	
				map->v2map = (map->id == (WORD)-1);
				if (map->v2map) {	/* 新格式 */
					long nsiz;
					__S(nsiz, l, long, p, 4); nsiz -= 4;
					if (nsiz >= 0x4)
						__S(map->id, l, DWORD, p, 4);
					if (nsiz >= 0x8)
						__S(map->unkrown[0], l, DWORD, p, 4);
					if (nsiz >= 0xC)
						__S(map->unkrown[1], l, DWORD, p, 4);
				}
				int lyr, sy, sx;
				for(lyr = 0; lyr < 2; ++lyr) {
					map->layer[lyr] = (WORD **)malloc(sizeof(WORD *) * map->ysiz);
					for(sy = 0; sy < map->ysiz; ++sy) {
						map->layer[lyr][sy] = (WORD *)malloc(sizeof(WORD) * map->xsiz);
						for(sx = 0; sx < map->xsiz; ++sx)
							__S(map->layer[lyr][sy][sx], s, WORD, p, 2);
					}
				}
			}
			mFile_Put(n, buf, msiz);
		}
		mFile_MapFileRemove(n);
		return TRUE;
	}
	return FALSE;
}
