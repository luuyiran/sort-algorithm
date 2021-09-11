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

void selec_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char *maxptr, *end, *start;
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    for (end = (char *)a + (n - 1) * es; end > (char *)a; end -= es) {
        maxptr = end;
        for (start = (char *)a; start < end; start += es) {
            if (cmp(start, maxptr) > 0)
                maxptr = start;
        }
        if (maxptr != end)
            swap(maxptr, end, es);

    }

}



/**
 * out for loop, 'end' will store the bigest element in range [start, end], like bubble sort.
 * inner for loop, find the bigest element's pointer,  then put it on 'end'  by swapping.
 * 
 * Time Complexity: O(N^2)
 * Auxiliary space: O(1)
 * Stability: unstatble.
 */

/**
 * A sorting algorithm is said to be stable if two objects with equal keys appear in the same order in sorted output as they appear in the input array to be sorted.
*/
