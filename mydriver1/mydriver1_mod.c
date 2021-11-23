
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

/*==============================================================================*/

#define  DEVICE_NAME "mydev"
#define  CLASS_NAME  "mydevclass"

dev_t my_devt;
static struct cdev my_dev;
static struct class*  my_dev_class  = NULL;
static struct device* my_device = NULL;

static const char g_s_Hello_World_string[] = "Hello world from kernel mode 22!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);

/*==============================================================================*/

static int mydriver1_open(struct inode *inode, struct file *file)
{
	pr_info("mydriver1_open\n");
	return 0;
}

/*==============================================================================*/

static int mydriver1_close(struct inode *inode, struct file *file)
{
	pr_info("mydriver1_close\n");
	return 0;
}

/*==============================================================================*/

static ssize_t mydriver1_read(
    struct file *file_ptr
    , char __user *user_buffer
    , size_t count
    , loff_t *position)
{
    printk( KERN_NOTICE "read:  count,position  %u, %i \n",
        (unsigned int)count,
        (int)*position);

    if( *position >= g_s_Hello_World_size )
        return 0;

    if( *position + count > g_s_Hello_World_size )
        count = g_s_Hello_World_size - *position;

    if( copy_to_user(user_buffer, g_s_Hello_World_string + *position, count) != 0 )
        return -EFAULT;

    *position += count;
    printk( KERN_NOTICE "read:  count  %u\n", (unsigned int)count);
    return count;
}

/*==============================================================================*/

static long mydriver1_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_info("mydriver1_ioctl cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

/*==============================================================================*/

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = mydriver1_read,
	.open = mydriver1_open,
	.release = mydriver1_close,
	.unlocked_ioctl = mydriver1_ioctl,
};

static int __init mydriver1_init(void)
{
    dev_t devtalloc;
	int major, minor;
	int ret;
	pr_info("mydriver1 init**************\n");

	/* Allocate dynamically device numbers */
	ret = alloc_chrdev_region(&devtalloc, 0, 1, DEVICE_NAME);
	if (ret < 0){
		pr_info("Unable to allocate major number \n");
		return ret;
	}
	major = MAJOR(devtalloc);
	minor = MINOR(devtalloc);
	my_devt = MKDEV(major,0);

	pr_info("Allocated major,minor %d %d\n", major, minor);

	/* Initialize the cdev structure and add it to the kernel space start */
	cdev_init(&my_dev, &my_fops);
	ret = cdev_add(&my_dev, my_devt, 1);
	if (ret < 0){
		unregister_chrdev_region(my_devt, 1);
		pr_info("Unable to add cdev\n");
		return ret;
	}

	/* Register the device class */
	my_dev_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(my_dev_class)){
		unregister_chrdev_region(my_devt, 1);
	    	pr_info("Failed to register device class\n");
	    	return PTR_ERR(my_dev_class);
	}
	pr_info("device class registered correctly\n");

	/* Create a device node */
	my_device = device_create(my_dev_class, NULL, my_devt, NULL, DEVICE_NAME);
	if (IS_ERR(my_device)){
	    class_destroy(my_dev_class);
	    unregister_chrdev_region(my_devt, 1);
	    pr_info("Failed to create the device\n");
	    return PTR_ERR(my_device);
	}
	pr_info("The device is created correctly\n");

	return 0;
}

/*==============================================================================*/
static void __exit mydriver1_exit(void)
{
	device_destroy(my_dev_class, my_devt);   // remove the device
	class_unregister(my_dev_class);        // unregister the device class
	class_destroy(my_dev_class);           // remove the device class
	unregister_chrdev_region(my_devt, 1);    // unregister the device numbers
	pr_info("mydriver1 exit\n");
}

/*==============================================================================*/
module_init(mydriver1_init);
module_exit(mydriver1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chrishoen");
MODULE_DESCRIPTION("mydriver1 description");

