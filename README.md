# HSpeed WebServer

**HSpeed is a lightweight and efficient web server based on event-driven model. It is written in the C programming language. The project will be updated until feature have been completed. Have a fun. Mxxxxx.**
## Dev Environment

##Dev Tool


- 操作系统：Ubuntu 16.04

- 编辑器：Sublime + Vim

- 编译器：gcc 5.4.0

- 版本控制：git

- 压测工具：ab

--- 

## Compile and run

For now HSpeed only support Linux 2.6 or later.

Please make sure you have [cmake](https://cmake.org) installed.

```cmake
mkdir build && cd build
cmake .. && make
./HSpeed
```
By default the server accepts connections on port 3000, if you want to assign other port for the server. You can change the port value of the hspeed.conf file

## Timeline

**Now**

- 版本1.0已经完成，本地已调试通过。
　　-特性：

- [x] 基于事件驱动模型的多线程非阻塞I/O
- [x] 添加定时器，定时回调handler函数处理超时请求
- [x] 利用epoll边缘触发模式实现HTTP持久连接(HTTP Keep-Alive)
- [x] 多线程借助线程池实现
- [x] 使用状态机解析HTTP协议
- [x] HTTP GET 方法


## TODO

- [ ] FastCGI
- [ ] HTTP POST and other methods
---

