#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char sig[] = "Special Sdcard for Sangu";
char dont_mark[] = "/dev/sda";
#define	MARK_POS	0
int main(int argc, char *argv[])
{
	FILE		*fp;
	if (argc != 2)
	{
		printf("Usage: markme <device>\n");
		return -1;
	}
	int len = strlen(dont_mark);
	
	if(strncmp(argv[1],dont_mark,len) == 0)
	{
		printf("don't mark %s\n",dont_mark);
		return -1;
	}
	fp = fopen(argv[1], "wb");
	if(!fp)
	{
		printf("open device error\n");
		return -1;
	}
	fseek(fp,MARK_POS,SEEK_SET);
	
	len = strlen(sig);
	if(fwrite(sig,1,len,fp) == len)
	{
		printf("write success\n");
	}
	else
		printf("write failed\n");
	fclose(fp);
	return 0;
}
