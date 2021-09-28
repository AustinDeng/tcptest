#include "server.h"
 
int server_main()
{	
	// socket句柄
	int iSocketFD = 0;
	// 接收成功后的返回值
	int iRecvLen = 0;
	// 建立连接后的句柄
	int new_fd = 0;
	// 接收消息的缓存
	char buf[128] = {0};
	// 本地地址信息结构图，下面有具体的属性赋值
	struct sockaddr_in stLocalAddr = {0};
	// 对方地址信息
	struct sockaddr_in stRemoteAddr = {0};
	socklen_t socklen = 0;  	
	// 建立socket
	iSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > iSocketFD)
	{
		printf("创建socket失败！\n");
		return 0;
	}	
 
	// 该属性表示接收本机或其他机器传输
	stLocalAddr.sin_family = AF_INET;
	// 端口号
	stLocalAddr.sin_port = htons(PORT);
	// IP，括号内容表示本机IP
	stLocalAddr.sin_addr.s_addr=htonl(INADDR_ANY);
 
	//绑定地址结构体和socket
	if(0 > bind(iSocketFD, (void *)&stLocalAddr, sizeof(stLocalAddr)))
	{
		printf("绑定失败！\n");
		return 0;
	}
 
	//开启监听 ，第二个参数是最大监听数
	if(0 > listen(iSocketFD, BACKLOG))
	{
		printf("监听失败！\n");
		return 0;
	}
 
	printf("iSocketFD: %d\n", iSocketFD);
	while(1) {
		//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
		new_fd = accept(iSocketFD, (void *)&stRemoteAddr, &socklen);
		if(0 > new_fd)
		{
			printf("接收失败！\n");
			return 0;
		}else{
			printf("接收成功！\n");
			recv(new_fd, buf, sizeof(buf), 0);
			printf("收到发送端的消息：%s ", buf);
			// 将数据写入文件中
			append_data(buf);
			//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
			send(new_fd, "OK!", sizeof("OK!"), 0);
		}
	
		// printf("new_fd: %d\n", new_fd);	
		iRecvLen = recv(new_fd, buf, sizeof(buf), 0);	
		if(0 >= iRecvLen)    //对端关闭连接返回0
		{	
			// printf("接收失败或者对端关闭连接！\n");
			printf("接收端关闭连接！\n");
		}else{
			printf("buf: %s\n", buf);
		}
	
		close(new_fd);
	}
	close(iSocketFD);
 
	return 0;
}