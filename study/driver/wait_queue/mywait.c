
#include <linux/kernel.h>  
#include <linux/fs.h>  
#include <linux/init.h>  
#include <linux/delay.h>  
#include <asm/uaccess.h>  
#include <asm/irq.h>  
#include <asm/io.h>  
#include <linux/module.h>  
#include <linux/device.h>     //class_create  
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/kernel.h>

static DECLARE_WAIT_QUEUE_HEAD(myqueue); 

int hello(void)
{
	DECLARE_WAITQUEUE(wait,current);  
    daemonize("hello"); 
    add_wait_queue(&myqueue,&wait);
    printk("hello\n");
    set_current_state(TASK_INTERRUPTIBLE);
    schedule();
    remove_wait_queue(&myqueue,&wait);
    return 0;
}

static int my_wait_init(void)
{
     int res = 0;
     printk(KERN_ALERT "Hello ...\n");
     kthread_run(hello,NULL,CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD );
     
     return res;

}
static void my_wait_exit(void)
{
	wake_up(&myqueue);
    printk(KERN_ALERT "Bye ...\n");

	printk("Goodbye, cruel world\n");
}
module_init(my_wait_init);
module_exit(my_wait_exit);
MODULE_AUTHOR("JEREMY"); 
MODULE_DESCRIPTION("Just for Study");  
MODULE_LICENSE("Dual BSD/GPL");