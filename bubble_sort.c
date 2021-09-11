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


void bubble_sort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char *end, *start;
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    for (end = (char *)a + (n - 1) * es; end > (char *)a; end -= es) {
        for (start = (char *)a; start < end; start += es) {
            if (cmp(start, start + es) > 0)
                swap(start, start + es, es);
        }
    }
}



/**
 * out for loop, 'end' will store the bigest element in range [start, end]
 * inner for loop, every element compare with its next element, push the bigest element to 'end'
 * 
 * Time Complexity: O(N^2)
 * Auxiliary space: O(1)
 * Stability: statble.
 */

/**
 * A sorting algorithm is said to be stable if two objects with equal keys 
 * appear in the same order in sorted output as they appear in the input array to be sorted.
*/