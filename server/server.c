#include "server.h"

int server_main() {	
	 // 初始化互斥锁
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		// 互斥锁初始化失败
		printf("Init mutex fail!\n");
		return 1;
	}
	// socket句柄
	int iSocketFD = 0;
	// 接收成功后的返回值
	int iRecvLen = 0;
	// 建立连接后的句柄
	int new_fd = 0;
	// 接收消息的缓存
	char buf[MSG_BUF] = {0};
	// 本地地址信息结构，可对具体的属性赋值
	struct sockaddr_in stLocalAddr = {0};
	// 对方地址信息
	struct sockaddr_in stRemoteAddr = {0};
	socklen_t socklen = 0;  	
	// 建立socket
	iSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > iSocketFD) {
		printf("create socket fail!\n");
		return 0;
	}
	// 该属性表示接收本机或其他机器传输
	stLocalAddr.sin_family = AF_INET;
	// 端口号
	stLocalAddr.sin_port = htons(PORT);
	// IP，括号内容表示本机IP
	stLocalAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//绑定地址结构体和socket
	if(0 > bind(iSocketFD, (void *)&stLocalAddr, sizeof(stLocalAddr))) {
		printf("Binding fail！\n");
		return 0;
	}
	//开启监听 ，第二个参数是最大监听数
	if(0 > listen(iSocketFD, BACKLOG)) {
		printf("Listening fail!\n");
		return 0;
	}
	printf("Listening success!The iSocketFD: %d\n", iSocketFD);

	while(1) {
		//在这里阻塞直到接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
		new_fd = accept(iSocketFD, (void *)&stRemoteAddr, &socklen);
		if(0 > new_fd) {
			printf("Accept Fail!\n");
			return 0;
		}
		printf("Accept success！\n");

		pthread_t thread; //定义一个线程号
		pthread_create(&thread, NULL, start_routine, (void *)&new_fd);
	}
	close(iSocketFD);
	pthread_mutex_destroy(&mutex);
 
	return 0;
}