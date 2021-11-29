#ifndef __MYDRV3_H__
#define __MYDRV3_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

void mydrv3_init_proc(void);
long mydrv3_ioctl(struct file *file, unsigned int cmd, void __user *arg);

int mydrv3_init_gpio(void);
void mydrv3_exit_gpio(void);
void mydrv3_write_gpio_5(bool value);

#endif
