/*
 * 项目结构
 * |---config.h 项目配置文件
 * |---append.h 定义了数据写入的操作
 * |---server.h 定义了服务器端的操作
 * |---test.h 测试
 */
#ifndef CONFIG__H
#define CONFIG__H

// 存储数据的文件路径
#define STORE_DIR "/home/deng/tcptest/server/data"
// TCP 监听的端口号
#define PORT 4466
// 最大监听的数量
#define BACKLOG 5

#endif