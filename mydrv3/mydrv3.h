#ifndef __MYDRV3_H__
#define __MYDRV3_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

void mydrv3_init_proc(void);
long mydrv3_ioctl(struct file *file, unsigned int cmd, void __user *arg);

static const unsigned int gpioA = 5;

#endif
