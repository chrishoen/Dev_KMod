#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include "mydrv3.h"

static char *MYDRV_NAME = "MYDRV";
#if 0
/* interrupt handler */
static irqreturn_t mydrv3_isr(int irq, void *data)
{
	pr_info("mydrv3: interrupt received\n");
	return IRQ_HANDLED;
}
#endif
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

static int irq;

static int __init mydrv3_init(void)
{
	int ret_val;

	pr_info("mydrv3: init****************************************\n");

	ret_val = mydrv3_init_gpio();
	if (ret_val != 0) {
		pr_err("mydrv3: init: gpio FAIL\n");
		return ret_val;
	}

       mydrv3_init_proc();

	ret_val = misc_register(&my_miscdevice);
	if (ret_val != 0) {
		pr_err("mydrv3: init: could not register the misc device\n");
		return ret_val;
	}
	pr_info("mydrv3: my_miscdevice.minor %i\n",my_miscdevice.minor);

	irq = gpio_to_irq(6);
	if (irq < 0) {
		pr_err("mydrv3: init: gpio_to_irq FAIL\n");
		return irq;
	}
	pr_info("mydrv3: irq %d\n",irq);

       ret_val =  request_irq(irq, mydrv3_isr, 
                     IRQF_TRIGGER_FALLING, MYDRV_NAME, &my_miscdevice);
	if (ret_val != 0) {
		pr_err("mydrv3: init: request irq FAIL\n");
		return ret_val;
	}
	return 0;
}

static void __exit mydrv3_exit(void)
{
	pr_info("mydrv3: exit\n");

       free_irq(irq, &my_miscdevice);

       mydrv3_exit_gpio();

	misc_deregister(&my_miscdevice);
}

module_init(mydrv3_init);
module_exit(mydrv3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chrishoen");
MODULE_DESCRIPTION("misc char driver with gpio");

