#include "transaction.h"

void *start_routine(void *ptr)
{
    int fd = *(int *)ptr;
    char buf[MSG_BUF];
    
    printf("this is a new thread,you got connected!\n");
    printf("fd=%d\n",fd);

    int msg_size = 0;
    if ((msg_size=recv(fd, buf, sizeof(buf), 0)) == -1){ 
        printf("recv() error in new thread!\n"); 
        exit(1); 
    } 
    printf("Get the message from cilent：%s ", buf);
    
    if (pthread_mutex_lock(&mutex) != 0)
    {    
        fprintf(stdout, "lock error!\n");
    }

    // 将数据写入文件中
    append_data(buf);

    // 解锁
    pthread_mutex_unlock(&mutex);
    
    //发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
    send(fd, "Data processing is completed!", sizeof("Data processing is completed!"), 0);
    
    msg_size = recv(fd, buf, sizeof(buf), 0);	
    if(0 >= msg_size)    //对端关闭连接返回0
    {	
        printf("The client closes the connection！\n");
    }else{
        printf("buf: %s\n", buf);
    }	
    close(fd);
}