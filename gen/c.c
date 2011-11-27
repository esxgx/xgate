#include <stdio.h>


int main()
{
	char buf[256];
	memset(buf,0, 256);
	rt_1(buf);
	int i;
	for(i=0; i<256; i+=4)
		printf("0x%06X\n", *(unsigned long *)(buf+i) & 0xFFFFFF);

	return 0;
}

