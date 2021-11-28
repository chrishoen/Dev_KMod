
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

static const unsigned int gpioA = 5;
static bool my_stateA;

static ssize_t mydrv2_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char *led_on = "on"; 
	char *led_off = "off"; 
	unsigned char myled_value[10]; 

	if(copy_from_user(myled_value, buf, count)){
	       pr_info("mydrv2: write error\n");
		return -EFAULT;
	}

	if(strncmp(led_on, myled_value, count) == 0)
	{
		my_stateA = 1;
		gpio_set_value(gpioA, my_stateA);
	}
	else if(strncmp(led_off, myled_value, count) == 0)
	{
		my_stateA = 0;
		gpio_set_value(gpioA, my_stateA);
	}
	else {
              pr_info("mydrv2: write error 102\n");
		return -EINVAL;
	}
       pr_info("mydrv2: write %d\n", my_stateA);

	return 0;
}

static int mydrv2_open(struct inode *inode, struct file *file)
{
	pr_info("mydrv2: mydrv2_open() is called.\n");
	return 0;
}

static int mydrv2_close(struct inode *inode, struct file *file)
{
	pr_info("mydrv2: mydrv2_close() is called.\n");
	return 0;
}

static long mydrv2_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_info("mydrv2: mydrv2_ioctl() is called. cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

static const struct file_operations mydrv2_fops = {
	.owner = THIS_MODULE,
	.open = mydrv2_open,
	.write = mydrv2_write,
	.release = mydrv2_close,
	.unlocked_ioctl = mydrv2_ioctl,
};

static struct miscdevice mydev = {
		.minor = MISC_DYNAMIC_MINOR,
		.name = "mydev",
		.fops = &mydrv2_fops,
};

static int mydrv2_probe(struct platform_device *pdev)
{
	int ret_val;
	pr_info("mydrv2: platform_probe enter\n");
	
	if(!gpio_is_valid(gpioA)){
		pr_info("mydrv2: Invalid led GPIO");
		return -ENODEV;
	}

	gpio_request(gpioA, "sysfs");
	gpio_direction_output(gpioA, 1);
	gpio_export(gpioA, 0);

	ret_val = misc_register(&mydev);
	if (ret_val != 0)
	{
		pr_err("mydrv2: register FAIL");
		return ret_val;
	}

	pr_info("mydrv2: probe %i\n",mydev.minor);
	return 0;
}

static int mydrv2_remove(struct platform_device *pdev)
{
	gpio_unexport(gpioA);
	misc_deregister(&mydev);

	pr_info("mydrv2: remove\n");
	return 0;
}

static const struct of_device_id my_of_ids[] = {
	{ .compatible = "arrow,intkey"},
	{},
};

MODULE_DEVICE_TABLE(of, my_of_ids);

static struct platform_driver my_platform_driver = {
	.probe = mydrv2_probe,
	.remove = mydrv2_remove,
	.driver = {
		.name = "intkey",
		.of_match_table = my_of_ids,
		.owner = THIS_MODULE,
	}
};

module_platform_driver(my_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("This is a platform driver that controls the LED using a sysfs and an user application. It also manages GPIO interrupts");



