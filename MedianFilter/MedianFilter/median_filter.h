//
//  median_filter.h
//  MedianFilter
//
//  Created by Влад Агиевич on 18.05.17.
//  Copyright © 2017 Mocsmart. All rights reserved.
//

#ifndef median_filter_h
#define median_filter_h

#include <inttypes.h>

typedef struct {
    int     val;
    size_t  aix;
} data;

struct bst_node{
    int key;
    struct bst_node *p;
    struct bst_node *l;
    struct bst_node *r;
};

typedef struct bst_node bst_node;

data make_data(int val, size_t aix);

void bst_node_init(bst_node *node, int key, bst_node *parent, bst_node *left, bst_node *right);

void bst_free(bst_node *root);

void bst_pushdown_leftside(bst_node *root);
void bst_pushdown_rightside(bst_node *root);

void bst_pullup_leftside(bst_node *node);
void bst_pullup_rightside(bst_node *node);

void array_print(int *a, size_t n);
void array_println(int *a, size_t n);

void bst_insert(bst_node *root, int key);
void bst_remove_node(bst_node *node);
void bst_remove_key(bst_node *root, int key);

bst_node *bst_leftest_node(bst_node *root);
bst_node *bst_rightest_node(bst_node *root);

void bst_root2left(bst_node *root);
void bst_root2right(bst_node *root);

//void bst_print(int *t, size_t n);
//void bst_println(int *t, size_t n);

bst_node **bst_print_aux(bst_node **parents, size_t n);
void bst_print(bst_node *root);
void bst_println(bst_node *root);


size_t pow2(size_t sa_len);
void sa2bbst(int *sa, size_t start, size_t end, bst_node *node);

void mf_step(bst_node *root, int oldkey, int newkey);

void mf_bst(int *input, size_t n, size_t r, int *output);
void mf_direct(int *input, size_t n, size_t r, int *output);

uint8_t test_mf_bst(size_t array_size, size_t window_size);
uint8_t testset_mf_bst();
void time_mf_bst(size_t array_size, size_t window_size);

#endif /* median_filter_h */
