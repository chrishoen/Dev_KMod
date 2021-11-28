
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "mydrv1.h"

/*==============================================================================*/

static const ssize_t my_buffer_maxsize = 256;
static char my_buffer[256];
static ssize_t my_buffer_size = 0;

/*==============================================================================*/

void mydrv1_init_fileops(void)
{
       strcpy(my_buffer,"mydrv1 file_buffer **************************\n");
       my_buffer_size = strlen(my_buffer);
}

/*==============================================================================*/

ssize_t mydrv1_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
       pr_info("mydrv1: read  count,ppos  %u, %i \n", (unsigned int)count, (int)*ppos);

       if( *ppos >= my_buffer_size ){
              pr_info("mydrv1: read  count  empty\n");
              return 0;
       }

       if( *ppos + count > my_buffer_size )
              count = my_buffer_size - *ppos;

       if( copy_to_user((void*)buf, (const void*)my_buffer + *ppos, count) != 0 )
              return -EFAULT;

       *ppos += count;
       pr_info("mydrv1: read  count  %u\n", (unsigned int)count);
       return count;
}

/*==============================================================================*/

ssize_t mydrv1_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
       pr_info( "mydrv1: write  count,ppos  %u, %i \n", (unsigned int)count, (int)*ppos);

       if( count >= my_buffer_maxsize ){
              pr_err("mydrv1: write FAIL 101\n");
              return -EINVAL;
       }

       if( copy_from_user((void*)my_buffer, (void*)buf, count) != 0 )
              return -EFAULT;

       my_buffer_size = count;

       pr_info("mydrv1: write count  %u\n", (unsigned int)count);
       return count;
}

/*==============================================================================*/

long mydrv1_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
       pr_info("mydrv1: ioctl cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

