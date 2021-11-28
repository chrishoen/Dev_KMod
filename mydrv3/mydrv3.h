#ifndef __MYDRV2_H__
#define __MYDRV2_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

void mydrv3_init_fileops(void);
ssize_t mydrv3_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
ssize_t mydrv3_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
long mydrv3_ioctl(struct file *file, unsigned int cmd, void __user *arg);

static const unsigned int gpioA = 5;

#endif
