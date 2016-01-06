
#include <linux/kernel.h>  
#include <linux/fs.h>  
#include <linux/init.h>  
#include <linux/delay.h>  
#include <asm/uaccess.h>  
#include <asm/irq.h>  
#include <asm/io.h>  
#include <linux/module.h>  
#include <linux/device.h>     //class_create  
 
int major;
static struct class *firstdrv_class;  
static struct device *firstdrv_device; 

static int first_drv_open(struct inode * inode, struct file * filp)  
{  
    printk("first_drv_open\n");  
    return 0;  
}

static int first_drv_write(struct file * file, const char __user * buffer, size_t count, loff_t * ppos)  
{  
    int value = 0;
	
	copy_from_user(&value,buffer,2);
	printk("first_drv_write: count:%d, value:%d \n",(int)count,(int)value);
	
    return 0;  
}
/* File operations struct for character device */  
static const struct file_operations my_drv_fops = {
	.owner = THIS_MODULE,  
	.open  = first_drv_open,  
	.write = first_drv_write,  
}; 

static int my_drv_init(void)
{
	/* 主设备号设置为0表示由系统自动分配主设备号 */  
	major = register_chrdev(0, "my_drv", &my_drv_fops);  
  
	/* 创建firstdrv类 */  
	firstdrv_class = class_create(THIS_MODULE, "firstdrv");  
  
	/* 在firstdrv类下创建xxx设备，供应用程序打开设备*/  
	firstdrv_device = device_create(firstdrv_class, NULL, MKDEV(major, 0), NULL, "my_drv");  
	return 0;

}
static void my_drv_exit(void)
{
	unregister_chrdev(major, "my_drv");  
	printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(my_drv_init);
module_exit(my_drv_exit);
MODULE_AUTHOR("JEREMY"); 
MODULE_DESCRIPTION("Just for Study");  
MODULE_LICENSE("Dual BSD/GPL");