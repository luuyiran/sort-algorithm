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

void comb_sort(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char again = 1;
    size_t step = n, i, j;

    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    while ((step /= 1.3) > 1) {
        for (j = n - 1; j >= step; j--) {
            if (_CMP(j, j - step) < 0)
                _SWAP(j, j - step);
        }
    }

    for (i = 0; i < n - 1 && again; i++) {
        again = 0;
        for (j = n - 1; j > i; j--) {
            if(_CMP(j, j - 1) < 0) {
                _SWAP(j, j - 1);
                again = 1;
            }
        }
    }
}


/**
 * Comb sort improves on bubble sort in the same way that Shellsort improves on insertion sort.
 * it works better than Bubble Sort on average, worst case remains O(n2)
 * 
 * 第一阶段，对间隔 step 的元素进行冒泡排序，把最小元素推到前端
 * 第二阶段，执行冒泡排序，把小元素推到前端，内层循环如果已没有逆序元素，则排序结束。
*/


/* 一二截断可以合并，测试发现有少许性能损失 */
void comb_sort1(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char again = 1;
    size_t step = n, i;

    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);
 
    while (step != 1 || again == 1) {
        again = 0;
        step /= 1.3;
        if (step < 1) step = 1;

        for (i = 0; i < n - step; i++) {
            if (_CMP(i, i + step) > 0)  {
                _SWAP(i, i + step);
                again = 1;
            }
        }
    }
}




