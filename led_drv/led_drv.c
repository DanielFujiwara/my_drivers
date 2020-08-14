#include <linux/init.h>		//__init __exit
#include <linux/kernel.h>	//printk
#include <linux/module.h>	//module_init module_exit...
#include <linux/cdev.h>		//字符设备
#include <linux/fs.h>		//file_operations
#include <linux/uaccess.h>	//copy_from_user、copy_to_user
#include <linux/device.h>	//class_create、device_create
#include <linux/ioport.h>	//request_mem_region 
#include <linux/io.h>		//ioremap
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/miscdevice.h>//misc_register
#include <linux/ioctl.h>
#include <linux/delay.h>

#define CMD_LED_D7		_IOW('L',0,unsigned int)
#define CMD_LED_D8		_IOW('L',1,unsigned int)
#define CMD_LED_D9		_IOW('L',2,unsigned int)
#define CMD_LED_D10		_IOW('L',3,unsigned int)


// //声明一个led字符设备结构体
// static struct cdev led_cdev;

// //声明一个led的设备号
// static dev_t led_num;

// //声明一个led的类指针
// static struct class  *led_class;

// //声明一个led的设备指针
// static struct device  *led_device;

//声明一个led的资源指针
// static struct resource *led_resource;
static struct gpio leds_gpios[] = {
	{ PAD_GPIO_E+13, GPIOF_OUT_INIT_HIGH, "LED D7" }, /* default to OFF */
	{ PAD_GPIO_C+17, GPIOF_OUT_INIT_HIGH, "LED D8" }, /* default to OFF */
	{ PAD_GPIO_C+8, GPIOF_OUT_INIT_HIGH, "LED D9" }, /* default to OFF */
	{ PAD_GPIO_C+7, GPIOF_OUT_INIT_HIGH, "LED D10" }, /* default to OFF */
};


//声明一个gpioe 13起始虚拟地址的指针
// void __iomem *gpioa_va=NULL;
// void __iomem *gpiob_va=NULL;
// void __iomem *gpioc_va=NULL;
// void __iomem *gpiod_va=NULL;
// void __iomem *gpioe_va=NULL;

// void __iomem *gpioa_out_va=NULL;
// void __iomem *gpiob_out_va=NULL;
// void __iomem *gpioc_out_va=NULL;
// void __iomem *gpiod_out_va=NULL;
// void __iomem *gpioe_out_va=NULL;

// void __iomem *gpioa_outenb_va=NULL;
// void __iomem *gpiob_outenb_va=NULL;
// void __iomem *gpioc_outenb_va=NULL;
// void __iomem *gpiod_outenb_va=NULL;
// void __iomem *gpioe_outenb_va=NULL;

// void __iomem *gpioa_atlfn0_va=NULL;
// void __iomem *gpioa_atlfn1_va=NULL;

// void __iomem *gpiob_atlfn0_va=NULL;
// void __iomem *gpiob_atlfn1_va=NULL;

// void __iomem *gpioc_atlfn0_va=NULL;
// void __iomem *gpioc_atlfn1_va=NULL;

// void __iomem *gpiod_atlfn0_va=NULL;
// void __iomem *gpiod_atlfn1_va=NULL;

// void __iomem *gpioe_atlfn0_va=NULL;
// void __iomem *gpioe_atlfn1_va=NULL;


//设备打开接口

int led_open (struct inode * inode, struct file *file)
{
	//点灯
	gpio_set_value(PAD_GPIO_E+13,0);
		
	gpio_set_value(PAD_GPIO_C+17,0);

	gpio_set_value(PAD_GPIO_C+8,0);
		
	gpio_set_value(PAD_GPIO_C+7,0);
	printk(KERN_INFO"led_open\n");
	
	
	return 0;
}

//设备关闭
int led_close (struct inode * inode, struct file *file)
{
	
	gpio_set_value(PAD_GPIO_E+13,1);
		
	gpio_set_value(PAD_GPIO_C+17,1);

	gpio_set_value(PAD_GPIO_C+8,1);
		
	gpio_set_value(PAD_GPIO_C+7,1);
	printk(KERN_INFO"led_close\n");
	
	
	return 0;
}

ssize_t led_write (struct file *file,const char __user *buf, size_t len, loff_t *offset)
{
	char kbuf[2]={0};
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

		if(rt != 0)
		return -EFAULT;
	
	switch(kbuf[0])
	{
		case 1://D7灯的亮与灭
		{
			gpio_set_value(PAD_GPIO_E+13,!kbuf[1]);
		}
		break;
		case 2://D8灯的亮与灭
		{
			gpio_set_value(PAD_GPIO_C+17,!kbuf[1]);
		}
		break;
		case 3://D9灯的亮与灭
		{
			gpio_set_value(PAD_GPIO_C+8,!kbuf[1]);
		}
		break;
		case 4://D10灯的亮与灭
		{
			gpio_set_value(PAD_GPIO_C+7,!kbuf[1]);
		}
		break;
		default:
			return -EINVAL;
			break;
	}

	// if(buf[1] == '1') //亮灯
	// {
	// 	switch (buf[0])
	// 	{
	// 		case '1':
	// 					(*(volatile unsigned int *)gpioe_va)&=~(1<<13);
	// 					return len;
	// 		case '2':
	// 					(*(volatile unsigned int *)gpioc_va)&=~(1<<17);
	// 					return len;
	// 		case '3':
	// 					(*(volatile unsigned int *)gpioc_va)&=~(1<<8);
	// 					return len;

	// 		case '4':
	// 					(*(volatile unsigned int *)gpioc_va)&=~(1<<7);
	// 					return len;
	// 	}
	// }
	// else if(buf[1] == '0') //灭灯
	// {
	// 	switch (buf[0])
	// 	{
	// 		case '1':
	// 					(*(volatile unsigned int *)gpioe_va)|=(1<<13);
	// 					return len;
	// 		case '2':
	// 					(*(volatile unsigned int *)gpioc_va)|=(1<<17);
	// 					return len;
	// 		case '3':
	// 					(*(volatile unsigned int *)gpioc_va)|=(1<<8);
	// 					return len;

	// 		case '4':
	// 					(*(volatile unsigned int *)gpioc_va)|=(1<<7);
	// 					return len;
	// 	}
	// }

	
	
	// printk(KERN_INFO"kbuf:%s\n",kbuf);
	
	
	//返回实际写入数据的长度
	return len;
}

// read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)

ssize_t led_read (struct file *file, char __user *buf, size_t len, loff_t * offs)
{
	char kbuf[5]="asdf";
	int rt;
	
	
	if(len > sizeof kbuf)
		{
			printk(KERN_INFO"read error\n");
			return -EINVAL;
			
		}

	//拷贝用户空间的数据，类似于memcpy
	//返回没有被成功拷贝的字节数
	rt = copy_to_user(buf,kbuf,len);
	// printk(KERN_INFO"kbuf:%s \n",kbuf);
	// printk(KERN_INFO"rt = %d\n",rt);
	//得到实际拷贝的字节数
	//len=16
	//rt=2	
	len = len - rt;
	
	// printk(KERN_INFO"kbuf:%s\n",kbuf);
	
	
	//返回实际读取数据的长度
	return len;
}
long led_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
{
	switch(cmd)
	{
		case CMD_LED_D7:
		{
			gpio_set_value(PAD_GPIO_E+13,args?0:1);
			break;
		}
		case CMD_LED_D8:
		{
			gpio_set_value(PAD_GPIO_C+17,args?0:1);
			break;
		}
		case CMD_LED_D9:
		{
			gpio_set_value(PAD_GPIO_C+8,args?0:1);
			break;
		}
		case CMD_LED_D10:
		{
			gpio_set_value(PAD_GPIO_C+7,args?0:1);
			break;
		}
		default:
			return -ENOIOCTLCMD;
	}
}


struct file_operations led_fops={
		.owner          = THIS_MODULE,
		.open           = led_open,
		.release        = led_close,
		.write          = led_write,
		.read           = led_read,
		.unlocked_ioctl = led_ioctl,
	
};

static struct miscdevice led_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, //MISC_DYNAMIC_MINOR,动态分配次设备号
	.name = "myled_drv", //设备名称,/dev/myled
	.fops = &led_fops,//文件操作集
};


static int __init myled_init(void)
{
	int rt;
	
	//混杂设备的注册
	rt = misc_register(&led_miscdev);
	
	if(rt < 0)
	{
		printk(KERN_INFO"misc_register error\n");
		
		goto err_misc_register;
		
	}

	//由于内核已经申请过引脚，必须先释放
	gpio_free_array(leds_gpios, ARRAY_SIZE(leds_gpios));
	
	//申请一组GPIO
	rt = gpio_request_array(leds_gpios, ARRAY_SIZE(leds_gpios));
	if (rt < 0)	
	{
		printk(KERN_INFO"gpio_request_array error\n");
		goto err_gpio_request_array;
	}
	//申请物理地址空间
	// led_resource=request_mem_region(0xC001A000,0x28,"gpioa");
	
	// if(led_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpioa\n");
		
	// 	goto err_request_mem_region;
	// }

	// led_resource=request_mem_region(0xC001B000,0x28,"gpiob");
	
	// if(led_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpiob\n");
		
	// 	goto err_request_mem_region;
	// }

	// led_resource=request_mem_region(0xC001C000,0x28,"gpioc");
	
	// if(led_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpioc\n");
		
	// 	goto err_request_mem_region;
	// }

	// led_resource=request_mem_region(0xC001D000,0x28,"gpiod");
	
	// if(led_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpiod\n");
		
	// 	goto err_request_mem_region;
	// }


	// led_resource=request_mem_region(0xC001E000,0x28,"gpioe");
	
	// if(led_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error\n");
		
	// 	goto err_request_mem_region;
	// }

	gpio_set_value(PAD_GPIO_E+13,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_E+13,0);
	mdelay(300);
	gpio_set_value(PAD_GPIO_C+17,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_C+17,0);
	mdelay(300);
	gpio_set_value(PAD_GPIO_C+8,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_C+8,0);
	mdelay(300);
	gpio_set_value(PAD_GPIO_C+7,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_C+7,0);
	mdelay(300);

	gpio_set_value(PAD_GPIO_E+13,1);
	gpio_set_value(PAD_GPIO_C+17,1);
	gpio_set_value(PAD_GPIO_C+8,1);
	gpio_set_value(PAD_GPIO_C+7,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_E+13,0);
	gpio_set_value(PAD_GPIO_C+17,0);
	gpio_set_value(PAD_GPIO_C+8,0);
	gpio_set_value(PAD_GPIO_C+7,0);
	mdelay(300);
	gpio_set_value(PAD_GPIO_E+13,1);
	gpio_set_value(PAD_GPIO_C+17,1);
	gpio_set_value(PAD_GPIO_C+8,1);
	gpio_set_value(PAD_GPIO_C+7,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_E+13,0);
	gpio_set_value(PAD_GPIO_C+17,0);
	gpio_set_value(PAD_GPIO_C+8,0);
	gpio_set_value(PAD_GPIO_C+7,0);
	mdelay(300);
	gpio_set_value(PAD_GPIO_E+13,1);
	gpio_set_value(PAD_GPIO_C+17,1);
	gpio_set_value(PAD_GPIO_C+8,1);
	gpio_set_value(PAD_GPIO_C+7,1);
	mdelay(300);
	gpio_set_value(PAD_GPIO_E+13,0);
	gpio_set_value(PAD_GPIO_C+17,0);
	gpio_set_value(PAD_GPIO_C+8,0);
	gpio_set_value(PAD_GPIO_C+7,0);
	mdelay(300);

	gpio_set_value(PAD_GPIO_E+13,1);
	gpio_set_value(PAD_GPIO_C+17,1);
	gpio_set_value(PAD_GPIO_C+8,1);
	gpio_set_value(PAD_GPIO_C+7,1);


	
	//由于linux需要虚拟地址访问，将物理地址转换为虚拟地址
	// gpioa_va=ioremap(0xC001A000,0x28);
	
	// if(gpioa_va == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"ioremap error\n");
		
	// 	goto err_ioremapa;
	// }	

	// gpiob_va=ioremap(0xC001B000,0x28);
	
	// if(gpiob_va == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"ioremap error\n");
		
	// 	goto err_ioremapb;
	// }	


	// gpioc_va=ioremap(0xC001C000,0x28);
	
	// if(gpioc_va == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"ioremap error\n");
		
	// 	goto err_ioremapc;
	// }	


	// gpiod_va=ioremap(0xC001D000,0x28);
	
	// if(gpiod_va == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"ioremap error\n");
		
	// 	goto err_ioremapd;
	// }	

	// gpioe_va=ioremap(0xC001E000,0x28);
	
	// if(gpioe_va == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"ioremap error\n");
		
	// 	goto err_ioremape;
	// }	

	
	
	//得到相应的寄存器虚拟地址
	// gpioa_out_va    = gpioa_va+0x00;
	// gpioa_outenb_va = gpioa_va+0x04;
	// gpioa_atlfn0_va = gpioa_va+0x20;
	// gpioa_atlfn1_va = gpioa_va+0x24;

	// gpiob_out_va    = gpiob_va+0x00;
	// gpiob_outenb_va = gpiob_va+0x04;
	// gpiob_atlfn0_va = gpiob_va+0x20;
	// gpiob_atlfn1_va = gpiob_va+0x24;

	// gpioc_out_va    = gpioc_va+0x00;
	// gpioc_outenb_va = gpioc_va+0x04;
	// gpioc_atlfn0_va = gpioc_va+0x20;
	// gpioc_atlfn1_va = gpioc_va+0x24;

	// gpiod_out_va    = gpiod_va+0x00;
	// gpiod_outenb_va = gpiod_va+0x04;
	// gpiod_atlfn0_va = gpiod_va+0x20;
	// gpiod_atlfn1_va = gpiod_va+0x24;

	// gpioe_out_va    = gpioe_va+0x00;
	// gpioe_outenb_va = gpioe_va+0x04;
	// gpioe_atlfn0_va = gpioe_va+0x20;
	// gpioe_atlfn1_va = gpioe_va+0x24;


	//配置GPIOE13为GPIO模式
	// (*(volatile unsigned int *)gpioe_atlfn0_va)&=~(3<<26);			
	//配置GPIOE13为输出功能
	// (*(volatile unsigned int *)gpioe_outenb_va)|=1<<13;
	

		//配置GPIOC17为GPIO模式
	// (*(volatile unsigned int *)gpioc_atlfn1_va)&=~(3<<2);
	// (*(volatile unsigned int *)gpioc_atlfn1_va)|=(1<<2);
	//配置GPIOC17为输出功能
	// (*(volatile unsigned int *)gpioc_outenb_va)|=1<<17;


		//配置GPIOC8为GPIO模式
	// (*(volatile unsigned int *)gpioc_atlfn0_va)&=~(3<<16);		
	// (*(volatile unsigned int *)gpioc_atlfn0_va)|=(1<<16);		
	//配置GPIOCS8为输出功能
	// (*(volatile unsigned int *)gpioc_outenb_va)|=1<<8;


		//配置GPIOC7为GPIO模式
	// (*(volatile unsigned int *)gpioc_atlfn0_va)&=~(3<<14);			
	// (*(volatile unsigned int *)gpioc_atlfn0_va)|=(1<<14);			
	//配置GPIOC7为输出功能
	// (*(volatile unsigned int *)gpioc_outenb_va)|=1<<7;


	// (*(volatile unsigned int *)gpioe_va)|=(1<<13);
	// (*(volatile unsigned int *)gpioc_va)|=(1<<17);
	// (*(volatile unsigned int *)gpioc_va)|=(1<<8);
	// (*(volatile unsigned int *)gpioc_va)|=(1<<7);
	
	printk(KERN_INFO"myled_init\n");


	//成功返回
	return 0;
	
// err_ioremape:
	// release_mem_region(0xC001E000,0x28);
// err_ioremapd:
// 	release_mem_region(0xC001D000,0x28);
// err_ioremapc:
	// release_mem_region(0xC001C000,0x28);
// err_ioremapb:
// 	release_mem_region(0xC001B000,0x28);	
// err_ioremapa:
// 	release_mem_region(0xC001A000,0x28);




err_gpio_request_array:
	misc_deregister(&led_miscdev);

err_misc_register:	
	return rt;
	
	
}	


// ioctl
// long led_ioctl (struct file *filp, unsigned int cmd, unsigned long args)





static void __exit myled_exit(void)
{
	//解除映射
	// iounmap(gpioa_va);
	// iounmap(gpiob_va);
	// iounmap(gpioc_va);
	// iounmap(gpiod_va);
	// iounmap(gpioe_va);
	//释放物理内存
	gpio_free_array(leds_gpios, ARRAY_SIZE(leds_gpios));

	// release_mem_region(0xC001A000,0x28);
	// release_mem_region(0xC001B000,0x28);
	// release_mem_region(0xC001C000,0x28);
	// release_mem_region(0xC001D000,0x28);
	// release_mem_region(0xC001E000,0x28);

	//注销混杂设备
	misc_deregister(&led_miscdev);
	

	
	printk(KERN_INFO"myled_exit\n");
}



//加载驱动：当使用insmod led_drv.ko的时候，会调用module_init函数，module_init函数会回调myled_init函数
module_init(myled_init);

//卸载驱动：当使用rmmod led_drv的时候，会调用module_exit函数，module_exit函数会回调myled_exit函数
module_exit(myled_exit);

//添加作者信息[可选添加的]
MODULE_AUTHOR("511");

//添加模块描述[可选添加的]
MODULE_DESCRIPTION("This is led driver");

//添加GPL许可证[必须的]
MODULE_LICENSE("GPL");