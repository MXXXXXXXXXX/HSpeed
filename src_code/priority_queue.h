//
// Latest edit by Mxxxxx on 2018/1/23.
//

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

#define HS_PQ_DEFAULT_SIZE 10

typedef int (*hs_pq_comparator_pt)(void *pi, void *pj);

typedef struct priority_queue{
    void **pq;
    size_t nalloc;
    size_t size;
	hs_pq_comparator_pt comp;
}hs_pq_t;

int hs_pq_init(hs_pq_t *hs_pq, hs_pq_comparator_pt comp, size_t size);
int hs_pq_is_empty(hs_pq_t *hs_pq);
size_t hs_pq_size(hs_pq_t *hs_pq);
void *hs_pq_min(hs_pq_t *hs_pq);
int hs_pq_delmin(hs_pq_t *hs_pq);
int hs_pq_insert(hs_pq_t *hs_pq, void *item);
int hs_pq_sink(hs_pq_t *hs_pq, size_t i);

#endif 
