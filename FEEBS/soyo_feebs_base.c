#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h> //Centos7.x版本
// #include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

//----------定义模块名称
#define MODNAME "SOYO_FEEBS_BASE" 

MODULE_LICENSE("GPL");
/**************************************************
 *     深圳冠标科技发展有限公司                
 *     消防应急疏散广播系统FEEBS              
 *     Fire Emergency Evacuation Broadcast System  
 *     基站信息内核模块                                  
 *     内核名称  SOYO_FEEBS_BASE                                  
 *     作者 卢汉利
 *     时间 2021-03-31
 *     版本 v1.0
 **************************************************/                                   

//-------- 数据定义
//定义输出缓存
char OutData[100000];

/*-------------------------内核定义 ---------------------*/
/*******************************************************
 *  * seq_operations->show
 *   * 必须返回0，否则什么也显示不出来
 *      
 *
 *      --------------目标函数--------------
 *      当 cat /proc/helloworld文件的时候，将会执行这个文件，
 *      有什么想输出的，都通过seq_printf来输出
 *    ***************************************************/
static int my_proc_show(struct seq_file *m, void *v)
{
//	      printk("Read Kernel Module %s Data %s\n",MODNAME,OutData);
        seq_printf(m, "%s\n", OutData);         
        return 0;
}


/*******************************************************
 **     file_operations->open
 *******************************************************/
static int my_proc_open(struct inode *inode, struct file *file)
{
        return single_open(file, my_proc_show, NULL);
}

/********************************************************
 *         file_operations->soyo_write
 ********************************************************/
static ssize_t soyo_write(struct file * file,const char __user* data,size_t len,loff_t *off)
{
	int i;
	if(len > 0 )
	{
//		  memset(OutData, 0, sizeof(OutData));
			copy_from_user(OutData, data, len );
		  //去除字符末端回车、换行、空格字符
    	for(i=len-1;i>0;i--)
    	{
    		if(OutData[i]==10||OutData[i]==13||OutData[i]==32)
    		{
    			OutData[i]='\0';
    		}
    		else
    		{
    			break;
    		}
    	} 
			OutData[len]='\0';
//			printk("Module %s Modify Data as: %s\n",MODNAME,OutData);			
	}
	return len;
}
static struct file_operations soyo_proc_ops = {
	.owner 	= THIS_MODULE,
	.read 	= seq_read,
	.open   = my_proc_open,
  .release = single_release,
  .llseek = seq_lseek,
	.write = soyo_write
};


static int proc_init(void)
{
	printk("-------------------------\n");
	printk("Load module %s",MODNAME);
	printk("-------------------------\n");
//	memset(OutData, 0, sizeof(OutData)); 
	proc_create(MODNAME,0,NULL,&soyo_proc_ops);  
	return 0;
}
static void proc_exit(void)
{
	  printk("Unload module %s",MODNAME);
    remove_proc_entry(MODNAME,NULL);
}
 
module_init(proc_init);
module_exit(proc_exit);