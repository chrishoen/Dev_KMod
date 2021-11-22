#include "device_file.h"
#include <linux/init.h>       /* module_init, module_exit */
#include <linux/module.h>     /* version info, MODULE_LICENSE, MODULE_AUTHOR, printk() */

MODULE_DESCRIPTION("mydriver1 Linux driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chrishoen");

/*===============================================================================================*/
static int mydriver1_init(void)
{
    int result = 0;
    printk( KERN_NOTICE "mydriver1: Initializating\n" );

    result = register_device();
    return result;
}

/*===============================================================================================*/
static void mydriver1_exit(void)
{
    printk( KERN_NOTICE "mydriver1: Exiting\n" );
    unregister_device();
}

/*===============================================================================================*/
module_init(mydriver1_init);
module_exit(mydriver1_exit);
