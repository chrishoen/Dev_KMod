
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#include "mydrv3.h"

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

/* GPIO controller: FE200000 */
#define BCM2710_PERI_BASE		0xfe000000
#define GPIO_BASE			BCM2710_PERI_BASE + 0x200000
#define GPSET0   	              GPIO_BASE + 0x1C
#define GPCLR0 		       GPIO_BASE + 0x28

/* used to set and clear the bit */
#define GPIO_5		5
#define GPIO_5_BIT    	1 << (GPIO_5 % 32)

/* virtual addresses */
static void __iomem *GPSET0_V;
static void __iomem *GPCLR0_V;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

int mydrv3_init_gpio(void)
{
       /* sysfs gpio 5 */
	if(!gpio_is_valid(GPIO_5)){
		pr_info("mydrv3: mydrv3_init_gpio FAIL");
		return -ENODEV;
	}

       /* sysfs gpio 5 as output */
	gpio_request(GPIO_5, "sysfs");
	gpio_direction_output(GPIO_5, 0);
	gpio_export(GPIO_5, 0);

       /* map physical addresses */
	GPSET0_V =  ioremap(GPSET0, sizeof(u32));
	GPCLR0_V =  ioremap(GPCLR0, sizeof(u32));

       /* clear gpio 5 */
	iowrite32(GPIO_5_BIT, GPCLR0_V);

       return 0;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_exit_gpio(void)
{
       /* sysfs gpio 5 */
	gpio_direction_input(GPIO_5);
	gpio_unexport(GPIO_5);

       /* unmap physical addresses */
	iounmap(GPSET0_V);
	iounmap(GPCLR0_V);
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_write_gpio_5(bool value)
{
       /* set or clear gpio 5*/
	if(value) {
		iowrite32(GPIO_5_BIT, GPSET0_V);
	}
	else{
		iowrite32(GPIO_5_BIT, GPCLR0_V);
	}
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
