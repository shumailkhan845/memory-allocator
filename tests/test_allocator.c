#include "allocator.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    int *a = my_malloc(sizeof(int));
    int *b = my_malloc(sizeof(int));

    assert(a != NULL);
    assert(b != NULL);

    *a = 42;
    *b = 99;

    printf("a: %p, value: %d\n", (void *)a, *a);
    printf("b: %p, value: %d\n", (void *)b, *b);

    my_free(a);

    int *c = my_malloc(sizeof(int));

    assert(c != NULL);

    printf("c: %p\n", (void *)c);

    /* First-fit search should reuse the freed block a. */
    assert(c == a);

    *c = 7;

    printf("Reused block value: %d\n", *c);
    printf("Memory reuse test passed.\n");

    my_free(b);
    my_free(c);

    return 0;
}