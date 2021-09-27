# tcptest

该项目构建了一套完整的TCP数据传输流程

1. 服务器端启动服务并监听端口
2. 客户端发送模拟数据
3. 服务器端接收到数据并将数据写入到内核队列或本地文件

## 项目结构

- clien
简单客户端程序，用于发送模拟数据
- FEEBS
内核队列
- server
服务器端程序

## 运行

### server
- 修改配置文件`tcptest/server/config.h`的存储路径`STORE_DIR`
- 执行命令
    
    ```
    $ cd tcptest/server
    $ cd make
    $ cd make test
    ```

### client

- 执行命令

    ```
    $ cd tcptest/client
    $ gcc client -o client
    $ ./client
    ```

### FEEBS
- 执行命令

    ```
    $ cd tcptest/FEEBS
    $ sudo make
    $ ls /proc | grep SOYO
    $ sudo chmod 777 SOYO_FEEBS_B2C
    $ echo 1234 > SOYO_FEEBS_B2C
    $ cat SOYO_FEEBS_B2C
    ```
