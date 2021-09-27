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
#define MODNAME "SOYO_FEEBS_C2B"
#define MAXLEN  1000    //队列最大深度 

MODULE_LICENSE("GPL");
/**************************************************
 *     深圳冠标科技发展有限公司                
 *     消防应急疏散广播系统FEEBS              
 *     Fire Emergency Evacuation Broadcast System  
 *     云端到基站端内核队列模块                                  
 *     内核名称  SOYO_FEEBS_C2B                                   
 *     作者 卢汉利
 *     时间 2021-03-28
 *     版本 v1.0
 **************************************************/                                   

//-------- 数据定义
//定义输出缓存
char OutData[1000];
//定义队列深度
static int  Qdepth;

//---------------定义链表队列结构--------------------
typedef struct QNode
{
    char 		data[1000];
    struct 	QNode * next;
}QNode;

//-------------定义队列，队列头，队列尾
QNode *queue,*top,*rear;

//-------------初始化队列
QNode * initQueue(void)
{
    QNode *queue=(QNode*)vmalloc(sizeof(QNode));
    queue->next=NULL;
    return queue;
}

//-------------从队列尾插入元素进入队列
QNode * enQueue(QNode * rear,char * data)
{
    QNode * enElem=(QNode*)vmalloc(sizeof(QNode));
    strcpy(enElem->data,data);
    enElem->next=NULL;
    //使用尾插法向链队列中添加数据元素
    rear->next=enElem;
    rear=enElem;
    Qdepth++;  //加队列深度
    return rear;
}

//-------------从队列头摘取队列元素
QNode* DeQueue(QNode * top,QNode * rear)
{
	  QNode * p;
    if (top->next==NULL) {
        strcpy(OutData,"Empty");
        return rear;
    }
    p=top->next;
    strcpy(OutData,p->data);
    top->next=p->next;
    if (rear==p) {
        rear=top;
    }
    vfree(p);
    Qdepth--; //减队列深度
    return rear;
}

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
//        seq_printf(m, "current kernel time is %ld\n", jiffies);        
        rear=DeQueue(top, rear);
//        printk("Module %s deQueue Data: %s\n",MODNAME,OutData);	
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
	char *wd;
	if(len > 0 )
	{
			wd = (char *)vmalloc( len+1 );
//			printk("Write to kernel data %d byte\n",len);
			copy_from_user(wd, data, len );
			wd[len]='\0';
//			printk("Data: %s\n",wd);
			rear=enQueue(rear, wd);
			vfree(wd);
			if(Qdepth>MAXLEN)  //队列深度过长,抛弃最老队列节点
			{
				printk("Module %s Queue Depth %d is too long, Free the Top Queue Message \n",MODNAME,Qdepth);
				rear=DeQueue(top, rear);				
			}
//						
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
	printk("----------------------------\n");
	printk("Load module %s",MODNAME);
	printk("----------------------------\n");
	Qdepth=0; //初始化队列深度
  queue=top=rear=initQueue();//创建头结点	
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