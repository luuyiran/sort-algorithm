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

void quick_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    size_t left_n;
    char *median, *left, *right;

    if (n <= 1) return;
    assert(a != NULL || es == 0);
    assert(cmp != NULL);

    left = (char *)a + es;
    right = (char *)a + (n - 1) * es;
    median = (char *) a + (n / 2) * es;     /* median as pivot */
    swap(a, median, es);                    /* put pivot to head */

    /* partition: left elements <= pivot */
    while (1) {
        while (left <= right && cmp(left, a) <= 0) left += es;
        while (left <= right && cmp(right, a) > 0) right -= es;
        if (left < right) {
            swap(left, right, es);
            left += es;
            right -= es;
        } else  break;
    }

    swap(a, right, es);  /* right now hold the pivot */

    left_n = (right - (char *)a) / es; /* pivot not go next recursion */

    quick_sort(a, left_n, es, cmp);
    /* right holds the pivot, pivot not go next recursion */
    quick_sort(right + es, n - left_n - 1, es, cmp);
}


/**
 * Time Complexity: O(N*lgN)
 * Auxiliary space: O(1)
 * Stability: unstatble.
 */



/**
 * the standard library
 * by P.J.Pauger
*/
void libc_qsort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    while (1 < n) {
        size_t i = 0;
        size_t j = n - 1;
        char *qi = (char *)a;
        char *qj = qi + es * j;
        char *qp = qj;

        while (i < j) {
            while (i < j && cmp(qi, qp) <= 0)
                ++i, qi += es;
            while (i < j && cmp(qp, qj) <= 0)
                --j, qj -= es;

            if (i < j) {
                swap(qi, qj, es);
                ++i, qi += es;
            }
        }

        if (qi != qp) {
            swap(qi, qp, es);
        }

        j = n - i -1, qi += es;

        if (j < i) {
            if (1 < j)
                quick_sort(qi, j, es, cmp);
            n = i;
        } else {
            if (1 < i) {
                quick_sort(a, i, es, cmp);
            }
            a = qi;
            n = j;
        }
    }
}


