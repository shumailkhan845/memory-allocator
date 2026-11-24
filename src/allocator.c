
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

#define ALIGNMENT _Alignof(max_align_t)

static block_t *head = NULL;
static block_t *tail = NULL;

static size_t align_size(size_t size)
{
    size_t remainder = size % ALIGNMENT;

    if (remainder == 0)
        return size;

    return size + (ALIGNMENT - remainder);
}

void *my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    if (size > SIZE_MAX - (ALIGNMENT - 1))
        return NULL;

    size_t aligned_size = align_size(size);

    /* First-fit search */
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

    if (aligned_size > SIZE_MAX - sizeof(block_t) - (ALIGNMENT - 1))
        return NULL;

    size_t padding = 0;
    void *program_break = sbrk(0);

    if (program_break == (void *)-1)
        return NULL;

    uintptr_t address = (uintptr_t)program_break;
    size_t remainder = address % ALIGNMENT;

    if (remainder != 0)
        padding = ALIGNMENT - remainder;

    size_t total_size = padding + sizeof(block_t) + aligned_size;

    void *allocation = sbrk(total_size);

    if (allocation == (void *)-1)
        return NULL;

    block_t *block = (block_t *)((char *)allocation + padding);

    block->meta.size = aligned_size;
    block->meta.free = 0;
    block->meta.next = NULL;

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

    return (void *)(block + 1);
}

void my_free(void *ptr)
{
    if (ptr == NULL)
        return;

    block_t *block = (block_t *)ptr - 1;

    block->meta.free = 1;
}
