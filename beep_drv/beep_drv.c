#include <linux/init.h>		//__init __exit
#include <linux/kernel.h>	//printk
#include <linux/module.h>	//module_init module_exit...
#include <linux/cdev.h>		//字符设备
#include <linux/fs.h>		//file_operations
#include <linux/uaccess.h>	//copy_from_user、copy_to_user
#include <linux/device.h>	//class_create、device_create
#include <linux/ioport.h>	//request_mem_region 
#include <linux/io.h>		//ioremap
#include <linux/gpio.h>     //gpio标准函数
#include <cfg_type.h>       //gpio引脚
#include <linux/miscdevice.h>//misc_register


//设备打开接口

int beep_open (struct inode * inode, struct file *file)
{

	printk(KERN_INFO"beep_open\n");
	
	
	return 0;
}

//设备关闭
int beep_close (struct inode * inode, struct file *file)
{
	
	printk(KERN_INFO"beep_close\n");
	gpio_direction_output(PAD_GPIO_C+14,0);
	
	
	return 0;
}

ssize_t beep_write (struct file *file,const char __user *buf, size_t len, loff_t *offset)
{
	char kbuf[1]={0};
	int rt;
	if(buf == NULL)
		return -EINVAL;	//判断参数的合法性，若是非法，就返回EINVAL
	
	
	if(len >sizeof kbuf)
		len = 2;

	//拷贝用户空间的数据，类似于memcpy
	//返回没有被成功拷贝的字节数
	//得到实际拷贝的字节数
	rt = copy_from_user(kbuf,buf,len);
	len = len - rt;
	if(buf[0] == '1') //亮灯
	{
		//配置GPIOE13引脚为输出模式，初始电平状态为高电平
		gpio_direction_output(PAD_GPIO_C+14,1);
	}
	else if(buf[0] == '0')
	{
		//配置GPIOE13引脚为输出模式，初始电平状态为低电平
		gpio_direction_output(PAD_GPIO_C+14,0);
	}
	
	// printk(KERN_INFO"kbuf:%s\n",kbuf);
	
	
	//返回实际写入数据的长度
	return len;
}

// read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)

ssize_t beep_read (struct file *file, char __user *buf, size_t len, loff_t * offs)
{
	
	return len;
}


struct file_operations beep_fops={
	 .owner = THIS_MODULE,
	 .open = beep_open,
	 .release = beep_close,
	 .write = beep_write,
	 .read = beep_read,
	
};

static struct miscdevice beep_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, //MISC_DYNAMIC_MINOR,动态分配次设备号
	.name = "mybeep_drv", //设备名称,/dev/myled
	.fops = &beep_fops,//文件操作集
};

static int __init mybeep_init(void)
{
	int rt;
	//混杂设备的注册
	rt = misc_register(&beep_miscdev);
	
	if(rt < 0)
	{
		printk(KERN_INFO"misc_register error\n");
		
		goto err_misc_register;
		
	}


	//释放内核占用的引脚
	gpio_free(PAD_GPIO_C+14);
	//申请GPIOC14引脚
	rt = gpio_request(PAD_GPIO_C+14,"gpioc14");
	
	if(rt < 0)
	{
		printk(KERN_INFO"gpio_request error\n");
		
		goto err_gpio_request_array;
	}
	/////////////
	
	//配置GPIOE13引脚为输出模式，初始电平状态为低电平
	gpio_direction_output(PAD_GPIO_C+14,0);
		
	printk(KERN_INFO"mybeep_init\n");

	//成功返回
	return 0;
	



err_gpio_request_array:
	misc_deregister(&beep_miscdev);

err_misc_register:	
	return rt;


	
}	

static void __exit mybeep_exit(void)
{
	//gpio接触
	gpio_free(PAD_GPIO_C+14);

		//注销混杂设备
	misc_deregister(&beep_miscdev);
	

	
	printk(KERN_INFO"mybeep_exit\n");
}



//加载驱动：当使用insmod beep_drv.ko的时候，会调用module_init函数，module_init函数会回调mybeep_init函数
module_init(mybeep_init);

//卸载驱动：当使用rmmod beep_drv的时候，会调用module_exit函数，module_exit函数会回调mybeep_exit函数
module_exit(mybeep_exit);

//添加作者信息[可选添加的]
MODULE_AUTHOR("511");

//添加模块描述[可选添加的]
MODULE_DESCRIPTION("This is beep driver");

//添加GPL许可证[必须的]
MODULE_LICENSE("GPL");