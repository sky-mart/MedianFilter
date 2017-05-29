//
//  median_filter.c
//  MedianFilter
//
//  Created by Влад Агиевич on 18.05.17.
//  Copyright © 2017 Mocsmart. All rights reserved.
//

#include "median_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ARRAY_SIZE      4000
#define MAX_WINDOW_SIZE    64

#define LEFT(x)     (2*(x)+1)
#define RIGHT(x)    (2*(x)+2)


bst_node tree[MAX_WINDOW_SIZE+10];
size_t free_node = 0;


data make_data(int val, size_t aix)
{
    data new = {val, aix};
    return new;
}

void array_print(int *a, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%i ", a[i]);
    }
}

void array_println(int *a, size_t n)
{
    array_print(a, n);
    printf("\n");
}

//void bst_print(int *t, size_t n)
//{
//    size_t nElements = 1;
//    size_t i = 0, j;
//    
//    while (i < n) {
//        for (j = i; j < i + nElements; j++) {
//            printf("%i ", t[j]);
//        }
//        printf("\n");
//        
//        i += nElements;
//        nElements <<= 1;
//    }
//}
//
//void bst_println(int *t, size_t n)
//{
//    bst_print(t, n);
//    printf("\n");
//}

void bst_print(bst_node *root)
{
    bst_node **parents, **children;
    size_t nNodes = 1;
    parents = &root;
    while ( (children = bst_print_aux(parents, nNodes)) ) {
        if (nNodes > 1)
            free(parents);
        parents = children;
        nNodes *= 2;
    }
    
}

bst_node **bst_print_aux(bst_node **parents, size_t n)
{
    size_t i;
    uint8_t last_level = 1;
    bst_node **children = (bst_node **)malloc(n * 2 * sizeof(bst_node *));
    for (i = 0; i < n; i++) {
        if (parents[i]) {
            printf("%i ", parents[i]->key);
            children[2*i] = parents[i]->l;
            children[2*i+1] = parents[i]->r;
            if (children[2*i] || children[2*i+1])
                last_level = 0;
        } else {
            printf("* ");
            children[2*i] = NULL;
            children[2*i+1] = NULL;
        }
    }
    printf("\n");
    if (last_level)
        return NULL;
    return children;
}

void bst_node_init(bst_node *node, int key, bst_node *p, bst_node *l, bst_node *r)
{
    node->key = key;
    node->p = p;
    node->l = l;
    node->r = r;
}

bst_node **bst_free_aux(bst_node **parents, size_t n)
{
    size_t i;
    uint8_t last_level = 1;
    bst_node **children = (bst_node **)malloc(n * 2 * sizeof(bst_node *));
    for (i = 0; i < n; i++) {
        if (parents[i]) {
            children[2*i] = parents[i]->l;
            children[2*i+1] = parents[i]->r;
            free(parents[i]);
            if (children[2*i] || children[2*i+1])
                last_level = 0;
        } else {
            children[2*i] = NULL;
            children[2*i+1] = NULL;
        }
    }
    if (last_level)
        return NULL;
    return children;
}

void bst_free(bst_node *root)
{
    bst_node **parents, **children;
    size_t nNodes = 1;
    parents = &root;
    while ( (children = bst_free_aux(parents, nNodes)) ) {
        if (nNodes > 1)
            free(parents);
        parents = children;
        nNodes *= 2;
    }
}

size_t bst_leftlen(bst_node *root)
{
    bst_node *left;
    size_t len = 0;
    
    left = root->l;
    
    while (left) {
        len++;
        left = left->l;
    }
    return len;
}

size_t bst_rightlen(bst_node *root)
{
    bst_node *right;
    size_t len = 0;
    
    right = root->r;
    
    while (right) {
        len++;
        right = right->r;
    }
    return len;
}

bst_node *bst_leftest_node(bst_node *root)
{
    bst_node *node = root;

    while (node->l) {
        node = node->l;
    }
    return node;
}

bst_node *bst_rightest_node(bst_node *root)
{
    bst_node *node = root;
    
    while (node->r) {
        node = node->r;
    }
    return node;
}

bst_node *bst_new_node(int key)
{
    //bst_node *new = (bst_node *)malloc(sizeof(bst_node));
    bst_node *new = &tree[free_node];
    free_node++;
    new->key = key;
    new->l = NULL;
    new->r = NULL;
    new->p = NULL;
    return new;
}

void bst_attach_left(bst_node *parent, int key)
{
    bst_node *new = bst_new_node(key);
    parent->l = new;
    new->p = parent;
}

void bst_attach_right(bst_node *parent, int key)
{
    bst_node *new = bst_new_node(key);
    parent->r = new;
    new->p = parent;
}

void bst_root2left(bst_node *root)
{
    if (root->l) {
        bst_node *left_rightest = bst_rightest_node(root->l);
        bst_attach_right(left_rightest, root->key);                 // alloc
    } else {
        root->l = bst_new_node(root->key);                          // alloc
        root->l->p = root;
    }
    root->key = -1;
}

void bst_root2right(bst_node *root)
{
    if (root->r) {
        bst_node *right_leftest = bst_leftest_node(root->r);
        bst_attach_left(right_leftest, root->key);                  // alloc
    } else {
        root->r = bst_new_node(root->key);                          // alloc
        root->r->p = root;
    }
    root->key = -1;
}

void bst_left2root(bst_node *root)
{
    bst_node *left_rightest = bst_rightest_node(root->l);
    root->key = left_rightest->key;
    bst_remove_node(left_rightest);                                 // free
}

void bst_right2root(bst_node *root)
{
    bst_node *right_leftest = bst_leftest_node(root->r);
    root->key = right_leftest->key;
    bst_remove_node(right_leftest);                                 // free
}

void bst_insert(bst_node *root, int key)
{
    bst_node *p = root; // родитель
    bst_node *gp;       // дед
    int tmp;
    
    while (1) {
        gp = p->p;
        if (key < p->key) {
            if (p->l) {
                p = p->l;
            } else {
                // проверка на лысые ветки
                if (!p->r && gp) {   // у родителя нет других детей
                    // цепляемся не к корню
                    
                    if (gp->l == p) {    // родитель - левый ребенок у деда
                        if (!gp->r) {       // у деда 1 ребенок
                            tmp         = gp->key;
                            gp->key     = p->key;
                            p->key      = key;
                            bst_attach_right(gp, tmp);
                            return;
                        }
                    } else {                //  родитель - правый ребенок у деда
                        if (!gp->l) {       // у деда 1 ребенок
                            if (key > gp->key) {
                                // только правый ребенок может быть равен родителю
                                tmp     = gp->key;
                                gp->key = key;
                                bst_attach_left(gp, tmp);
                                return;
                            }
                        }
                    }
                }
                bst_attach_left(p, key);
                return;
            }
        } else {
            if (p->r) {
                p = p->r;
            } else {
                // проверка на лысые ветки
                if (!p->l && gp) {      // у родителя нет других детей
                                        // цепляемся не к корню
                    
                    if (gp->l == p) {    // родитель - левый ребенок у деда
                        if (!gp->r) {    // у деда 1 ребенок
                            if (key > p->key) {
                                tmp         = gp->key;
                                gp->key     = key;
                                bst_attach_right(gp, tmp);
                                return;
                            }
                        }
                    } else {                //  родитель - правый ребенок у деда
                        if (!gp->l) {       // у деда 1 ребенок
                            if (p->key > gp->key) {
                                tmp         = gp->key;
                                gp->key     = p->key;
                                p->key      = key;
                                bst_attach_left(gp, tmp);
                                return;
                            }
                        }
                    }
                }
                bst_attach_right(p, key);
                return;
            }
        }
    }
}


void bst_remove_parent_link(bst_node *node)
{
    if (node->p) {
        if (node->p->l == node)
            node->p->l = NULL;
        else
            node->p->r = NULL;
    }
}

void bst_remove_node(bst_node *node)
{
    bst_node *m;
    if (!node->l && !node->r) {
        bst_remove_parent_link(node);
        //free(node);
        free_node = node - tree;
        return;
    }
    if (!node->l) {
        node->r->p = node->p;
        if (node->p) {
            if (node->p->l == node) {
                node->p->l = node->r;
            } else {
                node->p->r = node->r;
            }
        }
        //free(node);
        free_node = node - tree;
        return;
    }
    if (!node->r) {
        node->l->p = node->p;
        if (node->p) {
            if (node->p->l == node) {
                node->p->l = node->l;
            } else {
                node->p->r = node->l;
            }
        }
        //free(node);
        free_node = node - tree;
        return;
    }
    if (!node->r->l) {
        node->key = node->r->key;
        m = node->r->r;
        //free(node->r);
        free_node = node->r - tree;
        node->r = m;
        if (m) {
            m->p = node;
        }
        return;
    }
    bst_right2root(node);
}

void bst_remove_key(bst_node *root, int key)
{
    bst_node *p = root;
    while (1) {
        if (key < p->key) {
            p = p->l;
            //bst_remove_key(root->l, key);
        } else if (key > p->key) {
            p = p->r;
            //bst_remove_key(root->r, key);
        } else {
            bst_remove_node(p);
            return;
        }
    }
}

void mf_step(bst_node *root, int oldkey, int newkey)
{
    bst_node *left_rightest, *right_leftest;
    
    if (oldkey != root->key) {
        bst_remove_key(root, oldkey);                           // free
        
        if (oldkey < root->key) {
            if (newkey < root->key) {
                bst_insert(root, newkey);                       // alloc
            } else {
                right_leftest = bst_leftest_node(root->r);
                bst_root2left(root);                            // alloc
                if (newkey >= right_leftest->key) {
                    root->key = right_leftest->key;
                    bst_remove_node(right_leftest);             // free
                    bst_insert(root, newkey);                   // alloc
                } else {
                    root->key = newkey;
                }
            }
        } else {
            if (newkey >= root->key) {
                bst_insert(root, newkey);                       // alloc
            } else {
                left_rightest = bst_rightest_node(root->l);
                bst_root2right(root);                           // alloc
                if (newkey < left_rightest->key) {
                    root->key = left_rightest->key;
                    bst_remove_node(left_rightest);             // free
                    bst_insert(root, newkey);                   // alloc
                } else {
                    root->key = newkey;
                }
            }
        }
    } else {
        left_rightest = bst_rightest_node(root->l);
        if (newkey < left_rightest->key) {
            root->key = left_rightest->key;
            bst_remove_node(left_rightest);                     // free
            bst_insert(root, newkey);                           // alloc
        } else {
            right_leftest = bst_leftest_node(root->r);
            if (newkey >= right_leftest->key) {
                root->key = right_leftest->key;
                bst_remove_node(right_leftest);                 // free
                bst_insert(root, newkey);                       // alloc
            } else {
                root->key = newkey;
            }
        }
    }
}

//void bststr(struct bst_node *root, char **pStr, size_t len)
//{
//}

void swap(int *a, int *b)
{
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

size_t pow2(size_t sa_len)
{
    size_t p2 = 1;
    while (p2 <= sa_len) {
        p2 <<= 1;
    }
    return p2;
}

// sorted array to balanced binary search bst
void sa2bbst(int *sa, size_t start, size_t end, bst_node *node)
{
    size_t mid;    
    if (start >= end) {
        return;
    }

    mid = (start + end) / 2;
    node->key = sa[mid];
    
    if (start < mid) {
        node->l = bst_new_node(-1); //(bst_node *)malloc(sizeof(bst_node));
        node->l->p = node;
        sa2bbst(sa, start, mid, node->l);
    } else {
        node->l = NULL;
    }
    if (mid + 1 < end) {
        node->r = bst_new_node(-1); //(bst_node *)malloc(sizeof(bst_node));
        node->r->p = node;
        sa2bbst(sa, mid+1, end, node->r);
    } else {
        node->r = NULL;
    }
}

int cmpint(const void *a, const void *b)
{
    return ( *(int *)a - *(int *)b );
}

// TODO:
// ---1) Замеры времени
// ---2) Переход от рекурсии к циклу в операции вставки
// ---3) Переход от рекурсии к циклу в операции удаления
// ---4) Уход от динамической памяти

void mf_bst(int *input, size_t n, size_t r, int *output)
{
    bst_node *root;
    int sa[MAX_WINDOW_SIZE];
    size_t i;
    
    memcpy(sa, input, r * sizeof(int));
    qsort(sa, r, sizeof(int), cmpint);
    
    root = bst_new_node(input[0]);
    
    sa2bbst(sa, 0, r, root);
    
//    array_println(input, n);
//    printf("\n");
//    bst_print(root);
//    printf("\n");
    
    output[0] = root->key;
    for (i = 1; i < n-r+1; i++) {
        mf_step(root, input[i-1], input[i+r-1]);
        output[i] = root->key;
//        bst_print(root);
//        printf("\n");
    }
//    bst_free(root);
    free_node = 0;
}

void mf_direct(int *input, size_t n, size_t r, int *output)
{
    int sa[MAX_WINDOW_SIZE];
    size_t i;
    
    for (i = 0; i < n-r+1; i++) {
        memcpy(sa, &input[i], r*sizeof(int));
        qsort(sa, r, sizeof(int), cmpint);
        output[i] = sa[r/2];
    }
}

uint8_t test_mf_bst(size_t array_size, size_t window_size)
{
    int a[MAX_ARRAY_SIZE];
    int out_bst[MAX_ARRAY_SIZE];
    int out_dir[MAX_ARRAY_SIZE];
    
    size_t i;
    for (i = 0; i < array_size; i++) {
        a[i] = rand();
    }
    
    mf_bst(a, array_size, window_size, out_bst);
    mf_direct(a, array_size, window_size, out_dir);
    
    for (i = 0; i < array_size - window_size + 1; i++) {
        if (out_bst[i] != out_dir[i])
            return 0;
    }
    return 1;
}

uint8_t testset_mf_bst()
{
    size_t array_size, window_size;
    for (array_size = MAX_WINDOW_SIZE; array_size < 301; array_size++) {
        for (window_size = 3; window_size < MAX_WINDOW_SIZE; window_size++) {
            if (!test_mf_bst(array_size, window_size)) {
                printf("array_size=%zu, window_size=%zu: FAIL\n", array_size, window_size);
                return 0;
            }
            
        }
        if (array_size % 100 == 0) {
            printf("array_size<=%zu: SUCCESS\n", array_size);
        }
    }
    return 1;
}

void time_mf_bst(size_t array_size, size_t window_size)
{
    int a[MAX_ARRAY_SIZE];
    int out_bst[MAX_ARRAY_SIZE];
    clock_t start, finish;
    size_t i;
    for (i = 0; i < array_size; i++) {
        a[i] = rand();
    }
    
    start = clock();
    for (i = 0; i < 10000; i++) {
        mf_bst(a, array_size, window_size, out_bst);
    }
    
    finish = clock();
    
    printf("1000 time: %f\n", (float)(finish - start) / CLOCKS_PER_SEC);
}

// рекурсивная версия
//void bst_insert(bst_node *root, int key)
//{
//    bst_node *gp = root->p;
//    int tmp;
//    if (key < root->key) {
//        if (root->l) {
//            bst_insert(root->l, key);
//        } else {
//            // проверка на лысые ветки
//            if (!root->r && gp) {   // у родителя нет других детей
//                                    // цепляемся не к корню
//
//                if (gp->l == root) {    // родитель - левый ребенок у деда
//                    if (!gp->r) {       // у деда 1 ребенок
//                        tmp         = gp->key;
//                        gp->key     = root->key;
//                        root->key   = key;
//                        bst_attach_right(gp, tmp);
//                        return;
//                    }
//                } else {                //  родитель - правый ребенок у деда
//                    if (!gp->l) {       // у деда 1 ребенок
//                        if (key > gp->key) {
//                        // только правый ребенок может быть равен родителю
//                            tmp     = gp->key;
//                            gp->key = key;
//                            bst_attach_left(gp, tmp);
//                            return;
//                        }
//                    }
//                }
//            }
//            bst_attach_left(root, key);
//        }
//    } else {
//        if (root->r) {
//            bst_insert(root->r, key);
//        } else {
//            // проверка на лысые ветки
//            if (!root->l && gp) {   // у родителя нет других детей
//                                    // цепляемся не к корню
//
//                if (gp->l == root) {    // родитель - левый ребенок у деда
//                    if (!gp->r) {       // у деда 1 ребенок
//                        if (key > root->key) {
//                            tmp         = gp->key;
//                            gp->key     = key;
//                            bst_attach_right(gp, tmp);
//                            return;
//                        }
//                    }
//                } else {                //  родитель - правый ребенок у деда
//                    if (!gp->l) {       // у деда 1 ребенок
//                        if (root->key > gp->key) {
//                            tmp         = gp->key;
//                            gp->key     = root->key;
//                            root->key   = key;
//                            bst_attach_left(gp, tmp);
//                            return;
//                        }
//                    }
//                }
//            }
//            bst_attach_right(root, key);
//        }
//    }
//}

// рекурсивная версия
//void bst_remove_key(bst_node *root, int key)
//{
//    if (key < root->key) {
//        bst_remove_key(root->l, key);
//    } else if (key > root->key) {
//        bst_remove_key(root->r, key);
//    } else {
//        bst_remove_node(root);
//    }
//}


//void bst_pushdown_leftside(bst_node *node)
//{
//    bst_node *left, *new;
//    int tmp_par_val, tmp_left_val;
//    size_t tmp_par_ix, tmp_left_ix;
//    
//    tmp_par_val = node->val;
//    tmp_par_ix = node->aix;
//    
//    left = node->l;
//    
//    while (1) {
//        tmp_left_val = left->val;
//        tmp_left_ix = left->aix;
//        
//        left->val = tmp_par_val;
//        left->aix = tmp_par_ix;
//        
//        tmp_par_val = tmp_left_val;
//        tmp_par_ix = tmp_left_ix;
//        
//        if (left->l) {
//            left = left->l;
//        } else {
//            new = (bst_node *)malloc(sizeof(bst_node));
//            new->val = tmp_par_val;
//            new->aix = tmp_par_ix;
//            new->p = left;
//            new->l = NULL;
//            new->r = NULL;
//            left->l = new;
//            break;
//        }
//    }
//    node->val = -1;
//}
//
//void bst_pushdown_rightside(bst_node *node)
//{
//    bst_node *right, *new;
//    int tmp_par_val, tmp_right_val;
//    size_t tmp_par_ix, tmp_right_ix;
//    
//    tmp_par_val = node->val;
//    tmp_par_ix = node->aix;
//    
//    right = node->r;
//    
//    while (1) {
//        tmp_right_val = right->val;
//        tmp_right_ix = right->aix;
//        
//        right->val = tmp_par_val;
//        right->aix = tmp_par_ix;
//        
//        tmp_par_val = tmp_right_val;
//        tmp_par_ix = tmp_right_ix;
//        
//        if (right->r) {
//            right = right->r;
//        } else {
//            new = (bst_node *)malloc(sizeof(bst_node));
//            new->val = tmp_par_val;
//            new->aix = tmp_par_ix;
//            new->p = right;
//            new->l = NULL;
//            new->r = NULL;
//            right->r = new;
//            break;
//        }
//    }
//    node->val = -1;
//}
//
//void bst_pullup_leftside(bst_node *node)
//{
//    bst_node *left, *parent;
//    
//    left = node;
//    parent = left->p;
//    
//    while (1) {
//        parent->val = left->val;
//        parent->aix = left->aix;
//        
//        if (!left->l) {
//            if (!left->r) {
//                parent->l = NULL;
//                free(left);
//            }
//            break;
//        }
//        parent = left;
//        left = left->l;
//    }
//}
//
//void bst_pullup_rightside(bst_node *node)
//{
//    bst_node *right, *parent;
//    
//    right = node;
//    parent = right->p;
//    
//    while (1) {
//        parent->val = right->val;
//        parent->aix = right->aix;
//        
//        if (!right->r) {
//            if (!right->l) {
//                parent->r = NULL;
//                free(right);
//            }
//            break;
//        }
//        parent = right;
//        right = right->r;
//    }
//}
