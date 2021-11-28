#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>


#include "mydrv2.h"

static int mydrv2_open(struct inode *inode, struct file *file)
{
	pr_info("mydrv2: open\n");
	return 0;
}

static int mydrv2_close(struct inode *inode, struct file *file)
{
	pr_info("mydrv2: close\n");
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = mydrv2_read,
	.write = mydrv2_write,
	.open = mydrv2_open,
	.release = mydrv2_close,
	.unlocked_ioctl = mydrv2_ioctl,
};

/* declare & initialize struct miscdevice */
static struct miscdevice my_miscdevice = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "mydev2",
		.fops = &my_fops,
};

static int __init mydrv2_init(void)
{
	int ret_val;
	pr_info("mydrv2: init****************************************\n");

	if(!gpio_is_valid(gpioA)){
		pr_info("mydrv2: init: Invalid GPIO");
		return -ENODEV;
	}
	gpio_request(gpioA, "sysfs");
	gpio_direction_output(gpioA, 1);
	gpio_export(gpioA, 0);

       mydrv2_init_fileops();

	ret_val = misc_register(&my_miscdevice);

	if (ret_val != 0) {
		pr_err("mydrv2: could not register the misc device mydev1");
		return ret_val;
	}

	pr_info("mydrv2: my_miscdevice.minor %i\n",my_miscdevice.minor);
	return 0;
}
static void __exit mydrv2_exit(void)
{
	pr_info("mydrv2: exit\n");

	gpio_unexport(gpioA);
	misc_deregister(&my_miscdevice);
}

module_init(mydrv2_init);
module_exit(mydrv2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chrishoen");
MODULE_DESCRIPTION("misc char driver with gpio");

