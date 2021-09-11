#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */

static void swap(void *a, void *b, size_t size) {
    char tmp, *x = (char *)a, *y = (char *)b;
    do {
        tmp = *x;
        *x++ = *y;
        *y++ = tmp;
    } while (--size > 0);
}

void shell_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    size_t gap;
    char *insertptr, *pos;
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    for (gap = n >> 1; gap > 0; gap >>= 1) {
        for (insertptr = (char *) a + gap * es; insertptr < (char *) a + n * es; insertptr += gap*es)
            for (pos = insertptr; pos > (char *) a && cmp(pos - gap*es, pos) > 0; pos -= gap*es)
                swap(pos, pos - gap*es, es);
    }
}

/**
 * ShellSort is mainly a variation of Insertion Sort.
 * 
 * Time Complexity: O(N^2)
 * Auxiliary space: O(1)
 * Stability: unstatble.
 */


