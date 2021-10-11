/*
 * 项目结构
 * |---config.h 项目配置文件,包括全局变量的定义
 * |---utils.h 定义了功能函数，包括数据的写入
 * |---server.h 定义了服务器端的操作
 * |---test.h 测试
 * |---Makefile
 *   |--- make           --> 生成 test 文件，直接 ./test 运行服务器，等待客户端连接
 *   |--- make clean     --> 清空编译产生的临时文件以及可执行文件
 */
#ifndef CONFIG__H
#define CONFIG__H

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <malloc.h>

// 存储数据的文件路径
#define STORE_DIR "/home/deng/tcptest/server/data"
// TCP 监听的端口号
#define PORT 4466
// 最大监听的数量
#define BACKLOG 5
// 消息缓存队列长度
#define MSG_BUF 128

// 文件读写锁
pthread_mutex_t mutex;

#endif