

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include "mydrv1.h"

static int mydrv1_open(struct inode *inode, struct file *file)
{
	pr_info("mydrv1: open\n");
	return 0;
}

static int mydrv1_close(struct inode *inode, struct file *file)
{
	pr_info("mydrv1: close\n");
	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = mydrv1_read,
	.open = mydrv1_open,
	.release = mydrv1_close,
	.unlocked_ioctl = mydrv1_ioctl,
};

/* declare & initialize struct miscdevice */
static struct miscdevice my_miscdevice = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "mydev1",
		.fops = &my_fops,
};

static int __init mydrv1_init(void)
{
	int ret_val;
	pr_info("mydrv1: init\n");

	/* Register the device with the Kernel */
	ret_val = misc_register(&my_miscdevice);

	if (ret_val != 0) {
		pr_err("mydrv1: could not register the misc device mydev1");
		return ret_val;
	}

	pr_info("mydrv1: my_miscdevice.minor %i\n",my_miscdevice.minor);
	return 0;
}
static void __exit mydrv1_exit(void)
{
	pr_info("mydrv1: exit\n");

	/* unregister the device with the Kernel */
	misc_deregister(&my_miscdevice);
}

module_init(mydrv1_init);
module_exit(mydrv1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chrishoen");
MODULE_DESCRIPTION("misc char driver");

