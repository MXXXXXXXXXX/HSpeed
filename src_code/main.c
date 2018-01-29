//
// Latest edit by Mxxxxx on 2018/1/23.
//

// 系统头文件
#include <stdio.h>
#include "threadpool.h"
#include "http.h"

#define DEFAULT_CONFIG "hspeed.conf"

extern struct epoll_event *events;
char *conf_file = DEFAULT_CONFIG;
hs_conf_t conf;

int main(int argc, char *argv[]){
    // 读取配置文件
    read_conf(conf_file, &conf);

    // 处理SIGPIPE
    handle_for_sigpipe();

    // 初始化套接字开始监听
    int listen_fd = socket_bind_listen(conf.port);

    // 设置为socket非阻塞
    int rc = make_socket_non_blocking(listen_fd);

    // 创建epoll并注册监听描述符
    int epoll_fd = hs_epoll_create(0);
    hs_http_request_t* request = (hs_http_request_t*)malloc(sizeof(hs_http_request_t));
    hs_init_request_t(request, listen_fd, epoll_fd, conf.root);
    hs_epoll_add(epoll_fd, listen_fd, request, (EPOLLIN | EPOLLET));

    // 初始化线程池
	hs_threadpool_t *tp = threadpool_init(conf.thread_num);

    // 初始化计时器
	hs_timer_init();

    while(1){
        // 得到最近且未删除时间和当前时间差值（等待时间）
        int time = hs_find_timer();

        // 调用epoll_wait函数，返回接收到事件的数量
        int events_num = hs_epoll_wait(epoll_fd, events, MAXEVENTS, -1);

        // 处理已经超时的请求
		hs_handle_expire_timers();

        // 遍历events数组，根据监听种类及描述符类型分发操作
		hs_handle_events(epoll_fd, listen_fd, events, events_num, conf.root, tp);
    }

    // 回收线程资源
    // threadpool_destory(tp, graceful_shutdown);
}

