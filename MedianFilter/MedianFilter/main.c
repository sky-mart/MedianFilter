//
//  main.c
//  MedianFilter
//
//  Created by Влад Агиевич on 18.05.17.
//  Copyright © 2017 Mocsmart. All rights reserved.
//

#include <stdio.h>
#include "median_filter.h"
#include <stdlib.h>
#include <time.h>

#define A_LEN 7

int main(int argc, const char * argv[])
{
//    bst_node *n0, *n1, *n2, *n3, *n4, *n5, *n6;
////    bst_node *tn;
//    
//    n0 = (bst_node *)malloc(sizeof(bst_node));
//    n1 = (bst_node *)malloc(sizeof(bst_node));
//    n2 = (bst_node *)malloc(sizeof(bst_node));
//    n3 = (bst_node *)malloc(sizeof(bst_node));
//    n4 = (bst_node *)malloc(sizeof(bst_node));
//    n5 = (bst_node *)malloc(sizeof(bst_node));
//    n6 = (bst_node *)malloc(sizeof(bst_node));
//    
//    bst_node_init(n0, 4, NULL, n1, n2);
//    bst_node_init(n1, 2, n0, n3, n4);
//    bst_node_init(n2, 7, n0, n5, n6);
//    bst_node_init(n3, 1, n1, NULL, NULL);
//    bst_node_init(n4, 3, n1, NULL, NULL);
//    bst_node_init(n5, 5, n2, NULL, NULL);
//    bst_node_init(n6, 8, n2, NULL, NULL);
//    bst_print(n0);
//    
//    bst_insert(n0, 2);
//    bst_print(n0);
//    
//    bst_remove_key(n0, 2);
//    bst_print(n0);
    
//    tn = bst_leftest_node(n6);
//    printf("x = %i\n", tn->value);
    
//    bst_root2right(n0);
//    bst_print(n0);
//    bst_insert(n0, 6, -1);
//    bst_print(n0);
//    
//    bst_remove(n5->r);
//    bst_print(n0);
//    
//    bst_remove(n0);
//    bst_print(n0);
//    
//    mf_step(n0, n0, make_data(1, 7));
//    bst_print(n0);
    
//    bst_free(n0);
//    free(n0);
    
//    bst_node *root = (bst_node *)malloc(sizeof(bst_node));
//    int a[] = {1, 2, 3, 4, 5, 7, 8};
//    sa2bbst(a, 0, 7, root);
//    bst_print(root);
//    bst_free(root);
//    free(root);
 
//    int ns[] = {2, 7, 8, 1, 3, 5, 4, 10, 12, 3, 2, 1, 15, 6};
//    int out[14];
//    int n = 14, r = 9;
//    
//    array_println(ns, n);
//    
//    mf_bst(ns, n, r, out);
//    array_println(out, n-r+1);
//    
//    mf_direct(ns, n, r, out);
//    array_println(out, n-r+1);
    
    srand(time(NULL));
//    testset_mf_bst();
    time_mf_bst(4000, 32);
    return 0;
}

// n 1 n n n 1 n
