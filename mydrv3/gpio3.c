
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/uaccess.h>

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

/* GPIO controller: FE200000 */
#define BCM2710_PERI_BASE		0xfe000000
#define GPIO_BASE			BCM2710_PERI_BASE + 0x200000
#define GPFSEL0                    GPIO_BASE + 0x00
#define GPSET0   	              GPIO_BASE + 0x1C
#define GPCLR0 		       GPIO_BASE + 0x28

#define GPIO_5			5

/* to set and clear the bit */
#define GPIO_5_BIT    	1 << (GPIO_5 % 32)

/* function select for output*/
#define GPIO_5_FUNC	       0b001 << ((GPIO_5 % 10) * 3)

/* function select mask */
#define FSEL_5_MASK 	       0b111 << ((GPIO_5 % 10) * 3)

/* Declare __iomem pointers that will keep virtual addresses */
static void __iomem *GPFSEL0_V;
static void __iomem *GPSET0_V;
static void __iomem *GPCLR0_V;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_open_gpio(void)
{
	u32 GPFSEL_read, GPFSEL_write;
	pr_info("demo_init enter\n");


       /* map physical addresses */
	GPFSEL0_V = ioremap(GPFSEL0, sizeof(u32));
	GPSET0_V =  ioremap(GPSET0, sizeof(u32));
	GPCLR0_V =  ioremap(GPCLR0, sizeof(u32));

       /* read current function select register */
	GPFSEL_read = ioread32(GPFSEL0_V);

       /* do a masked or with the function bits for gpio 5*/
	GPFSEL_write = (GPFSEL_read & ~FSEL_5_MASK) | GPIO_5_FUNC;

       /* write the new function select register */
	iowrite32(GPFSEL_write, GPFSEL0_V);

       /* clear gpio 5 */
	iowrite32(GPIO_5_BIT, GPCLR0_V);
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_release_gpio(void)
{
       /* clear gpio 5 */
	iowrite32(GPIO_5_BIT, GPCLR0_V);

       /* unmap physical addresses */
	iounmap(GPFSEL0_V);
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

