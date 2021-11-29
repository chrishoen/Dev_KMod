
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include "mydrv3.h"

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

static const char* MYDEV_NAME = "mydev3";

static bool prn = true;
static int gpio6 = 6;
static int irq = -1;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

int mydrv3_init_isr(struct miscdevice *device)
{
       int ret_val;

	if(!gpio_is_valid(gpio6)){
		pr_info("mydrv3: gpio_is_valid FAIL");
		return -ENODEV;
	}

	irq = gpio_to_irq(gpio6);
	if (irq < 0) {
		pr_info("mydrv3: gpio_to_irq FAIL");
		return irq;
	}
	pr_info("mydrv3: irq %d\n",irq);

       ret_val =  request_irq(
              irq,
              mydrv3_isr,
              IRQF_TRIGGER_FALLING,
              MYDEV_NAME,
              device);
	if (ret_val != 0) {
		pr_err("mydrv3: init: request_irq FAIL\n");
		return ret_val;
	}

	pr_info("mydrv3: mydrv3_init_isr  PASS");
	return 0;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_exit_isr(struct miscdevice *device)
{
       if (irq < 0){
       	pr_info("mydrv3: mydrv3_exit_isr  NOP");
              return;
       }

       free_irq(irq, device);
	pr_info("mydrv3: mydrv3_exit_isr  PASS");
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

irqreturn_t mydrv3_isr(int irq, void *data)
{
	if(prn) pr_info("mydrv3: interrupt received\n");
	return IRQ_HANDLED;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
