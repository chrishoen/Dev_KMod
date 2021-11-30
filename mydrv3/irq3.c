
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include "cmn3.h"
#include "mydrv3.h"

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

static const char* MYDEV_NAME = "mydev3";

static bool prn = true;
static int gpio6 = 6;
static int irq = -1;
static int cookie = 0;
static int count = 0;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

int mydrv3_init_isr(void)
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
              IRQF_TRIGGER_RISING,
              MYDEV_NAME,
              &cookie);
	if (ret_val != 0) {
		pr_err("mydrv3: init: request_irq FAIL\n");
		return ret_val;
	}

	pr_info("mydrv3: mydrv3_init_isr  PASS\n");
	return 0;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_exit_isr(void)
{
       if (irq < 0){
       	pr_info("mydrv3: mydrv3_exit_isr  NOP\n");
              return;
       }

       free_irq(irq, &cookie);
	pr_info("mydrv3: mydrv3_exit_isr  PASS\n");
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

irqreturn_t mydrv3_isr(int irq, void *data)
{
       int* pdata = (int*)data;
       count++;
       cmn.count1++;
       (*pdata)++;
	if(prn) pr_info("mydrv3: interrupt received %d %d %d\n",
              *pdata, count, cmn.count1);
	return IRQ_HANDLED;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
