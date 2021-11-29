#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>


#include "mydrv3.h"

static int mydrv3_open(struct inode *inode, struct file *file)
{
	pr_info("mydrv3: open\n");
	return 0;
}

static int mydrv3_close(struct inode *inode, struct file *file)
{
	pr_info("mydrv3: close\n");
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = mydrv3_open,
	.release = mydrv3_close,
	.unlocked_ioctl = mydrv3_ioctl,
};

/* declare & initialize struct miscdevice */
static struct miscdevice my_miscdevice = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "mydev3",
		.fops = &my_fops,
};

static int __init mydrv3_init(void)
{
	int ret_val;
	pr_info("mydrv3: init****************************************\n");

	if(!gpio_is_valid(gpioA)){
		pr_info("mydrv3: init: Invalid GPIO");
		return -ENODEV;
	}
	gpio_request(gpioA, "sysfs");
	gpio_direction_output(gpioA, 1);
	gpio_export(gpioA, 0);

       mydrv3_init_proc();
       mydrv3_open_gpio();

	ret_val = misc_register(&my_miscdevice);

	if (ret_val != 0) {
		pr_err("mydrv3: could not register the misc device mydev1");
		return ret_val;
	}

	pr_info("mydrv3: my_miscdevice.minor %i\n",my_miscdevice.minor);
	return 0;
}
static void __exit mydrv3_exit(void)
{
	pr_info("mydrv3: exit\n");

       mydrv3_release_gpio();

	gpio_unexport(gpioA);
	misc_deregister(&my_miscdevice);
}

module_init(mydrv3_init);
module_exit(mydrv3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chrishoen");
MODULE_DESCRIPTION("misc char driver with gpio");

