#include <linux/init.h>		//__init __exit
#include <linux/kernel.h>	//printk
#include <linux/module.h>	//module_init module_exit...
#include <linux/cdev.h>		//字符设备
#include <linux/fs.h>		//file_operations
#include <linux/uaccess.h>	//copy_from_user、copy_to_user
#include <linux/device.h>	//class_create、device_create
#include <linux/ioport.h>	//request_mem_region 
#include <linux/io.h>		//ioremap
#include <linux/delay.h>
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/miscdevice.h>//misc_register


static struct gpio hcsr04_gpios[] ={
{ PAD_GPIO_D+19,GPIOF_OUT_INIT_HIGH,"19"},
{ PAD_GPIO_D+15,GPIOF_DIR_IN,"15"},
};


//测距
int distanceget(void)
{

	// unsigned int v;
	int d;
	//GPIOD19输出10us
	gpio_set_value(PAD_GPIO_D+19,1);
	
	//设置10微秒的高电平
	udelay(10);	
	//恢复GPIOD19低电平
	gpio_set_value(PAD_GPIO_D+19,0);

	int t = 0;

	//等待信号返回
	//添加超时处理，以防硬件问题（超声波模块的硬件故障、短路现象）让程序一直阻塞等待
	// gpio_get_value(PAD_GPIO_D+15)
	while(gpio_get_value(PAD_GPIO_D+15) == 0)
	{
		t++;
		udelay(1);
		if(t >= 1000000)
			return -1;	
	}

	t = 0;
	//有信号返回开始计时
	//添加超时处理，以防硬件问题（超声波模块的硬件故障、短路现象）让程序一直阻塞等待
	while(gpio_get_value(PAD_GPIO_D+15))
	{
		t++;
		udelay(5); //传输9us，传输3mm

		//若时间超长
		if(t >= 1000000)
			return -2;

	}

	t = t/2;
	d = 3*t;

	return d;

}
//设备打开接口

int hcsr04_open (struct inode * inode, struct file *file)
{
	
	printk(KERN_INFO"hcsr04_open\n");
	
	
	return 0;
}

//设备关闭
int hcsr04_close (struct inode * inode, struct file *file)
{
	
	
	printk(KERN_INFO"hcsr04_close\n");
	
	
	return 0;
}


// read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)

ssize_t hcsr04_read (struct file *file, char __user *buf, size_t len, loff_t * offs)
{
	
	int rt;
	int distance;
	
	char sr04_val=0;
	
	if(buf == NULL)
		return -EINVAL;	//判断参数的合法性，若是非法，就返回EINVAL
	
	
	if(len >sizeof sr04_val)
		len = sizeof sr04_val;	
	
	
	//读取超声波测距
	distance=distanceget();
	
	
	rt = copy_to_user(buf,&distance,sizeof distance);
	
	//得到实际拷贝的字节数
	len = len - rt;	
	
	
	//返回实际读取的字节数
	return len;
}


struct file_operations hcsr04_fops={
	 .owner = THIS_MODULE,
	 .open = hcsr04_open,
	 .release = hcsr04_close,
	 .read = hcsr04_read,
	
};

static struct miscdevice hcsr04_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, //MISC_DYNAMIC_MINOR,动态分配次设备号
	.name = "myhcsr04_drv", //设备名称,/dev/myled
	.fops = &hcsr04_fops,//文件操作集
};


static int __init myhcsr04_init(void)
{
	int rt;
	

	//混杂设备的注册
	rt = misc_register(&hcsr04_miscdev);
	
	if(rt < 0)
	{
		printk(KERN_INFO"misc_register error\n");
		
		goto err_misc_register;
		
	}
	//由于内核已经申请过引脚，必须先释放
	gpio_free_array(hcsr04_gpios, ARRAY_SIZE(hcsr04_gpios));
	
	//申请一组GPIO
	rt = gpio_request_array(hcsr04_gpios, ARRAY_SIZE(hcsr04_gpios));
	if (rt < 0)	
	{
		printk(KERN_INFO"gpio_request_array error\n");
		goto err_gpio_request_array;
	}

	//初始化19使其处于没输出状态
	gpio_set_value(PAD_GPIO_D+19,0);

	               
	
	printk(KERN_INFO"myhcsr04_init\n");


	//成功返回
	return 0;
	


err_gpio_request_array:
	misc_deregister(&hcsr04_miscdev);



err_misc_register:	
	return rt;
	
}	

static void __exit myhcsr04_exit(void)
{
	//解除映射
	// iounmap(gpiod_va);

	//释放物理内存

	// release_mem_region(0xC001D000,0x28);
	gpio_free_array(hcsr04_gpios, ARRAY_SIZE(hcsr04_gpios));

	misc_deregister(&hcsr04_miscdev);
	

	
	printk(KERN_INFO"myhcsr04_exit\n");
}



//加载驱动：当使用insmod hcsr04_drv.ko的时候，会调用module_init函数，module_init函数会回调myhcsr04_init函数
module_init(myhcsr04_init);

//卸载驱动：当使用rmmod hcsr04_drv的时候，会调用module_exit函数，module_exit函数会回调myhcsr04_exit函数
module_exit(myhcsr04_exit);

//添加作者信息[可选添加的]
MODULE_AUTHOR("511");

//添加模块描述[可选添加的]
MODULE_DESCRIPTION("This is hcsr04 driver");

//添加GPL许可证[必须的]
MODULE_LICENSE("GPL");