//This is the block of mermoy that will be given by the my_malloc

#include "allocator.h"

#include <unistd.h>

typedef struct block {
    size_t size; // how much data?
    int free; // is it free or not ?
    struct block *next; // next data payload same as linkedlist
} block_t;


void *my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size_t total_size = sizeof(block_t) + size;

    block_t *block = sbrk(total_size);

    if (block == (void *)-1)
        return NULL;

    block->size = size;
    block->free = 0;
    block->next = NULL;

    return (void *)(block + 1);
}