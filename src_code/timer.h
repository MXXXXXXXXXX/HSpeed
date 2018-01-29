//
// Latest edit by Mxxxxx on 2018/1/23.
//

#ifndef HS_TIMER_H
#define HS_TIMER_H

#include "priority_queue.h"
#include "http_request.h"

#define TIMEOUT_DEFAULT 500     /* ms */

// 函数指针，负责超时处理，hs_add_timer时指定处理函数
typedef int (*timer_handler_pt)(hs_http_request_t* request);

typedef struct hs_timer{
    size_t key;    // 标记超时时间
    int deleted;    // 标记是否被删除
    timer_handler_pt handler;    // 超时处理，add时指定
	hs_http_request_t* request;    // 指向对应的request请求
} hs_timer_t;

// hs_pq_t定义在"priority_queue.h"中，优先队列中节点
extern hs_pq_t hs_timer;
extern size_t hs_current_msec;

int hs_timer_init();
int hs_find_timer();
void hs_handle_expire_timers();
void hs_add_timer(hs_http_request_t* request, size_t timeout, timer_handler_pt handler);
void hs_del_timer(hs_http_request_t* request);
int timer_comp(void *ti, void *tj);

#endif