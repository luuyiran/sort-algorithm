#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>

#define COUNTS 100000u
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


extern void libc_qsort  (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void bsd_qsort   (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void bubble_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void heap_sort   (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void insert_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void insert_sort1(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void merge_sort  (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void quick_sort  (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void selec_sort  (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void shell_sort  (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void rbtree_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));
extern void comb_sort   (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));

typedef void (*SORT_TYPE)(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *));

typedef struct _FUNCTION_TYPES_ {
    const char *name;
    const SORT_TYPE func;
} FUNCTION;

static FUNCTION func[] = {
    {"bsd_qsort",   bsd_qsort},
    {"libc_qsort",  libc_qsort},
    {"quick_sort",  quick_sort},
    {"heap_sort",   heap_sort},
    {"merge_sort",  merge_sort},
    {"insert_sort", insert_sort},
    {"insert_sort1",insert_sort1},
    {"shell_sort",  shell_sort},
    {"selec_sort",  selec_sort},
    {"bubble_sort", bubble_sort},
    {"rbtree_sort", rbtree_sort},
    {"glibc_qsort", qsort},
    {"comb_sort", comb_sort}
};

typedef struct SORT_INFO {
    const char   *name;
    double        elapsed;
} SORT_INFO;

static int cmp_nums(const void *a, const void *b) {
   return ( *(size_t *)a - *(size_t *)b );
}

static int cmp_time(const void *a, const void *b) {
    return (((SORT_INFO *)a)->elapsed > ((SORT_INFO *)b)->elapsed) ?  1 : -1;
}


static int is_sorted(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    char *start = (char *)a;
    char *end = start + (n - 1) * es;
    while (start < end) {
        if (cmp(start, start + es) > 0)
            return 0;
        start += es;
    }
    return 1;
}

static void test_func(size_t *nums) {
    int i, n;
    struct timeval start, end;
    n = ARRAY_SIZE(func);

    SORT_INFO *table = (SORT_INFO *)malloc(n * sizeof(SORT_INFO));
    size_t *bak = (size_t *)malloc(COUNTS * sizeof(size_t));

    assert(nums && table && bak);
    memcpy(bak, nums, COUNTS * sizeof(size_t));

    for (i = 0; i < n; i++) {
        gettimeofday(&start, NULL);
        func[i].func(nums, COUNTS, sizeof(size_t), cmp_nums);
        gettimeofday(&end, NULL);

        table[i].name = func[i].name;
        table[i].elapsed = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.f;
        assert(is_sorted(nums, COUNTS, sizeof(size_t), cmp_nums));
        memcpy(nums, bak, COUNTS * sizeof(size_t));     /* recover */
    }

    /* rank by elapsed time */
    rbtree_sort(table, n, sizeof(SORT_INFO), cmp_time);
    for (i = 0; i < n; i++)
        printf("%s\t%.7f seconds\n", table[i].name, table[i].elapsed);

    free(table);
    free(bak);
}

/* generate a sorted array */
size_t *get_sorted_array(size_t *nums, size_t n) {
    size_t i;
    assert(nums && n);
    srand(time(NULL));
    for (i = 0; i < n; i++) 
        nums[i] = i;
    return nums;
}

/* generate random array */
size_t *get_raw_rand(size_t *nums, size_t n) {
    size_t i;
    assert(nums && n);
    srand(time(NULL));
    for (i = 0; i < n; i++) 
        nums[i] = rand();
    return nums;
}

/* 
 * generate a lot of repeated values,
 * distributed between [l - h) 
 */
size_t *get_range_rand(size_t *nums, size_t n, size_t l, size_t h) {
    size_t i;
    assert(nums && n);
    srand(time(NULL));
    for (i = 0; i < n; i++) 
        nums[i] = rand() % (h - l) + l ;
    return nums;
}


static inline void test2() {
    size_t *nums = (size_t *)malloc(COUNTS * sizeof(size_t));
    assert(nums);

    printf("sorted nums:\n");
    get_sorted_array(nums, COUNTS);
    test_func(nums);

    printf("\nrepeated nums:\n");
    get_range_rand(nums, COUNTS, 1, 66);
    test_func(nums);

    printf("\nrandom nums:\n");
    get_raw_rand(nums, COUNTS);
    test_func(nums);

    free(nums);
}

static void print(size_t *nums, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) 
        printf("%3lu ", nums[i]);
    printf("\n");
}

static void test1(FUNCTION sort) {
    size_t n = 16;
    size_t *nums = (size_t *)malloc(n * sizeof(size_t));
    assert(nums);
    get_range_rand(nums, n, 0, 100);

    printf(" ======== %s ========\n", sort.name);
    print(nums, n);

    sort.func(nums, n, sizeof(size_t), cmp_nums);
    assert(is_sorted(nums, n, sizeof(size_t), cmp_nums));

    print(nums, n);

    printf("\n");
    free(nums);
}

static inline void test0() {
    int i, n = ARRAY_SIZE(func);
    for (i = 0; i < n; i++)
        test1(func[i]);
}


int main() {

    test0();
    test2();

    return 0;
}


