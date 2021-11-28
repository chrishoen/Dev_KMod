
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#include "mydrv2.h"

/*==============================================================================*/

static bool my_stateA;

/*==============================================================================*/

void mydrv2_init_fileops(void)
{
       my_stateA = false;
	gpio_set_value(gpioA, my_stateA);
}

/*==============================================================================*/

ssize_t mydrv2_read(struct file *file, char __user *userbuffer, size_t count, loff_t *ppos)
{
       char buffer[10];
       char size = 3;

       pr_info("mydrv2: read  count,ppos  %u, %i \n", (unsigned int)count, (int)*ppos);

       if( *ppos >= size ){
              pr_info("mydrv2: read  count  empty\n");
              return 0;
       }

       if (my_stateA)
              strcpy(buffer, "1\n");
       else
              strcpy(buffer, "0\n");

       if( *ppos + count > size )
              count = size - *ppos;
       
       if( copy_to_user((void*)userbuffer, (const void*)buffer + *ppos, count) != 0 )
              return -EFAULT;

       *ppos += count;
       pr_info("mydrv2: read  count  %u\n", (unsigned int)count);
       return count;
}

/*==============================================================================*/

ssize_t mydrv2_write(struct file *file, const char __user *userbuffer, size_t count, loff_t *ppos)
{
       char buffer[10];
       char size = 2;

       // pr_info( "mydrv2: write  count,ppos  %u, %i \n", (unsigned int)count, (int)*ppos);

       if( count != size ){
              pr_err("mydrv2: write FAIL 101 %d\n", count);
              return -EINVAL;
       }

       if( copy_from_user((void*)buffer, (void*)userbuffer, count) != 0 )
              return -EFAULT;

	if(buffer[0] == '1')
	{
		my_stateA = 1;
		gpio_set_value(gpioA, my_stateA);
	}
	else if(buffer[0] == '0')
	{
		my_stateA = 0;
		gpio_set_value(gpioA, my_stateA);
	}
	else {
              pr_info("mydrv2: write error 102\n");
		return -EINVAL;
	}
       pr_info("mydrv2: write %d\n", my_stateA);

       if (buffer[0] == '1')
       pr_info("mydrv2: write count  %u\n", (unsigned int)count);
       return count;
}

/*==============================================================================*/

long mydrv2_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
       pr_info("mydrv2: ioctl cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

