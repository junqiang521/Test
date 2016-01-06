#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/errno.h>
#include <linux/mman.h>
#include <linux/dma-mapping.h>
#include <linux/fs.h>

/**
 * @def PCI_CLASS_STORAGE_EXPRESS
 * Set to value matching with NVME HW
 */
#define PCI_CLASS_STORAGE_EXPRESS    0x010802

#define DRV_NAME                "snvme"
#define NVME_DEVICE_NAME        "nvme"

static struct class *class_nvme;
static int nvme_major;

#define BAR0_BAR1               0x0
#define BAR2_BAR3               0x2
#define BAR4_BAR5               0x4

#define SUCCESS              0

/* local functions static declarations */
static int __init snvme_init(void);
static void __exit snvme_exit(void);

static int snvme_probe(struct pci_dev *pdev, const struct pci_device_id *id);
static void snvme_remove(struct pci_dev *dev);
/*
static struct metrics_device_list *lock_device(struct inode *inode);
static void unlock_device(struct  metrics_device_list *pmetrics_device);
static struct metrics_device_list *find_device(struct inode *inode);
*/

int dnvme_open(struct inode *inode, struct file *filp);
int dnvme_release(struct inode *inode, struct file *filp);
int dnvme_mmap(struct file *filp, struct vm_area_struct *vma);
long dnvme_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param);
static struct pci_device_id snvme_ids[] = {
    { PCI_DEVICE_CLASS(PCI_CLASS_STORAGE_EXPRESS, 0xffffff) },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, snvme_ids);


static struct pci_driver snvme_driver = {
    .name     = DRV_NAME,
    .id_table = snvme_ids,
    .probe    = snvme_probe,
    .remove   = snvme_remove,
};

static const struct file_operations snvme_fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = snvme_ioctl,
    .open           = snvme_open,
    .release        = snvme_release,
    .mmap           = snvme_mmap,
};

static int __init snvme_init(void)
{
    int err = SUCCESS;
    printk(KERN_ALERT "Hello snvme_init...\n");
     /* Check if class_nvme creation has any issues */
    

    /* Get a dynamically alloc'd major number for this driver */
    nvme_major = register_chrdev(0, NVME_DEVICE_NAME, &snvme_fops);
    if (nvme_major < 0) {
        printk(KERN_ALERT "snvme char device driver registration fail\n");
        return -ENODEV;
    }

    class_nvme = class_create(THIS_MODULE, NVME_DEVICE_NAME);
    if (IS_ERR(class_nvme)) {
        printk(KERN_ALERT "NVMe class creation failed\n");
        err = PTR_ERR(class_nvme);
        goto unreg_chrdrv_fail_out;
    }

    /* Register this driver */
    err = pci_register_driver(&snvme_driver);
    if (err) {
        printk(KERN_ALERT "PCIe driver registration failed\n");
        goto class_create_fail_out;
    }
    return err;

class_create_fail_out:
    class_destroy(class_nvme);
unreg_chrdrv_fail_out:
    unregister_chrdev(nvme_major, NVME_DEVICE_NAME);
    return err;
}
static void __exit snvme_exit(void)
{
    printk(KERN_ALERT "Hello snvme_exit...\n");
    return ;
}
static int snvme_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    printk(KERN_ALERT "Hello snvme_probe...\n");
    return 0;
}
static void snvme_remove(struct pci_dev *dev)
{
    printk(KERN_ALERT "Hello snvme_remove...\n");
    return ;
}
int snvme_open(struct inode *inode, struct file *filp)
{

    printk(KERN_ALERT "Hello snvme_open...\n");
    return 0;
}
int snvme_release(struct inode *inode, struct file *filp)
{

    printk(KERN_ALERT "Hello snvme_release...\n");
    return 0;
}

int snvme_mmap(struct file *filp, struct vm_area_struct *vma)
{
    printk(KERN_ALERT "Hello snvme_mmap...\n");
    return 0;
}
long snvme_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param)
{
    printk(KERN_ALERT "Hello snvme_ioctl...\n");
    return 0;
}

MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:"DRV_NAME);
MODULE_AUTHOR("nvmecompliance@intel.com");
MODULE_DESCRIPTION("NVMe small kernel driver");
//MODULE_VERSION(DRIVER_VERSION_STR(DRIVER_VERSION));

module_init(snvme_init);
module_exit(snvme_exit);

