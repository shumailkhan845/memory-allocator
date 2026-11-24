#include "allocator.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    printf("===== MEMORY ALLOCATOR DAY 4 TESTS =====\n\n");

    /* Test 1: Basic allocation */
    int *a = my_malloc(sizeof(int));

    assert(a != NULL);

    *a = 42;

    printf("Test 1: Basic allocation passed\n");
    printf("A address: %p\n", (void *)a);
    printf("A value: %d\n\n", *a);

    /* Test 2: Multiple allocations */
    int *b = my_malloc(sizeof(int));

    assert(b != NULL);
    assert(a != b);

    *b = 99;

    printf("Test 2: Multiple allocations passed\n");
    printf("B address: %p\n", (void *)b);
    printf("B value: %d\n\n", *b);

    /* Test 3: Free A */
    my_free(a);

    printf("Test 3: Free operation passed\n\n");

    /* Test 4: Reuse A */
    int *c = my_malloc(sizeof(int));

    assert(c != NULL);
    assert(c == a);

    *c = 7;

    printf("Test 4: Memory reuse passed\n");
    printf("A address: %p\n", (void *)a);
    printf("C address: %p\n", (void *)c);
    printf("C value: %d\n\n", *c);

    /* Test 5: Do not reuse an occupied block */
    int *d = my_malloc(sizeof(int));

    assert(d != NULL);
    assert(d != b);
    assert(d != c);

    printf("Test 5: Occupied block protection passed\n\n");

    /* Test 6: NULL handling */
    my_free(NULL);

    printf("Test 6: NULL handling passed\n\n");

    /* Test 7: Alignment */
    void *e = my_malloc(sizeof(max_align_t));
    void *f = my_malloc(1);

    assert(e != NULL);
    assert(f != NULL);

    assert((uintptr_t)e % _Alignof(max_align_t) == 0);
    assert((uintptr_t)f % _Alignof(max_align_t) == 0);

    printf("Test 7: Alignment passed\n\n");

    /* Cleanup */
    my_free(b);
    my_free(c);
    my_free(d);
    my_free(e);
    my_free(f);


    return 0;
}