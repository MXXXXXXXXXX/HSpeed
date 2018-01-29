//
// Latest edit by Mxxxxx on 2018/1/23.
//

#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "http.h"
#include "threadpool.h"

#define MAXEVENTS 1024

int hs_epoll_create(int flags);
int hs_epoll_add(int epoll_fd, int fd, hs_http_request_t* request, int events);
int hs_epoll_mod(int epoll_fd, int fd, hs_http_request_t* request, int events);
int hs_epoll_del(int epoll_fd, int fd, hs_http_request_t* request, int events);
int hs_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout);
void hs_handle_events(int epoll_fd, int listen_fd, struct epoll_event* events,
                      int events_num, char* path, hs_threadpool_t* tp);

#endif
