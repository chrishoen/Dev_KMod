#ifndef __MYDRV1_H__
#define __MYDRV1_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

ssize_t mydrv1_read(
       struct file *file_ptr, 
	char __user *user_buffer,
       size_t count,
       loff_t *position);

long mydrv1_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
