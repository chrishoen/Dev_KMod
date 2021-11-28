#ifndef __MYDRV1_H__
#define __MYDRV1_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

void mydrv1_init_fileops(void);
ssize_t mydrv1_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
ssize_t mydrv1_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
long mydrv1_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
