#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */
#include <string.h>     /* function memmove memcpy */
#include <stdlib.h>     /* function malloc */


static void swap(void *a, void *b, size_t size) {
    char tmp, *x = (char *)a, *y = (char *)b;
    do {
        tmp = *x;
        *x++ = *y;
        *y++ = tmp;
    } while (--size > 0);
}

/* first version */
void insert_sort1(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char *insertptr, *pos;
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    for (insertptr = (char *) a + es; insertptr < (char *) a + n * es; insertptr += es)
        for (pos = insertptr; pos > (char *) a && cmp(pos - es, pos) > 0; pos -= es)
            swap(pos, pos - es, es);
}


/* use memmove to speedup */
void insert_sort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char *insertptr, *pos, *buff, *target;
    buff = (char *)malloc(es);
    assert(a != NULL || buff != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    for (insertptr = (char *) a + es; insertptr < (char *) a + n * es; insertptr += es) {
        target = insertptr;
        for (pos = insertptr - es; pos >= (char *)a; pos -= es) {
            if (cmp(pos, insertptr) > 0)
                target = pos;
            else break;
        }
        if (target != insertptr) {
            memcpy(buff, insertptr, es);
            memmove(target + es, target, insertptr - target);
            memcpy(target, buff, es);
        }
    }

    free(buff);
}


/**
 * insert_sort1:
 * out for loop, the new elemnt we are going to insert is 'insertptr', we look position before it.
 * inner for loop, before insertptr's elments have been sorted, all the elements that bigger than insertptr will move to next position.
 * 
 * 
 * insert_sort:         optimized version
 * memmove all the elements larger than 'insertptr' to right,
 * then copy 'insertptr' to target.
 * 
 * 
 * if inputs is a almost sorted array it will cost O(N) time complexity, inner cmp always failure.
 * 
 * Time Complexity: O(N^2)
 * Auxiliary space: O(1)
 * Stability: statble.
 */




