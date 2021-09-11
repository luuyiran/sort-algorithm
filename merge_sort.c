#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */
#include <string.h>     /* function memmove memcpy */
#include <stdlib.h>     /* function malloc */

typedef int (*CMPFUNC)(const void *, const void *);


static void merge(char *left, char *right, char *bak, size_t n, size_t es, CMPFUNC cmp) {
    int off = 0;
    char *head = left;
    char *left_end = right - es;
    char *right_end = left + (n - 1) * es;

    while (left <= left_end && right <= right_end) {
        if (cmp(left, right) <= 0) {
            memcpy(bak + off, left, es);
            left += es;
        } else {
            memcpy(bak + off, right, es);
            right += es;
        }
        off += es;
    }

    if (left <= left_end) 
        memcpy(bak + off, left, left_end + es - left);

    if (right <= right_end) 
        memcpy(bak + off, right, right_end + es - right);

    memcpy(head, bak, n * es);
}


static void mergeSort(char *left, char *bak, size_t n, size_t es, CMPFUNC cmp) {
    char *right;
    if (n <= 1) return;

    right = left + (n >> 1) * es;
    mergeSort(left, bak, n >> 1, es, cmp);
    mergeSort(right, bak, n - (n >> 1), es, cmp);
    merge(left, right, bak, n, es, cmp);
}

void merge_sort (void *a, size_t n, size_t es, CMPFUNC cmp) {
    char *bak = (char *)malloc(n * es);
    assert(bak && a && cmp && n && es);
    mergeSort((char *)a, bak, n, es, cmp);
    free(bak);
}



/**
 * Time Complexity: O(N*lgN)
 * Auxiliary space: O(N)
 * Stability: statble.
 */



