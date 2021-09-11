#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */

static inline void swap(void *a, void *b, size_t size) {
    char tmp, *x = (char *)a, *y = (char *)b;
    do {
        tmp = *x;
        *x++ = *y;
        *y++ = tmp;
    } while (--size > 0);
}

/* swap elements at index i, j*/
#define _SWAP(i, j)  swap((char *)a + (i) * es, (char *)a + (j) *es, es)

/* cmp elements witch at index i, j */
#define _CMP(i, j) cmp((char *)a + (i)*es, (char *)a + (j) *es)



static inline void shift_down(void *a, size_t n, size_t k, size_t es, int (*cmp)(const void *, const void *)) {
    size_t j;
    while (2 * k + 1 < n) {
        j = 2 * k + 1;
        if (j + 1 < n && _CMP(j + 1, j) > 0)
            j += 1;
        if (_CMP(k, j) >= 0)
            break;
        _SWAP(k, j);
        k = j;
    }
}


void heap_sort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    int i;
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    /* heapify -> max heap */
    for (i = (n - 1) / 2; i >= 0; i--) {
        shift_down(a, n, i, es, cmp);
    }

    for (i = n - 1; i > 0; i--) {
        _SWAP(0, i);
        shift_down(a, i, 0, es, cmp);
    }
}

/**
 * Time Complexity: O(N*lgN)
 * Auxiliary space: O(1)
 * Stability: unstatble.
 */


#if 0
/*
 * A fast, small, non-recursive O(nlog n) sort for the Linux kernel
 *
 * Jan 23 2005  Matt Mackall <mpm@selenic.com>
 */
void heap_sort  (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    /* pre-scale counters for performance */
    int i = (n/2 - 1) * es, bytes = n * es, c, r;


    /* heapify */
    for ( ; i >= 0; i -= es) {
        for (r = i; r * 2 + es < bytes; r  = c) {
            c = r * 2 + es;
            if (c < bytes - es && cmp(a + c, a + c + es) < 0)
                c += es;
            if (cmp(a + r, a + c) >= 0)
                break;
            swap(a + r, a + c, es);
        }
    }

    /* sort */
    for (i = bytes - es; i > 0; i -= es) {
        swap(a, a + i, es);
        for (r = 0; r * 2 + es < i; r = c) {
            c = r * 2 + es;
            if (c < i - es && cmp(a + c, a + c + es) < 0)
                c += es;
            if (cmp(a + r, a + c) >= 0)
                break;
            swap(a + r, a + c, es);
        }
    }
}
#endif   






