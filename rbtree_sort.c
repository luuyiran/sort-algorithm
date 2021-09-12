#include <assert.h>     /* define assert */
#include <stddef.h>     /* define NULL */
#include <stdlib.h>
#include <string.h>


/* 获取自定义结构的地址 */
#define RB_TREE_ENTRY(ptr, type, member)                           \
        ((type *)((char *)ptr - (size_t)&((type *)0)->member))

typedef struct rb_node rb_node;

struct rb_node {
    rb_node *left;
    rb_node *right;
    rb_node *parent;
    _Bool color;
};

typedef struct NODE {
    void    *value;            /* value adress */
    size_t  count;             /* deal with duplicate elements */
    rb_node node;
} NODE;

static rb_node *rbt_first(rb_node *root);
static rb_node *rbt_next(rb_node *node);
static void rbt_insert(rb_node **root, rb_node *node);
static inline void rb_link_node(rb_node *parent, rb_node *node, rb_node **pos);

/* yes, sort by red black tree... */
void rbtree_sort (void *a, size_t n, size_t es, int (*cmp)(const void *, const void *)) {
    char repeat;
    size_t i, off = 0;
    NODE *this;
    rb_node *parent, **pos, *path, *root = NULL;
    void *bak = malloc(n * es);     /* cache to hold the sorted result */
    NODE *rb = (NODE *)calloc(n, sizeof(NODE));
    assert(a && bak && rb && cmp && es);

    /* insert to red black tree */
    for (i = 0; i < n; i++) {
        repeat = 0;
        rb[i].value = (char *)a + i * es;
        rb[i].count = 1;

        parent = NULL;
        pos = &root;
        while (*pos) {
            this = RB_TREE_ENTRY(*pos, NODE, node);
            parent = *pos;
            if (cmp(rb[i].value, this->value) < 0)
                pos = &((*pos)->left);
            else if (cmp(rb[i].value, this->value) > 0) 
                pos = &((*pos)->right);
            else {
                this->count++;
                repeat = 1;
                break;
            }
        }

        if (!repeat) {
            rb_link_node(parent, &rb[i].node, pos);
            rbt_insert(&root, &rb[i].node);
        }
    }

    /* Inorder Traversal */
    path = rbt_first(root);
    while (path) {
        this = RB_TREE_ENTRY(path, NODE, node);
        for (i = 0; i < this->count; i++) {
            memcpy((char *)bak + off, this->value, es);
            off += es;
        }
        path = rbt_next(path);
    }

    /* copy back */
    assert(off == n*es);
    memcpy(a, bak, off);

    free(rb);
    free(bak);
}

/**
 * Time Complexity: O(N*lgN)
 * Auxiliary space: O(N)
 * Stability: unstatble.
 */



/************************************** red black tree *********************************************/

#define RB_RED               (1u)
#define RB_BLACK             (0u)

#define rbt_is_red(node)     (RB_RED == (node)->color)
#define rbt_is_black(node)   (RB_BLACK == (node)->color)
#define rbt_set_red(node)    ((node)->color = RB_RED)
#define rbt_set_black(node)  ((node)->color = RB_BLACK)

/* 把 node 放在 parent 之后，放置位置在 pos */
static inline void rb_link_node(rb_node *parent, rb_node *node, rb_node **pos) {
    node->parent = parent;
    node->left = node->right = NULL;
    node->color = RB_RED;
    *pos = node;
}

/**
 *       parent                       parent
 *        |                            |
 *        y    Right Rotate (y)        x      
 *       / \   ----------------->     / \   
 *      x   T3                      T1   y        
 *     / \      <--------------         / \
 *   T1   T2     Left Rotation(x)     T2   T3
 */
static void rbtree_right_rotate(rb_node **root, rb_node *y) {
    rb_node *x = y->left;
    rb_node *T2= x->right;
    rb_node *parent = y->parent;

    x->right = y;
    y->parent = x;

    y->left = T2;
    if (T2) T2->parent = y;

    x->parent = parent;
    if (parent) {
        if (y == parent->right)
            parent->right = x;
        else
            parent->left = x;
    } else
        *root = x;
}

static void rbtree_left_rotate(rb_node **root, rb_node *x) {
    rb_node *y = x->right;
    rb_node *T2= y->left;
    rb_node *parent = x->parent;

    y->left = x;
    x->parent = y;

    x->right = T2;
    if (T2) T2->parent = x;

    y->parent = parent;
    if (parent) {
        if (x == parent->left) 
            parent->left = y;
        else 
            parent->right = y;
    } else
        *root = y;
}

static void rbt_insert(rb_node **root, rb_node *node) {
    rb_node *parent, *gparent;
    // node->color = RB_RED;
    while ((parent = node->parent) && rbt_is_red(parent)) {
        gparent = parent->parent;

        if (parent == gparent->left) {
            rb_node *uncle = gparent->right;
            /* case 1 */
            if (uncle && rbt_is_red(uncle)) {
                rbt_set_black(uncle);
                rbt_set_black(parent);
                rbt_set_red(gparent);
                node = gparent;
                continue;
            }
            /* case 2 */
            if (parent->right == node) {
                struct rb_node *tmp;
                rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }
            /* case 3 */
            rbt_set_black(parent);
            rbt_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        } else {
            rb_node *uncle = gparent->left;
            if (uncle && rbt_is_red(uncle)) {
                rbt_set_black(uncle);
                rbt_set_black(parent);
                rbt_set_red(gparent);
                node = gparent;
                continue;
            }

            if (parent->left == node) {
                rb_node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            rbt_set_black(parent);
            rbt_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }

    rbt_set_black(*root);
}

static rb_node *rbt_first(rb_node *root) {
    if (NULL == root) return NULL;
    while (root->left)
        root = root->left;
    return root;
}

static rb_node *rbt_next(rb_node *node) {
    rb_node *parent;
    if (node == NULL) return NULL;

    if (node->right) {
        node = node->right; 
        while (node->left)
            node=node->left;
        return node;
    }
    while ((parent = node->parent) && node == parent->right)
        node = parent;

    return parent;
}
