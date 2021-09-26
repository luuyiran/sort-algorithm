#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */
#include <stdlib.h>     /* calloc malloc */
#include <string.h>     /* memcpy */

#define SKIPLIST_MAXLEVEL   32

typedef struct NODE {
    const void  *value;            /* value adress */
    size_t      count;             /* deal with duplicate elements */
    struct NODE *next[];
} NODE;

static inline NODE *createNode(const void *v, int level) {
    NODE *res = (NODE *)calloc(1, sizeof(NODE) + level * sizeof(NODE *));
    assert(res);
    if (v) {
        res->value = v;
        res->count = 1;
    }
    return res;
}

static inline int randomLevel(void) {
    int level = 1;
    /* 16383 = 65535 >> 2;  only 1/4 random nums less than 16383 */
    while ((rand() & 65535) < 16383) level += 1;
    return level < SKIPLIST_MAXLEVEL ? level : SKIPLIST_MAXLEVEL;
}

/* just for fun */
void skplst_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    void *bak, *value;
    size_t j, off;
    int i, lvl, levels = 0;
    NODE *head, *node, *pre, *next, *update[SKIPLIST_MAXLEVEL];

    assert(a != NULL || n == 0 || es == 0);
    assert(cmp != NULL);

    head = createNode(NULL, SKIPLIST_MAXLEVEL);
    bak = malloc(n * es);     /* cache to hold the sorted result */
    assert(head && bak);

    for (j = 0; j < n; j++) {
        value = (char *)a + j * es;

        next = NULL;
        node = head;
        /* update[i] record previous path */
        for (i = levels - 1; i >= 0; i--) {
            while ((next = node->next[i]) && cmp(next->value, value) < 0 ) {
                node = next;
            }
            update[i] = node;
        }

        /* if already in list, update count */
        if (next && cmp(next->value, value) == 0) {
            next->count++;
            continue;
        }

        lvl = randomLevel();
        if (lvl > levels) {
            for (i = levels; i < lvl; i++)
                update[i] = head;
            levels = lvl;
        }

        node = createNode(value, lvl);
        for (i = 0; i < lvl; i++) {
            node->next[i] = update[i]->next[i];
            update[i]->next[i] = node;
        }

    }

    off = 0;
    pre = head;
    while (pre) {
        /* copy to bak buf */
        for (j = 0; j < pre->count; j++) {
            memcpy((char *)bak + off, pre->value, es);
            off += es;
        }
        /* free node */
        head = pre->next[0];
        free(pre);
        pre = head;
    }

    /* copy back */
    assert(off == n*es);
    memcpy(a, bak, off);

    free(bak);
}









