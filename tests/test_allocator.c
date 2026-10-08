#include "allocator.h"

#include <stdio.h>

int main(void)
{
    int *p = my_malloc(sizeof(int));

    if (p == NULL)
    {
        printf("Allocation failed\n");
        return 1;
    }

    *p = 42;

    printf("Value: %d\n", *p);
    printf("Address: %p\n", (void *)p);

    return 0;
}