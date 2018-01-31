
从零实现一个Web Server
===

这是一个初学者企图串联所学知识结构所进行的一次项目之旅，在整个开发过程中取舍了很多，所以最终的目的就是不仅要开发出一个”可以用”的Web Server，更重要的是把自己所会的零碎的知识点串联起来，尽善尽美的完成这个Web Server

本文将会对 HSpeed 目前所具备的关键特性进行阐述，并总结开发过程中的一点心得。

## 多线程 + Non-Blocking

　　- 多线程实现采用线程池模型，即固定线程的数量。

　　- 非阻塞I/O使用fcntl函数实现。
  
    ```C++
　　int fcntl(int fd, int cmd);
　　int fcntl(int fd, int cmd, long arg);
　　int fcntl(int fd, int cmd, struct flock *lock);
    ```

## HTTP Keep-Alive

　　- HSpeed 支持 HTTP 长连接（Persistent Connections），即多个请求可以复用同一个 TCP 连接，以此减少由 TCP 建立/断开连接所带来的性能开销。
　　- 每到来一个请求，HSpeed 会对请求进行解析，判断请求头中是否存在 Connection: keep-alive 请求头。
　　- 如果存在，在处理完一个请求后会保持连接，并对数据缓冲区（用于保存请求内容，响应内容）及状态标记和定时器进行重置，否则，关闭连接。


## 定时器 Timer

　　- HSpeed 定时器的实现参考了 Nginx 的设计，Nginx 使用一颗红黑树来存储各个定时事件，每次事件循环时从红黑树中不断找出最小（早）的事件，如果超时则触发超时处理。

　　- 为了简化使用优先队列结构（小根堆实现），堆头为到期时间最小的节点。

　　- 新任务添加时设置超时时间（expire = add_time + timeout）。

　　- 每次大循环先通过find_time函数得到最早超时请求的剩余时间（timer = expire - find_time）。  

　　- 定时器相关代码见 [timer.h](https://github.com/MXXXXXXXXXX/HSpeed/blob/master/src_code/timer.h) 和 [timer.c](https://github.com/MXXXXXXXXXX/HSpeed/blob/master/src_code/timer.c)。

## HTTP Parser

　　- 由于网络的不确定性，我们并不能保证一次就能读取所有的请求数据。因此，对于每一个请求，我们都会开辟一段缓冲区用于保存已经读取到的数据。

　　- 实现一个 HTTP 状态机（Parser）来维持当前的解析状态。

　　- 相关代码见 [http_parse.h](https://github.com/MXXXXXXXXXX/HSpeed/blob/master/src_code/http_parse.h) 和 [http_parse.c](https://github.com/MXXXXXXXXXX/HSpeed/blob/master/src_code/ http_parse.c)。

## 线程池

　　- 使用互斥锁保证对线程池的互斥访问，使用条件变量实现同步。

　　- 初始化线程池，创建worker线程。

　　- 各worker最外层为while循环，获得互斥锁的线程可以进入线程池，若无task队列为空则 pthread_cond_wait自动解锁互斥量，置该线程为等待状态并等待条件触发。若存在task则取出队列第一个任务，**之后立即开锁，之后再并执行具体操作**。这里若先执行后开锁则在task完成前整个线程池处于锁定状态，其他线程不能取任务，相当于串行操作！

　　- 建立连接后，当客户端请求到达服务器端，创建task任务并添加到线程池task队列尾，当添加完task之后调用pthread_cond_signal唤醒因没有具体task而处于等待状态的worker线程。

## 一些小坑

　　- 使用Epoll的边缘触发模式调用accept时必须使用while，以避免在高并发情况下，客户端连接不上。
　　- 服务器端需设置监听套接字属性SO_REUSEADDR，以避免服务器端关闭套接字处于TIME_WAIT状态导致无法绑定端口。其他的看需求设定。
　　- 文件描述符设置EPOLLONESHOT事件，从而有助于保证一个socket连接在任一时刻只被一个线程处理。注意！！监听套接字切勿注册该事件。
　　- 处理SIGPIPE信号，以避免当客户端直接关闭套接字时，服务器向客户端write时，操作系统发送SIGPIPE。SIGPIPE信号的默认处理动作是终止进程。handle_for_sigpipe函数的目的就是把SIGPIPE信号的handler设置为SIG_IGN（忽略）而非终止服务器运行。
　　当然，HSpeed 还有许多不足之处，以及未实现的特性。

* 仅支持 HTTP GET 方法，暂不支持其他 HTTP method。
* 暂不支持动态请求的处理。
* 支持的 HTTP/1.1 特性有限。
* ...

写这篇文章，希望对自己和初学者都有所帮助。

