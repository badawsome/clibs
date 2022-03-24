//
// Created by badcw on 2022/3/23.
//

#include <avl.h>
#include <stdio.h>

extern void* avl_walk(struct avl_tree*, void*, int);

extern void avl_create(avl_tree_t* tree, int (*compar)(const void*, const void*), size_t size, size_t offset);
extern void avl_find(avl_tree_t* tree, const void* node, avl_index_t* where);
extern void avl_insert(avl_tree_t* tree, void* new_data, void* here, int direction);

extern void* avl_first(avl_tree_t* tree);
extern void* avl_last(avl_tree_t* tree);

extern void* avl_nearest(avl_tree_t* tree, avl_index_t where, int direction);
extern void* avl_add(avl_tree_t* tree, void* node);
extern void* avl_remove(avl_tree_t* tree, void* node);

extern boolean_t avl_update(avl_tree_t* tree, void*);
extern boolean_t avl_update_lt(avl_tree_t* tree, void*);
extern boolean_t avl_update_gt(avl_tree_t* tree, void*);

extern void avl_swap(avl_tree_t* tree1, avl_tree_t* tree2);

extern ulong_t   avl_numnodes(avl_tree_t* tree);
extern boolean_t avl_is_empty(avl_tree_t* tree);

extern void* avl_destroy_nodes(avl_tree_t* tree, void** cookie);
extern void  avl_destroy(avl_tree_t* tree);
