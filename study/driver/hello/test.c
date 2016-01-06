#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)

{
	int fd;
	int val = 0x45;
	fd = open("/dev/my_drv",O_RDWR);
	if(fd < 0)
	{
		printf("open error fd:%d\n",fd);
	}
	
	write(fd,&val,16);
	
	return 0;
}
