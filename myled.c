// 	Copyright (C) 2020 Ryuichi Ueda and Yokoyama Ryota. All right reserved. //

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#define RED_PIN 25
#define GRE_PIN 24
#define BLU_PIN 23
MODULE_AUTHOR("Ryuichi Ueda and Yokoyama Ryota");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base=NULL;
static bool com_str(char* str,char* str2,int n)
{
	int i=0;
	bool flag=true;
	for(i=0;i<n;i++)
	{
		if(str[i]!=str2[i])
		{
			flag=false;
			break;
		}
	}
	return flag;
}
static void rgb(bool red,bool green,bool blue)
{
	if(red)
	{
		gpio_base[7]=1<<RED_PIN;
	}
	else
	{
		gpio_base[10]=1<<RED_PIN;
	}
	if(green)
	{
		gpio_base[7]=1<<GRE_PIN;
	}
	else
	{
		gpio_base[10]=1<<GRE_PIN;
	}
	if(blue)
	{
		gpio_base[7]=1<<BLU_PIN;
	}
	else
	{
		gpio_base[10]=1<<BLU_PIN;
	}
}
static ssize_t led_write(struct file* filp,const char * buf,size_t count,loff_t* pos)
{
	char c;
	char *str=kmalloc(sizeof(char)*count,GFP_KERNEL);
	if(copy_from_user(str,buf,sizeof(char)*count))
	{
		kfree(str);
		return -EFAULT;
	}
	if(com_str(str,"red",3))
	{
		rgb(1,0,0);
	}
	else if(com_str(str,"green",5))
	{
		rgb(0,1,0);
	}
	else if(com_str(str,"blue",4))
	{
		rgb(0,0,1);
	}
	else if(com_str(str,"purple",6))
	{
		rgb(1,0,1);
	}
	else if(com_str(str,"yellow",6))
	{
		rgb(1,1,0);
	}
	else if(com_str(str,"white",5))
	{
		rgb(1,1,1);
	}
	else if(com_str(str,"clear",5))
	{
		rgb(0,0,0);
	}
	kfree(str);
	return count;
}
static ssize_t sushi_read(struct file* filp,char *buf,size_t count,loff_t* pos)
{
	int size = 0;
	char sushi[]={"sushi"};
	if(copy_to_user(buf+size,(const char *)sushi,sizeof(sushi)))
	{
		printk(KERN_ERR"sushi copy_to_user failed\n");
		return -EFAULT;
	}
	size+= sizeof(sushi);
	return size;

}
static struct file_operations led_fops={
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read

};
static int gpio_set(int pin)
{
	uint32_t index=pin/10;
	uint32_t shift=(pin%10)*3;
	uint32_t mask=~(0x07<<shift);
	gpio_base[index]=(gpio_base[index]&mask)|(0x01<<shift);
}
static int __init init_mod(void)
{
	int retval;
	retval=alloc_chrdev_region(&dev,0,1,"myled");
	if(retval<0)
	{
		printk(KERN_ERR"alloc_chardev_region failed \n");
		return retval;
	}
	printk(KERN_INFO"%s is loaded.major :%d\n",__FILE__,MAJOR(dev));
	cdev_init(&cdv,&led_fops);
	retval=cdev_add(&cdv,dev,1);
	if(retval<0)
	{
		printk(KERN_ERR"cdev_add failed.major:%d,minor%d\n",MAJOR(dev),MINOR(dev));
	}
	cls=class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls))
	{
		printk(KERN_ERR"class_create failed");
		return PTR_ERR(cls);
	}
	device_create(cls,NULL,dev,NULL,"myled%d",MINOR(dev));
	gpio_base=ioremap_nocache(0x3f200000,0xA0);
	gpio_set(RED_PIN);
	gpio_set(GRE_PIN);
	gpio_set(BLU_PIN);
	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls,dev);
	class_destroy(cls);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO"%s is unloded.major%d\n",__FILE__,MAJOR(dev));
}
module_init(init_mod);
module_exit(cleanup_mod);
