#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define PORT 4466		//目标地址端口号
#define ADDR "8.129.147.222" //目标地址IP

int main()
{
	int iSocketFD = 0; //socket句柄
	unsigned int iRemoteAddr = 0;
	struct sockaddr_in stRemoteAddr = {0}; //对端，即目标地址信息
	socklen_t socklen = 0;  
	char data[] = "\"ID\":\"设备ID\",\"TIME\":\"2021-09-22 18:00:00\",\"TEMP\":\"37.52°C\",\"HR\":78,\"BOS\":96\n";	
	char buf[4096] = {0}; //存储接收到的数据
 
	iSocketFD = socket(AF_INET, SOCK_STREAM, 0); //建立socket
	if(0 > iSocketFD)
	{
		printf("创建socket失败！\n");
		return 0;
	}	
 
	stRemoteAddr.sin_family = AF_INET;
	stRemoteAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, ADDR, &iRemoteAddr);
	stRemoteAddr.sin_addr.s_addr=iRemoteAddr;
	
	//连接方法： 传入句柄，目标地址，和大小
	if(0 > connect(iSocketFD, (void *)&stRemoteAddr, sizeof(stRemoteAddr)))
	{
		printf("连接失败！\n");
		//printf("connect failed:%d",errno);//失败时也可打印errno
	}else{
		printf("连接成功！\n");
		send(iSocketFD, data, sizeof(data), 0);
		// 将接收数据打入buf，参数分别是句柄，储存处，最大长度，其他信息（设为0即可）。 
		recv(iSocketFD, buf, sizeof(buf), 0); 
		printf("Received:%s\n", buf);
	}
	
	close(iSocketFD);//关闭socket	
	return 0;
}