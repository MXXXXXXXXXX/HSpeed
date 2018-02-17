//
// Latest edit by Mxxxxx on 2018/1/23.
//

#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

void exch(hs_pq_t *hs_pq, size_t i, size_t j){
    void *tmp = hs_pq->pq[i];
    hs_pq->pq[i] = hs_pq->pq[j];
    hs_pq->pq[j] = tmp;
}

void swim(hs_pq_t *hs_pq, size_t k){
    while (k > 1 && hs_pq->comp(hs_pq->pq[k], hs_pq->pq[k/2])){
        exch(hs_pq, k, k/2);
        k /= 2;
    }
}

int sink(hs_pq_t *hs_pq, size_t k){
    size_t j;
    size_t nalloc = hs_pq->nalloc;
    while((k << 1) <= nalloc){
        j = k << 1;
        if((j < nalloc) && (hs_pq->comp(hs_pq->pq[j+1], hs_pq->pq[j])))
            j++;

        if(!hs_pq->comp(hs_pq->pq[j], hs_pq->pq[k]))
            break;

        exch(hs_pq, j, k);
        k = j;
    }
    return k;
}

int hs_pq_sink(hs_pq_t *hs_pq, size_t i){
    return sink(hs_pq, i);
}

int hs_pq_init(hs_pq_t *hs_pq, hs_pq_comparator_pt comp, size_t size){
    // 为hs_pq_t节点的pq分配(void *)指针
    hs_pq->pq = (void **)malloc(sizeof(void *) * (size + 1));
    if (!hs_pq->pq)
        return -1;

    hs_pq->nalloc = 0;
    hs_pq->size = size + 1;
    hs_pq->comp = comp;
    return 0;
}

int hs_pq_is_empty(hs_pq_t *hs_pq){
    // 通过nalloc值款快速判断是否为空
    return (hs_pq->nalloc == 0) ? 1 : 0;
}

size_t hs_pq_size(hs_pq_t *hs_pq){
    // 获取优先队列大小
    return hs_pq->nalloc;
}

void *hs_pq_min(hs_pq_t *hs_pq){
    // 优先队列最小值直接返回第一个元素（指针）
    if (hs_pq_is_empty(hs_pq))
        return (void *)(-1);

    return hs_pq->pq[1];
}


int resize(hs_pq_t *hs_pq, size_t new_size){
    if(new_size <= hs_pq->nalloc)
        return -1;

    void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
    if(!new_ptr)
        return -1;
    // 将原本nalloc + 1个元素值拷贝到new_ptr指向的位置
    memcpy(new_ptr, hs_pq->pq, sizeof(void *) * (hs_pq->nalloc + 1));
    // 释放旧元素
    free(hs_pq->pq);
    // 重新改写优先队列元素pq指针为new_ptr
    hs_pq->pq = new_ptr;
    hs_pq->size = new_size;
    return 0;
}

int hs_pq_delmin(hs_pq_t *hs_pq){
    if(hs_pq_is_empty(hs_pq))
        return 0;

    exch(hs_pq, 1, hs_pq->nalloc);
    --hs_pq->nalloc;
    sink(hs_pq, 1);
    if((hs_pq->nalloc > 0) && (hs_pq->nalloc <= (hs_pq->size - 1)/4)){
        if(resize(hs_pq, hs_pq->size / 2) < 0)
            return -1;
    }
    return 0;
}

int hs_pq_insert(hs_pq_t *hs_pq, void *item){
    if(hs_pq->nalloc + 1 == hs_pq->size){
        if(resize(hs_pq, hs_pq->size * 2) < 0){
            return -1;
        }
    }
    hs_pq->pq[++hs_pq->nalloc] = item;
    swim(hs_pq, hs_pq->nalloc);
    return 0;
}



