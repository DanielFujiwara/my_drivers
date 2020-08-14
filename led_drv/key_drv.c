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
#include <mach/devices.h>     //中断号
#include <linux/interrupt.h>  //常用的函数接口

// //声明一个key字符设备结构体
// static struct cdev key_cdev;

// //声明一个key的设备号
// static dev_t key_num;

// //声明一个key的类指针
// static struct class  *key_class;

// //声明一个key的设备指针
// static struct device  *key_device;

// //声明一个key的资源指针
// static struct resource *key_resource;

static struct gpio keys_gpios[] ={
{ PAD_GPIO_A+28,GPIOF_DIR_IN,"KEY2"},
{ PAD_GPIO_B+30,GPIOF_DIR_IN,"KEY3"},
{ PAD_GPIO_B+31,GPIOF_DIR_IN,"KEY4"},
{ PAD_GPIO_B+9,GPIOF_DIR_IN,"KEY4"},
};

//声明一个gpioa gpiob起始虚拟地址的指针
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


// void __iomem *gpioa_pad_va=NULL;
// void __iomem *gpiob_pad_va=NULL;
// void __iomem *gpioc_pad=NULL;
// void __iomem *gpiod_pad=NULL;
// void __iomem *gpioe_pad=NULL;


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
//中断服务函数
//irq，就是当前触发中断请求的中断号
//dev，就是request_irq传递的参数
irqreturn_t keys_irq_handler(int irq, void *dev)
{
	
	if(irq == gpio_to_irq(PAD_GPIO_A+28))
	{
		printk(KERN_INFO"K2 Down\n");
		
		
	}
	else if(irq == gpio_to_irq(PAD_GPIO_B+30))
	{
		printk(KERN_INFO"K3 Down\n");
	}
	else if(irq == gpio_to_irq(PAD_GPIO_B+31))
	{
		printk(KERN_INFO"K4 Down\n");
	}
	else if(irq == gpio_to_irq(PAD_GPIO_B+9))
	{
		printk(KERN_INFO"K1 Down\n");
	}


// { PAD_GPIO_A+28,GPIOF_DIR_IN,"KEY2"},
// { PAD_GPIO_B+30,GPIOF_DIR_IN,"KEY3"},
// { PAD_GPIO_B+31,GPIOF_DIR_IN,"KEY4"},
// { PAD_GPIO_B+9,GPIOF_DIR_IN,"KEY4"},

	return IRQ_HANDLED;//当前中断处理已经完成
}


//设备打开接口

int key_open (struct inode * inode, struct file *file)
{
	
	printk(KERN_INFO"key_open\n");
	
	
	return 0;
}

//设备关闭
int key_close (struct inode * inode, struct file *file)
{
	
	
	printk(KERN_INFO"key_close\n");
	
	
	return 0;
}

ssize_t key_write (struct file *file,const char __user *buf, size_t len, loff_t *offset)
{
	return 0;
}

// read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)

ssize_t key_read (struct file *file, char __user *buf, size_t len, loff_t * offs)
{
	int rt;
	char key_val = 0;
	// unsigned int pada = 0;
	// unsigned int padb = 0;
	
	if(buf == NULL)
		return -EINVAL;	//判断参数的合法性，若是非法，就返回EINVAL
	
	
	if(len >sizeof key_val)
		len = sizeof key_val;

	//读取按键的电平
	//K2按键的电平，对应key_val的bit0
	//K3按键的电平，对应key_val的bit1	
	//K4按键的电平，对应key_val的bit2
	//K6按键的电平，对应key_val的bit3
	
	// pad=(*(volatile unsigned int *)gpioa_pad_va);
	// pada = ioread32(gpioa_pad_va);
	// padb = ioread32(gpiob_pad_va);
	//若K2按键按下，bit0就设置为1，否则为0
	
	// key_val|=(pada &(1<<28))?0:1;
	// key_val|=(padb &(1<<30))?0:(1<<1);
	// key_val|=(padb &(1<<31))?0:(1<<2);
	// key_val|=(padb &(1<<9))?0:(1<<3);
	
	key_val|=(gpio_get_value(PAD_GPIO_A+28))?0:1;
	key_val|=(gpio_get_value(PAD_GPIO_B+30))?0:(1<<1);
	key_val|=(gpio_get_value(PAD_GPIO_B+31))?0:(1<<2);
	key_val|=(gpio_get_value(PAD_GPIO_B+9))?0:(1<<3);
	

	// printk(KERN_INFO"%d\n",CONFIG_GEC6818_LED_N);

	rt = copy_to_user(buf,&key_val,sizeof key_val);
	
	//得到实际拷贝的字节数
	len = len - rt;	
	
	
	//返回实际读取的字节数
	return len;
}


struct file_operations key_fops={
	 .owner = THIS_MODULE,
	 .open = key_open,
	 .release = key_close,
	 .write = key_write,
	 .read = key_read,
	
};


static struct miscdevice keys_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, //MISC_DYNAMIC_MINOR,动态分配次设备号
	.name = "mykey_drv", //设备名称,/dev/myled
	.fops = &key_fops,//文件操作集
};

static int __init mykey_init(void)
{
	int rt;

	//混杂设备的注册
	rt = misc_register(&keys_miscdev);
	
	if(rt < 0)
	{
		printk(KERN_INFO"misc_register error\n");
		
		goto err_misc_register;
		
	}

	//由于内核已经申请过引脚，必须先释放
	gpio_free_array(keys_gpios, ARRAY_SIZE(keys_gpios));
	
	//申请一组GPIO
	rt = gpio_request_array(keys_gpios, ARRAY_SIZE(keys_gpios));
	if (rt < 0)	
	{
		printk(KERN_INFO"gpio_request_array error\n");
		goto err_gpio_request_array;
	}
	
	// int rt;
	
	//中断号：gpio_to_irq(PAD_GPIO_A+28)
	//中断服务函数：keys_irq_handler
	//触发方式：下降沿触发，IRQF_TRIGGER_FALLING
	//注册名字："gpioa28"
	//传递参数：NULL
	rt = request_irq(gpio_to_irq(PAD_GPIO_A+28),keys_irq_handler,IRQF_TRIGGER_FALLING,"gpioa28",NULL);
	
	if(rt < 0)
	{
		printk("request_irq error\n");
		
		goto err_request_irq;
		
	}

		rt = request_irq(gpio_to_irq(PAD_GPIO_B+30),keys_irq_handler,IRQF_TRIGGER_FALLING,"gpiob30",NULL);
	
	if(rt < 0)
	{
		printk("request_irq error\n");
		
		goto err_request_irq;
		
	}

		rt = request_irq(gpio_to_irq(PAD_GPIO_B+31),keys_irq_handler,IRQF_TRIGGER_FALLING,"gpiob31",NULL);
	
	if(rt < 0)
	{
		printk("request_irq error\n");
		
		goto err_request_irq;
		
	}

		rt = request_irq(gpio_to_irq(PAD_GPIO_B+9),keys_irq_handler,IRQF_TRIGGER_FALLING,"gpiob9",NULL);
	
	if(rt < 0)
	{
		printk("request_irq error\n");
		
		goto err_request_irq;
		
	}




	//////////old school///////////
	//申请物理地址空间
	// key_resource=request_mem_region(0xC001A000,0x28,"gpioa");
	
	// if(key_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpioa\n");
		
	// 	goto err_request_mem_region;
	// }

	// key_resource=request_mem_region(0xC001B000,0x28,"gpiob");
	
	// if(key_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpiob\n");
		
	// 	goto err_request_mem_region;
	// }

	// key_resource=request_mem_region(0xC001C000,0x28,"gpioc");
	
	// if(key_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpioc\n");
		
	// 	goto err_request_mem_region;
	// }

	// key_resource=request_mem_region(0xC001D000,0x28,"gpiod");
	
	// if(key_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error gpiod\n");
		
	// 	goto err_request_mem_region;
	// }


	// key_resource=request_mem_region(0xC001E000,0x28,"gpioe");
	
	// if(key_resource == NULL)
	// {
	// 	rt = -ENOMEM;
		
	// 	printk(KERN_INFO"request_mem_region error\n");
		
	// 	goto err_request_mem_region;
	// }


	
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
	// gpioa_pad_va    = gpioa_va+0x18;
	// gpioa_atlfn0_va = gpioa_va+0x20;
	// gpioa_atlfn1_va = gpioa_va+0x24;

	// gpiob_out_va    = gpiob_va+0x00;
	// gpiob_outenb_va = gpiob_va+0x04;
	// gpiob_pad_va    = gpiob_va+0x18;
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

	// unsigned int v;
	//按钮复用设置
	//keyGPIOA28 AF0 ALTFN1
	// GPIOAALTF1 &= ~(3<<24);//复用26 27置零
	// v = ioread32(gpioa_atlfn1_va);
	// v &= ~(3<<24);
	// iowrite32(v,gpioa_atlfn1_va);

	//keyGPIOB30	AF1 ALTFN1
	//keyGPIOB31    AF1 ALTFN1
	// v = ioread32(gpiob_atlfn1_va);
	// v &= ~(3<<28);
	// v |= (1<<28);
	// iowrite32(v,gpiob_atlfn1_va);

	// v = ioread32(gpiob_atlfn1_va);
	// v &= ~(3<<30);
	// v |= (1<<30);
	// iowrite32(v,gpiob_atlfn1_va);
	//keyGPIOB9 AF0 ALTFN0
	// GPIOBALTF0 &= ~(3<<18);	
	// v = ioread32(gpiob_atlfn0_va);
	// v &= ~(3<<18);
	// iowrite32(v,gpiob_atlfn0_va);

	//按钮引脚设置输入模式
	// GPIOAOUTENB &= ~(1<<28);
	// GPIOBOUTENB &= ~(1<<30);
	// GPIOBOUTENB &= ~(1<<31);
	// GPIOBOUTENB &= ~(1<<9);

	// v = ioread32(gpioa_outenb_va);
	// v &= ~(1<<28);
	// iowrite32(v,gpioa_outenb_va);

	// v = ioread32(gpiob_outenb_va);
	// v &= ~(1<<30);
	// v &= ~(1<<31);
	// v &= ~(1<<9);
	// iowrite32(v,gpiob_outenb_va);


	
	printk(KERN_INFO"mykey_init\n");


	//成功返回
	return 0;
	
// err_ioremape:
	// release_mem_region(0xC001E000,0x28);
// err_ioremapd:
// 	release_mem_region(0xC001D000,0x28);
// err_ioremapc:
	// release_mem_region(0xC001C000,0x28);
// err_ioremapb:
	// release_mem_region(0xC001B000,0x28);	
// err_ioremapa:
	// release_mem_region(0xC001A000,0x28);




err_request_irq:
	return rt;
	
err_gpio_request_array:
	misc_deregister(&keys_miscdev);

err_misc_register:	
	return rt;
	
}	

static void __exit mykey_exit(void)
{

	//释放中断
	free_irq(gpio_to_irq(PAD_GPIO_A+28),NULL);
	free_irq(gpio_to_irq(PAD_GPIO_B+30),NULL);
	free_irq(gpio_to_irq(PAD_GPIO_B+31),NULL);
	free_irq(gpio_to_irq(PAD_GPIO_B+9),NULL);
	//解除映射
	gpio_free_array(keys_gpios, ARRAY_SIZE(keys_gpios));
	// iounmap(gpioa_va);
	// iounmap(gpiob_va);
	// iounmap(gpioc_va);
	// iounmap(gpiod_va);
	// iounmap(gpioe_va);
	//释放物理内存
	// release_mem_region(0xC001A000,0x28);
	// release_mem_region(0xC001B000,0x28);
	// release_mem_region(0xC001C000,0x28);
	// release_mem_region(0xC001D000,0x28);
	// release_mem_region(0xC001E000,0x28);

	// //设备的销毁
	// device_destroy(key_class,key_num);
	
	// //类的销毁
	// class_destroy(key_class);
	// //删除字符设备
	// cdev_del(&key_cdev);
	
	// //注销设备号
	// unregister_chrdev_region(key_num,1);


		
	//注销混杂设备
	misc_deregister(&keys_miscdev);

	
	printk(KERN_INFO"mykey_exit\n");
}



//加载驱动：当使用insmod key_drv.ko的时候，会调用module_init函数，module_init函数会回调mykey_init函数
module_init(mykey_init);

//卸载驱动：当使用rmmod key_drv的时候，会调用module_exit函数，module_exit函数会回调mykey_exit函数
module_exit(mykey_exit);

//添加作者信息[可选添加的]
MODULE_AUTHOR("511");

//添加模块描述[可选添加的]
MODULE_DESCRIPTION("This is key driver");

//添加GPL许可证[必须的]
MODULE_LICENSE("GPL");