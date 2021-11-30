#ifndef __CMN3_H__
#define __CMN3_H__

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>

struct cmn_store{
       int code1;
	int code2;
	int count1;
	int count2;
};

void mydrv3_init_cmn(void);

#ifdef __CMN3_C__
          struct cmn_store cmn;
#else
   extern struct cmn_store cmn;
#endif

#endif
