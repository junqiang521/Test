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
#include <asm/uaccess.h>

/**
 * @def PCI_CLASS_STORAGE_EXPRESS
 * Set to value matching with NVME HW
 */
#include "dnvme_ioctls.h"
#include "dnvme_interface.h"

#define PCI_CLASS_STORAGE_EXPRESS    0x010802

#define DRV_NAME                "snvme"
#define NVME_DEVICE_NAME        "nvme"
//#define SNVME_ERR_CHK  10
//#define NVME_IOCTL_ERR_CHK _IOWR('N', SNVME_ERR_CHK, uint16_t)


static struct class *class_nvme;
static int nvme_major;
static struct device *admin_snvme;
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

int snvme_open(struct inode *inode, struct file *filp);
int snvme_release(struct inode *inode, struct file *filp);
int snvme_mmap(struct file *filp, struct vm_area_struct *vma);
long snvme_ioctl(struct file *filp, unsigned int ioctl_num,unsigned long ioctl_param);
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
    dev_t devno;

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

    devno = MKDEV(nvme_major, 0);
    admin_snvme = device_create(class_nvme, 0, devno, 0, NVME_DEVICE_NAME);
    if (IS_ERR(admin_snvme)) {
        printk(KERN_ALERT "NVMe device creation failed\n");
        err = PTR_ERR(admin_snvme);
        goto class_create_fail_out;
    }
    printk(KERN_ALERT "NVMe device creation SUCCESS\n");
    /* Register this driver */
    err = pci_register_driver(&snvme_driver);
    if (err) {
        printk(KERN_ALERT "PCIe driver registration failed\n");
        goto class_create_fail_out;
    }
    return err;

class_create_fail_out:
    device_del(admin_snvme);
    class_destroy(class_nvme);
unreg_chrdrv_fail_out:
    unregister_chrdev(nvme_major, NVME_DEVICE_NAME);
    return err;
}
static void __exit snvme_exit(void)
{
    device_del(admin_snvme);
    pci_unregister_driver(&snvme_driver);
    class_destroy(class_nvme);
    unregister_chrdev(nvme_major, NVME_DEVICE_NAME);
    printk(KERN_ALERT "Hello snvme_exit...\n");
    return ;
}
static int snvme_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    int err = -EINVAL;

   
    
    void __iomem *bar0 = NULL;
    //void __iomem *bar1 = NULL;
    //void __iomem *bar2 = NULL;
    static int nvme_minor = 0;
    //dev_t devno = MKDEV(nvme_major, nvme_minor);
    //struct metrics_device_list *pmetrics_device = NULL;
    int bars = 0;
    static u32 ID = 0; 
	static struct msix_entry admin_sqe;

    printk(KERN_ALERT "PCI snvme_probe...\n");
    pci_read_config_dword(pdev, 0x00, &ID);
    printk(KERN_ALERT "pci_read_config_dword ID:0x%x",ID);
    /* Get the bitmask value of the BAR's supported by device */
    bars = pci_select_bars(pdev, IORESOURCE_MEM);
    
    /* Map BAR0 & BAR1 (BAR0 for 64-bit); ctrlr register memory mapped  */
    if (request_mem_region(pci_resource_start(pdev, BAR0_BAR1),
        pci_resource_len(pdev, BAR0_BAR1), DRV_NAME) == NULL) {
        printk(KERN_ALERT "BAR0 memory already in use");
        goto fail_out;
    }

    bar0 = ioremap_nocache(pci_resource_start(pdev, BAR0_BAR1),
        pci_resource_len(pdev, BAR0_BAR1));
    if (bar0 == NULL) {
        printk(KERN_ALERT "Mapping BAR0 mem map'd registers failed");
        goto remap_fail_out;
    }
    printk(KERN_ALERT "CAP_LOW: 0x%x,CAP_HI: 0x%x,\n",readl(bar0 +0x00),readl(bar0 +0x04));
    printk(KERN_ALERT "Mapping BAR0 mem map'd registers SUCCESS,VS: 0x%x,CC: 0x%x,CSTS: 0x%x\n",readl(bar0 +0x08),readl(bar0 +0x14),readl(bar0 +0x1c));
    pci_set_master(pdev);

    if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64)) &&
        dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32)))
        goto remap_fail_out;
    /*
     * Some devices don't advertse INTx interrupts, pre-enable a single
     * MSIX vec for setup. We'll adjust this later.
     */
    
    memset(&admin_sqe,0,sizeof(admin_sqe));
    if (!pdev->irq) {
        err = pci_enable_msix(pdev, &admin_sqe, 1);
        if (err < 0)
            goto remap_fail_out;
    }
    err = pci_enable_device(pdev);
    if (err < 0) {
        printk(KERN_ALERT "Enabling the PCIe device has failed: 0x%04X", err);
        goto remap_fail_out;
    }
    return 0;
remap_fail_out:
    if (bar0 != NULL) {
        iounmap(bar0);
        release_mem_region(pci_resource_start(pdev, BAR0_BAR1),
            pci_resource_len(pdev, BAR0_BAR1));
    }
fail_out:
    printk(KERN_ALERT "PCI snvme_probe OK...\n");
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
    
    int err = -EINVAL;
    //struct metrics_device_list *pmetrics_device;
    struct nvme_create_admn_q *create_admn_q;
    struct inode *inode = inode = filp->f_path.dentry->d_inode;

	//printk(KERN_ALERT "Hello snvme_ioctl...,NVME_IOCTL_ERR_CHK:\n",ioctl_num,NVME_IOCTL_ERR_CHK);

    switch (ioctl_num) {

	    case NVME_IOCTL_ERR_CHK:
	        //LOG_DBG("NVME_IOCTL_ERR_CHK");
	        //err = device_status_chk(pmetrics_device, (int *)ioctl_param);
	        printk(KERN_ALERT "NVME_IOCTL_ERR_CHK...\n");
	        break;

	    case NVME_IOCTL_CREATE_ADMN_Q:

	        printk(KERN_ALERT " NVME_IOCTL_CREATE_ADMN_Q");
	        /* Allocating memory for user struct in kernel space */
	        create_admn_q = kmalloc(sizeof(struct nvme_create_admn_q), GFP_KERNEL);
	        if (create_admn_q == NULL) {
	            printk(KERN_ALERT "Unable to alloc kernel memory to copy user data");
	            err = -ENOMEM;
	            break;
	        }
	        if (copy_from_user(create_admn_q, (void *)ioctl_param,
	            sizeof(struct nvme_create_admn_q))) {

	            printk(KERN_ALERT "Unable to copy from user space");
	            kfree(create_admn_q);
	            err = -EFAULT;
	            break;
	        }

	        if (create_admn_q->type == ADMIN_SQ) {
	            printk(KERN_ALERT "Create ASQ");
	            //err = driver_create_asq(create_admn_q, pmetrics_device);
	        } else if (create_admn_q->type == ADMIN_CQ) {
	            printk(KERN_ALERT "Create ACQ");
	            //err = driver_create_acq(create_admn_q, pmetrics_device);
	        } else {
	            printk(KERN_ALERT "Unknown Q type specified");
	            err = -EINVAL;
	        }
	        kfree(create_admn_q);
        break;

	    default:
	    	printk(KERN_ALERT "ioctl_num not found...\n");

	}

    
    return 0;
}

MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:"DRV_NAME);
MODULE_AUTHOR("nvmecompliance@intel.com");
MODULE_DESCRIPTION("NVMe small kernel driver");
//MODULE_VERSION(DRIVER_VERSION_STR(DRIVER_VERSION));

module_init(snvme_init);
module_exit(snvme_exit);

