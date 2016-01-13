#define NVME_DEVICE_NAME        "/dev/nvme"
#include <stdio.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/fs.h>
#include "dnvme_ioctls.h"
#include "dnvme_interface.h"

//#define SNVME_ERR_CHK  10
//#define NVME_IOCTL_ERR_CHK _IOWR('N',SNVME_ERR_CHK,int)

void main()
{
	int test_data = 0;
	struct nvme_create_admn_q cadq;
	int fd = open(NVME_DEVICE_NAME, 0);
	if (fd)
	{
		printf("can not open file:%d\n",fd);
	}

	//int ret = ioctl(fd, NVME_IOCTL_ERR_CHK, test_data);
	cadq.type = ADMIN_SQ;
	cadq.element = 64;
	int ret = ioctl(fd, NVME_IOCTL_CREATE_ADMN_Q, &cadq);
	printf("can not open file:%ld\n",NVME_IOCTL_ERR_CHK);

}