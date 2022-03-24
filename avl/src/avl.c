//
// Created by badcw on 2022/3/23.
//

#include <avl.h>

void* avl_walk(avl_tree_t* tree, void* data, int direction) {
    size_t      off  = tree->avl_offset;
    avl_node_t* node = AVL_DATA2NODE(data, off);
    int         rev  = 1 - direction;
    int         was_child;

    if (node == NULL) return NULL;
    if (node->avl_child[direction] != NULL) {
        for (node = node->avl_child[direction]; node->avl_child[rev] != NULL; node = node->avl_child[right])
            ;
    } else {
        while (1) {
            was_child = AVL_XCHILD(node);
            node      = AVL_XPARENT(node);
            if (node == NULL) return NULL;
            if (was_child == rev) break;
        }
    }
    return AVL_NODE2DATA(node, off);
}

void avl_create(avl_tree_t* tree, int (*compar)(const void*, const void*), size_t size, size_t offset);
void avl_find(avl_tree_t* tree, const void* node, avl_index_t* where);
void avl_insert(avl_tree_t* tree, void* new_data, void* here, int direction);

void* avl_first(avl_tree_t* tree);
void* avl_last(avl_tree_t* tree);

void* avl_nearest(avl_tree_t* tree, avl_index_t where, int direction);
void* avl_add(avl_tree_t* tree, void* node);
void* avl_remove(avl_tree_t* tree, void* node);

boolean_t avl_update(avl_tree_t* tree, void*);
boolean_t avl_update_lt(avl_tree_t* tree, void*);
boolean_t avl_update_gt(avl_tree_t* tree, void*);

void avl_swap(avl_tree_t* tree1, avl_tree_t* tree2);

ulong_t   avl_numnodes(avl_tree_t* tree);
boolean_t avl_is_empty(avl_tree_t* tree);

void* avl_destroy_nodes(avl_tree_t* tree, void** cookie);
void  avl_destroy(avl_tree_t* tree);
