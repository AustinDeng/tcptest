  #include <stdio.h>          
    #include <strings.h>
    #include<string.h> 
    #include <stdlib.h>        
    #include <unistd.h>          
    #include <sys/types.h> 
    #include <sys/socket.h> 
    #include <netinet/in.h> 
    #include <arpa/inet.h>
    #include<pthread.h>
    #define PORT 1234 
    #define BACKLOG 1  //  队列数，即可以排队的最大连接数

    void *start_routine(void *arg); // 函数声明

// 定义一个ARG结构体，每一个线程都会定义一个ARG结构体，相当于一块内存
struct ARG {
    struct sockaddr_in client;
        int connfd;
 } ;    


void *start_routine(void *arg)
{
    struct ARG *info;
    info=(struct ARG *)arg; 

    int fd =(*info).connfd;

    send(fd,"Welcome to my server.\n",22,0); 
    printf("this is a new thread\n");
 // 发现了一个很奇怪的现象：线程中printf不加换行符\n就打印不出信息
    close(fd);
    free(arg);

}


int  main()  
{ 

    int listenfd, connectfd;    
    struct sockaddr_in server; 
    struct sockaddr_in client;      
    int sin_size; 
    sin_size=sizeof(struct sockaddr_in); 


    struct ARG *arg; //事实证明：ARG结构体放主函数里和放全局变量里并没有区别

    pthread_t thread; //定义一个线程号

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {   
        perror("Creating socket failed.");
        exit(1);
    }


    int opt = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server,sizeof(server));  

    server.sin_family=AF_INET; 
    server.sin_port=htons(PORT); 
    server.sin_addr.s_addr = htonl (INADDR_ANY); 

    // 绑定
    if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) { 
    perror("Bind error.");
    exit(1); 
    }   

    // 监听 
    if(listen(listenfd,BACKLOG) == -1){  /* calls listen() */ 
    perror("listen() error\n"); 
    exit(1); 
    } 

    while(1)
    {
        // accept 
        if ((connectfd = accept(listenfd,(struct sockaddr *)&client,&sin_size))==-1) {
        perror("accept() error\n"); 
        exit(1); 
        }

        //printf("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); 

        arg=malloc(sizeof(struct ARG));
        arg->connfd=connectfd; // 连接描述符
        //这里注意：一定要指明结构体内部变量的指针！！！
        //不能直接指明结构体本身的指针
        memcpy((void *)&arg->client,&client,sizeof(client));


        pthread_create(&thread,NULL,start_routine,(void*)arg);


    }
    close(listenfd);

}
// ————————————————
// 版权声明：本文为CSDN博主「TBG2017」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/gepanqiang3020/article/details/73662546