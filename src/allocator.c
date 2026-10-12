#define _DEFAULT_SOURCE

#include "allocator.h"

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef union block block_t;

union block
{
    struct
    {
        size_t size;
        int free;
        block_t *next;
    } meta;

    max_align_t alignment;
};

static block_t *head = NULL;
static block_t *tail = NULL;

void *my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    /* 1. Search existing blocks for reusable memory. */
    block_t *current = head;

    while (current != NULL)
    {
        if (current->meta.free && current->meta.size >= size)
        {
            current->meta.free = 0;
            return (void *)(current + 1);
        }

        current = current->meta.next;
    }

    /* 2. Check for size overflow before requesting memory. */
    if (size > SIZE_MAX - sizeof(block_t))
        return NULL;

    size_t total_size = sizeof(block_t) + size;

    /* 3. Request a new block from the OS. */
    block_t *block = sbrk(total_size);

    if (block == (void *)-1)
        return NULL;

    block->meta.size = size;
    block->meta.free = 0;
    block->meta.next = NULL;

    /* 4. Append the block to our linked list. */
    if (head == NULL)
    {
        head = block;
        tail = block;
    }
    else
    {
        tail->meta.next = block;
        tail = block;
    }

    /* 5. Return the payload, not the header. */
    return (void *)(block + 1);
}

void my_free(void *ptr)
{
    if (ptr == NULL)
        return;

    /* Recover the header immediately before the payload. */
    block_t *block = (block_t *)ptr - 1;

    /* Make this block available for future allocations. */
    block->meta.free = 1;
}