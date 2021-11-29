#ifndef __MYDRV3_H__
#define __MYDRV3_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>

static const unsigned int gpioA = 5;

void mydrv3_init_proc(void);
long mydrv3_ioctl(struct file *file, unsigned int cmd, void __user *arg);

void mydrv3_open_gpio(void);
void mydrv3_release_gpio(void);
void mydrv3_write_gpio_5(bool value);

#endif
