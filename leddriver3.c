#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <mach/gpio.h>


int led_port[1]={S5PV210_GPH3(3)};


dev_t devid=0;
int led_open(struct inode *inode,struct file *pfile)
{
    int i;
	for(i=0;i<1;i++)
	{
		gpio_direction_output(led_port[i],1);
	
	}
	return 0;
}

static void close_all(void)
{
	
	gpio_set_value(led_port[0],0);
}
int led_close(struct inode *inode,struct file *pfile)
{
	
	
	close_all();
	return 0;
}






//将write设备文件重
ssize_t led_write(struct file *pfile,const char __user *buffer,size_t len,loff_t *loff)
{
	char data;
	copy_from_user(&data,buffer,1);
//close_all();
	if(data==1)
	{
		
		gpio_set_value(led_port[0],0);
		
	
	}
	if(data!=1)
	{
		
		//close_all();
		gpio_set_value(led_port[0],1);
		
	
	}
	return 1;
}




struct file_operations fops={
    .owner=THIS_MODULE,
    .open=led_open,
	.release=led_close,//注册
	.write=led_write,
	
};

static int __init led_init(void)
{
	int i;
    printk(KERN_EMERG "hello world!i am a module.\n");
    
    if(alloc_chrdev_region(&devid,11,1,"ledid")==0)
    {
        printk(KERN_INFO "dev id was allocated. id is %u\n",devid);
        printk(KERN_INFO "major is %d,minor is %d\n",MAJOR(devid),MINOR(devid));
    }
    register_chrdev(MAJOR(devid),"just for show",&fops);
    //申请GPIO端口
	for(i=0;i<1;i++)
	{
		if(gpio_request(led_port[i],"led")<0)
			printk(KERN_INFO "request_gpio%u faild\n",i);
	
	
	}
	return 0;
}


static void __exit led_exit(void)
{
	int i;
    printk("Goodby!");
	
	for(i=0;i<1;i++)
	{
		gpio_free(led_port[i]);
	}
	
	unregister_chrdev(MAJOR(devid),"justforshow");//反注册
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");