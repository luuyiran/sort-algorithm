#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */

/* swap elements which at index i, j*/
#define _SWAP(i, j)  swap((char *)a + (i) * es, (char *)a + (j) *es, es)

/* cmp elements witch at index i, j */
#define _CMP(i, j) cmp((char *)a + (i)*es, (char *)a + (j) *es)

static void swap(void *a, void *b, size_t size) {
    char tmp, *x = (char *)a, *y = (char *)b;
    do {
        tmp = *x;
        *x++ = *y;
        *y++ = tmp;
    } while (--size > 0);
}


void bubble_sort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

/*
    char *end, *start;
    for (end = (char *)a + (n - 1) * es; end > (char *)a; end -= es) {
        for (start = (char *)a; start < end; start += es) {
            if (cmp(start, start + es) > 0)
                swap(start, start + es, es);
        }
    }
*/
    size_t i, j;

    for (i = n - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (_CMP(j, j + 1) > 0) {
                _SWAP(j, j + 1);
            }
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