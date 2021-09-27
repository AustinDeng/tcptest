#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm-generic/uaccess.h> // Centos7.x版本
// #include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

//----------定义模块名称
#define MODNAME "SOYO_FEEBS_MON" 

MODULE_LICENSE("GPL");
/**************************************************
 *     深圳冠标科技发展有限公司                
 *     消防应急疏散广播系统FEEBS              
 *     Fire Emergency Evacuation Broadcast System  
 *     基站信息内核监控模块                                  
 *     内核名称  SOYO_FEEBS_MON                                  
 *     作者 卢汉利
 *     时间 2021-04-14
 *     版本 v1.0
 **************************************************/                                   

//-------- 数据定义
//定义输出缓存
char OutData[100000];
char app[100];
char appid[10];
char rt[40];

//---------------定义链表队列结构--------------------
typedef struct QNode
{
    char 		App[100]; //进程名
    char 		AppID[10]; //进程PID
    char    FreshTime[40]; //开始运行时间
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
QNode * enQueue(QNode * top,QNode * rear,char *app,char *appid, char *rt)
{
   			QNode * p;
   			QNode * enElem;
   			int i;
//			printk("App: %s\n",app);
//			printk("Appid: %s\n",appid);
//			printk("FreshTime: %s\n",rt);			
    for(p=top;p!=NULL;p=p->next) //遍历队列
    {
    	i=strcmp(p->App,app);
    	if(i==0)  //查到数据
    	{
//    		printk("Found Item,Update Now!\n");
    		strcpy(p->FreshTime,rt); //修改时间属性
    		strcpy(p->AppID,appid); //修改进程PID
    		return rear;
    	}    	
    }
		printk("New Item,Register Now!\n");
    enElem=(QNode*)vmalloc(sizeof(QNode));
    strcpy(enElem->App,app);
    strcpy(enElem->AppID,appid);
    strcpy(enElem->FreshTime,rt);
    enElem->next=NULL;
    //使用尾插法向链队列中添加数据元素
    rear->next=enElem;
    rear=enElem;    
    return rear;
}

//-------------从队列头遍历所有数据
void readQueue(QNode * top) //遍历队列
{
	    QNode * p;
	    int i=0;
	    int len=0;
	    char tmp[100];
	    if (top->next==NULL) 
	    {
        strcpy(OutData,"[]");
        return;
    	}
    	p=top->next;
    	strcpy(OutData,"[");
	    for(p=top;p!=NULL;p=p->next) //遍历队列
    	{
    		len = strlen(p->App);
    		if(len<=4)continue;
    		if(i>0)strcat(OutData,",");
    		sprintf(tmp,"{\"AppName\":\"%s\",\"AppID\":\"%s\",\"FreshTime\":\"%s\"}",p->App,p->AppID,p->FreshTime);
    		strcat(OutData,tmp);
    		i++;
    	}
    	strcat(OutData,"]");
    	return;
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
	        OutData[0]='\0';
	        readQueue(top);
//	        strcpy(OutData,"Test");
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
	int i,j,k;
	char wd[10][100];
	int dlen;
//	int flg;
	dlen=len;
	if(len > 20 )
	{
			copy_from_user(OutData, data, len );
		  //去除字符末端回车、换行、空格字符
    	for(i=len-1;i>0;i--)
    	{
    		if(OutData[i]==10||OutData[i]==13||OutData[i]==32)
    		{
    			OutData[i]='\0';
    			dlen--;
    		}
    		else
    		{
    			break;
    		}
    	} 
			OutData[len]='\0';
//			printk("Module %s Get data %s len %d\n",MODNAME,OutData,dlen);
			j=0;
			k=0;
			for(i=0;i<dlen;i++)
			{
				if(OutData[i]=='|')
				{
					wd[j][k++]='\0';
//					printk("Subdata:%s\n",wd[j]);
					k=0;
					j++;
				}
				else
				{
					wd[j][k++]=OutData[i];
				}				
			}
			
			if(j>=3) //
			{
				strcpy(app,wd[0]);
				strcpy(appid,wd[1]);
				strcpy(rt,wd[2]);				
			}
			rear=enQueue(top,rear,app,appid,rt);
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