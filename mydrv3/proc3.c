
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#include "cmn3.h"
#include "mydrv3.h"

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

static bool prn = false;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_init_proc(void)
{
	pr_info("mydrv3: mydrv3_init_proc PASS\n");
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void mydrv3_exit_proc(void)
{
	pr_info("mydrv3: mydrv3_exit_proc PASS\n");
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

long mydrv3_ioctl(struct file *file, unsigned int cmd, void __user *arg)
{
       int value = 0;

       if (prn) pr_info("mydrv3: ioctl cmd = %d\n", cmd);

       /*************************************************************************/
       if (cmd == 101){
              if(copy_from_user((void*)&value, (void*)arg, sizeof(int)) != 0){
                     if (prn) pr_err("mydrv3: ioctl FAIL 101\n");
                     return -EFAULT;
              }

              if (prn) pr_info("mydrv3: value = %d\n", value);
              
              if(value == 0){
                     mydrv3_write_gpio_5(value);
              }
              else if(value == 1){
                     mydrv3_write_gpio_5(value);
              }
              else {
                     if (prn) pr_err("mydrv3: ioctl FAIL 102\n");
                     return -EINVAL;
              }
       }

       /*************************************************************************/
       if (cmd == 102){
              if(copy_to_user((void*)arg, (void*)&cmn.count1, sizeof(int)) != 0){
                     if (prn) pr_err("mydrv3: ioctl FAIL 201\n");
                     return -EFAULT;
              }
       }

       /*************************************************************************/
       if (cmd == 201){
              if(copy_from_user((void*)&value, (void*)arg, sizeof(int)) != 0){
                     if (prn) pr_err("mydrv3: ioctl FAIL 101\n");
                     return -EFAULT;
              }

              value++;

              if(copy_to_user((void*)arg, (void*)&value, sizeof(int)) != 0){
                     if (prn) pr_err("mydrv3: ioctl FAIL 102\n");
                     return -EFAULT;
              }
       }

	return 0;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
