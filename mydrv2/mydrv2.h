#ifndef __MYDRV2_H__
#define __MYDRV2_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

void mydrv2_init_fileops(void);
ssize_t mydrv2_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
ssize_t mydrv2_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
long mydrv2_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static const unsigned int gpioA = 5;

#endif
