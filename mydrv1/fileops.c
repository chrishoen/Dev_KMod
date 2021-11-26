
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "mydrv1.h"

/*==============================================================================*/

static const char g_s_Hello_World_string[] = "Hello world from kernel mode 22!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);

/*==============================================================================*/

ssize_t mydrv1_read(
       struct file *file_ptr,
       char __user *user_buffer,
       size_t count,
       loff_t *position)
{
       printk( KERN_NOTICE "mydrv1: read  count,position  %u, %i \n",
       (unsigned int)count,
       (int)*position);

       if( *position >= g_s_Hello_World_size )
              return 0;

       if( *position + count > g_s_Hello_World_size )
              count = g_s_Hello_World_size - *position;

       if( copy_to_user(user_buffer, g_s_Hello_World_string + *position, count) != 0 )
              return -EFAULT;

       *position += count;
       printk( KERN_NOTICE "mydrv1: read  count  %u\n", (unsigned int)count);
       return count;
}

/*==============================================================================*/

long mydrv1_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
       pr_info("mydrv1: ioctl cmd = %d, arg = %ld\n", cmd, arg);
	return 0;
}

