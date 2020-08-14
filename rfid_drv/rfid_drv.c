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

/*
	PC11 PC30  RST
	PC8  PD0   MISO
	PC6  PC31  MOSI
	PD7  PC7   SCK
	PD6  PC8   SDA
	
	gpio_set_value(PAD_GPIO_C+30, 0);
	gpio_set_value(PAD_GPIO_C+31, 0);
	gpio_set_value(PAD_GPIO_C+7, 0);
	gpio_set_value(PAD_GPIO_C+8, 0);

*/

static struct gpio rfid_gpios[] = {
	{ PAD_GPIO_C+30, GPIOF_OUT_INIT_LOW, "RST" }, /* default to OFF */
	{ PAD_GPIO_D+0, GPIOF_DIR_IN, "MISO" }, /* default to OFF */
	{ PAD_GPIO_C+31, GPIOF_OUT_INIT_LOW, "MOSI" }, /* default to OFF */
	{ PAD_GPIO_C+7, GPIOF_OUT_INIT_LOW, "SCK" }, /* default to OFF */
	{ PAD_GPIO_C+8, GPIOF_OUT_INIT_LOW, "SDA" }, /* default to OFF */
};


//设备打开接口

int rfid_open (struct inode * inode, struct file *file)
{
	
	printk(KERN_INFO"rfid_open\n");
	
	
	return 0;
}

//设备关闭
int rfid_close (struct inode * inode, struct file *file)
{
	
	
	printk(KERN_INFO"rfid_close\n");
	
	
	return 0;
}

ssize_t rfid_write (struct file *file,const char __user *buf, size_t len, loff_t *offset)
{
	return 0;
}

// read(struct file *file, char __user *userbuf, size_t bytes, loff_t *off)

ssize_t rfid_read (struct file *file, char __user *buf, size_t len, loff_t * offs)
{
	int rt;
	char rfid_val = 0;
	// unsigned int pada = 0;
	// unsigned int padb = 0;
	
	if(buf == NULL)
		return -EINVAL;	//判断参数的合法性，若是非法，就返回EINVAL
	
	
	if(len >sizeof rfid_val)
		len = sizeof rfid_val;

	
	//得到实际拷贝的字节数
	len = len - rt;	
	
	
	//返回实际读取的字节数
	return len;
}

struct file_operations rfid_fops={
	 .owner = THIS_MODULE,
	 .open = rfid_open,
	 .release = rfid_close,
	 .write = rfid_write,
	 .read = rfid_read,
	
};


static struct miscdevice rfid_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, //MISC_DYNAMIC_MINOR,动态分配次设备号
	.name = "myrfid_drv", //设备名称,/dev/myled
	.fops = &rfid_fops,//文件操作集
};


static int __init myrfid_init(void)
{
	int rt;

	//混杂设备的注册
	rt = misc_register(&rfid_miscdev);
	
	if(rt < 0)
	{
		printk(KERN_INFO"misc_register error\n");
		
		goto err_misc_register;
		
	}

	//由于内核已经申请过引脚，必须先释放
	gpio_free_array(rfid_gpios, ARRAY_SIZE(rfid_gpios));
	
	//申请一组GPIO
	rt = gpio_request_array(rfid_gpios, ARRAY_SIZE(keys_gpios));
	if (rt < 0)	
	{
		printk(KERN_INFO"gpio_request_array error\n");
		goto err_gpio_request_array;
	}
	

	//初始化GPIO
	gpio_set_value(PAD_GPIO_C+30, 0);
	gpio_set_value(PAD_GPIO_C+31, 0);
	gpio_set_value(PAD_GPIO_C+7, 0);
	gpio_set_value(PAD_GPIO_C+8, 0);


	printk(KERN_INFO"myrfid_init\n");


	//成功返回
	return 0;

	err_gpio_request_array:
	misc_deregister(&rfid_miscdev);

	err_misc_register:	
	return rt;

}