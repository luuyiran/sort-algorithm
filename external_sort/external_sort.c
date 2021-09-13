#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "external_sort.h"


/****************************** heap ********************************************/

#define HEAP_NCREMENT       (64) 

#define HEAP_AT(i)   ((char *)(hp->data) + (i) * hp->es)
#define _SWAP(i, j)  swap(HEAP_AT(i), HEAP_AT(j), hp->es)
#define _CMP(i, j)   (hp->cmp(HEAP_AT(i), HEAP_AT(j)))

typedef struct heap {
    void   *data;      // 存放数组
    size_t  n;         // 空间
    size_t  es;        // 每个元素大小
    size_t  size;      // 当前元素个数
    CMPFUNC cmp;
} heap;

static inline void swap(void *a, void *b, size_t size) {
    size_t BUF_LEN = 256;
    char buff[BUF_LEN];
    char *x = (char *)a, *y = (char *)b;
    do {
        size_t len = size < BUF_LEN ? size : BUF_LEN;
        memcpy(buff, x, len);
        memcpy(x, y, len);
        memcpy(y, buff, len);
        x += len;
        y += len;
        size -= len;
    } while (size);
}

static heap *heap_create(size_t n, size_t es, CMPFUNC cmp) {
    assert(n && es && cmp);
    heap *hp = (heap *)calloc(1, sizeof(heap));
    assert(hp);
    hp->data = malloc(n * es);
    assert(hp->data);
    hp->n = n;
    hp->es = es;
    hp->size = 0;
    hp->cmp = cmp;
    return hp;
}

static void heap_shift_up(heap *hp, size_t child) {
    size_t parent = (child - 1)/2;
    assert(hp);
    while (child > 0 && _CMP(parent, child) > 0) {
        _SWAP(parent, child);
        child = parent;
        parent = (child - 1)/2;
    }
}

static void heap_shift_down(heap *hp, size_t parent) {
    size_t child = parent * 2 + 1;

    while (child < hp->size) {
        // 找到最小的孩子，交换到父亲位置
        if (child + 1 < hp->size && _CMP(child + 1, child) < 0) 
            child += 1;
        if (_CMP(parent, child) <= 0)
            break;
        _SWAP(parent, child);
        parent = child;
        child = parent * 2 + 1;
    }
}

static void heap_push(heap *hp, void *a) {
    assert(hp && a);
    if (hp->size >= hp->n) {
        hp->data = realloc(hp->data, (hp->n + HEAP_NCREMENT) * hp->es);
        assert(hp->data);
        hp->n += HEAP_NCREMENT;
        printf("new: %d size: %ld capacity: %ld\n", HEAP_NCREMENT, hp->size, hp->n);
    }
    // add to tail 
    memcpy(HEAP_AT(hp->size), a, hp->es);
    heap_shift_up(hp, hp->size);
    hp->size++;
}

static void *heap_top(heap *hp) { 
    assert(hp);
    if (hp->size == 0)
        return NULL;
    else 
        return hp->data; 
}

static int heap_pop(heap *hp) {
    assert(hp);
    if (hp->size == 0) return -1;

    memcpy(HEAP_AT(0), HEAP_AT(hp->size - 1), hp->es);
    hp->size--;

    heap_shift_down(hp, 0);
    return 0;
}

static void heap_destroy(heap *hp) {
    free(hp->data);
    free(hp);
}

/************************************* external sort *******************************************/

static CMPFUNC iner_cmp;

typedef struct heap_pair {
    FILE *fp;
    char buf[0];
} heap_pair;

static inline void set_iner_cmp(CMPFUNC cmp) {
    iner_cmp = cmp;
}

static inline CMPFUNC get_iner_cmp() { 
    return iner_cmp; 
}

static int cmp_heap_node(const void *a, const void *b) {
   return get_iner_cmp()(((heap_pair *)a)->buf, ((heap_pair *)b)->buf);
}

static void dump_file(void *a, size_t len, size_t index) {
    char name[128] = {'\0'};
    snprintf(name, 128, "tmp%lu", index);
    FILE *fp = fopen(name, "w");
    assert(a && len && fp);
    if (1 != fwrite(a, len, 1, fp))
        printf("%s: %s failed.\n", __func__, name);
    fflush(fp);
    fclose(fp);
}

static void merge_files(SORT_CONFIG *config, size_t n) {
    size_t i, node_len;
    heap *hp = NULL;
    FILE *outfp = NULL;
    FILE **tmp = NULL;
    char name[128] = {'\0'};
    heap_pair *top, *buf;
    assert(config && n);

    set_iner_cmp(config->cmp);

    node_len = sizeof(heap_pair) + config->es;    // 堆节点的大小
    hp = heap_create(config->mem / config->es, node_len, cmp_heap_node);
    assert(hp);

    buf = malloc(node_len);
    assert(buf);

    tmp = (FILE **)malloc(n * sizeof(FILE *));
    for (i = 0; i < n; i++) {
        snprintf(name, 128, "tmp%lu", i);
        tmp[i] = fopen(name, "r");
        assert(tmp[i]);

        if (1 != fread(buf->buf, config->es, 1, tmp[i]))
            continue;

        buf->fp = tmp[i];
        heap_push(hp, buf);
    }

    assert(config->outputfile && config->outputfile[0]);
    outfp = fopen(config->outputfile, "w");
    assert(outfp);

    while (hp->size) {
        top = (heap_pair *)heap_top(hp);
        buf->fp = top->fp;

        config->write_to(outfp, top->buf);
        heap_pop(hp);

        if (1 != fread(buf->buf, config->es, 1, buf->fp))
            continue;

        heap_push(hp, buf);
    }

    for (i = 0; i < n; i++)
        fclose(tmp[i]);
    
    free(tmp);
    free(buf);
    heap_destroy(hp);

    if (system("rm -rf tmp*") < 0)
        printf("remove tmp file error!\n");
    fflush(outfp);
    fclose(outfp);
}

void external_sort(SORT_CONFIG *config) {
    FILE *infp = NULL;
    char *head, *buf;
    size_t i = 0, count, file_nums = 0;
    assert(config);
    count = config->mem / config->es;

    assert(config->inputfile && config->inputfile[0]);
    assert(config->outputfile && config->outputfile[0]);

    printf("every tmp file holds: %lu\n", count);

    head = (char *)malloc(config->mem);      // 排序数组，从文件中解析出数据，存进来
    assert(head);
    buf = head;

    infp = fopen(config->inputfile, "r");
    assert(infp);

    while (!feof(infp)) {
        config->read_from(infp, buf);
        i++;
        buf += config->es;
        if (i == count) {
            qsort(head, count, config->es, config->cmp);
            dump_file(head, count * config->es, file_nums);
            i = 0;
            buf = head;
            file_nums++;
        }
    }

    if (i) {
        qsort(head, i, config->es, config->cmp);
        dump_file(head, i * config->es, file_nums);
        file_nums++;
    }

    printf("created %lu tmp files!\n", file_nums);
    fclose(infp);
    free(head);
    merge_files(config, file_nums);
}

#if 0
/****************************** test code ********************************************/
static inline void test_heap() {
    const size_t COUNTS = 1024 * 1024;
    size_t *array = (size_t *)malloc(COUNTS * sizeof(size_t));
    assert(array);
    
    heap *hp = heap_create(COUNTS, sizeof(size_t), cmp_nums);
    assert(hp);

    srand(time(NULL));
    for (size_t i = 0; i < COUNTS; i++) {
        size_t num = COUNTS - i;
        heap_push(hp,&num);
    }
    
    size_t j = 0;
    while (hp->size) {
        size_t top = *((size_t *)heap_top(hp));
        array[j++] = top;
        heap_pop(hp);
    }

    for (j = 0; j < COUNTS - 1; j++) {
        if (array[j] > array[j + 1]) {
            printf("sort failure!\n");
            exit(-1);
        }
    }

    heap_destroy(hp);
    free(array);
}
#endif  

