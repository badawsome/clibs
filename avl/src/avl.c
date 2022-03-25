//
// Created by badcw on 2022/3/23.
//

#ifdef CMAKE
#include "include/avl.h"
#else
#include <avl.h>
#include <dbg.h>
#endif

void* avl_walk(avl_tree_t* tree, void* data, int direction) {
    int rev = 1 - direction;
    int was_child;

    size_t off = tree->avl_offset;

    avl_node_t* node = AVL_DATA2NODE(data, off);

    if (node == NULL) return NULL;
    if (node->avl_child[direction] != NULL) {
        for (node = node->avl_child[direction]; node->avl_child[rev] != NULL; node = node->avl_child[rev])
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

void avl_create(avl_tree_t* tree, int (*order)(const void*, const void*), size_t size, size_t offset) {
    ASSERT(tree);
    ASSERT(order);
    ASSERT(size > 0);
    ASSERT(size >= offset + sizeof(avl_node_t));
#ifdef _LP64
    ASSERT((offset & 0x7) == 0);
#endif
    tree->avl_order    = order;
    tree->avl_root     = NULL;
    tree->avl_numnodes = 0;
    tree->avl_size     = size;
    tree->avl_offset   = offset;
}

void avl_find(avl_tree_t* tree, const void* node, avl_index_t* where);
void avl_insert(avl_tree_t* tree, void* new_data, void* here, int direction);

void* avl_first(avl_tree_t* tree) {
    avl_node_t* node;
    avl_node_t* prev = NULL;

    for (node = tree->avl_root; node != NULL; node = node->avl_child[0]) { prev = node; }

    return prev ? AVL_NODE2DATA(prev, tree->avl_offset) : NULL;
}
void* avl_last(avl_tree_t* tree) {
    avl_node_t* node;
    avl_node_t* prev = NULL;

    for (node = tree->avl_root; node != NULL; node = node->avl_child[1]) { prev = node; }
    return prev ? AVL_NODE2DATA(prev, tree->avl_offset) : NULL;
}
void* avl_nearest(avl_tree_t* tree, avl_index_t where, int direction) {
    int         child = AVL_INDEX2CHILD(where);
    avl_node_t* node  = AVL_INDEX2NODE(where);

    void*  data;
    size_t off = tree->avl_offset;

    if (node == NULL) {
        ASSERT(tree->avl_root == NULL);
        return NULL;
    }
    data = AVL_NODE2DATA(node, off);
    if (child != direction) return data;

}

void* avl_add(avl_tree_t* tree, void* node);
void* avl_remove(avl_tree_t* tree, void* node);

boolean_t avl_update(avl_tree_t* tree, void*);
boolean_t avl_update_lt(avl_tree_t* tree, void*);
boolean_t avl_update_gt(avl_tree_t* tree, void*);

void avl_swap(avl_tree_t* tree1, avl_tree_t* tree2);

ulong_t   avl_numnodes(avl_tree_t* tree);
boolean_t avl_is_empty(avl_tree_t* tree);

#define CHILDBIT (1L)

void* avl_destroy_nodes(avl_tree_t* tree, void** cookie) {
    avl_node_t* node;
    avl_node_t* parent;
    int         child;
    void*       first;
    size_t      off = tree->avl_offset;

    if (*cookie == NULL) {
        first = avl_first(tree);
        if (first == NULL) {
            *cookie = (void*)CHILDBIT;
            return NULL;
        }

        node   = AVL_DATA2NODE(first, off);
        parent = AVL_XPARENT(node);
        goto check_right_side;
    }
    parent = (avl_node_t*)((uintptr_t)(*cookie) & ~CHILDBIT);
    if (parent == NULL) {
        if (tree->avl_root != NULL) {
            ASSERT(tree->avl_numnodes == 1);
            tree->avl_root     = NULL;
            tree->avl_numnodes = 0;
        }
    }
    child = (uintptr_t)(*cookie) & CHILDBIT;

    parent->avl_child[child] = NULL;
    ASSERT(tree->avl_numnodes > 1);
    --tree->avl_numnodes;

    if (child == 1 || parent->avl_child[1] == NULL) {
        node   = parent;
        parent = AVL_XPARENT(parent);
        goto done;
    }

    node = parent->avl_child[1];
    while (node->avl_child[0] != NULL) {
        parent = node;
        node   = node->avl_child[0];
    }

check_right_side:
    if (node->avl_child[1] != NULL) {
        ASSERT(AVL_XBALANCE(node) == 1);
        parent = node;
        node   = node->avl_child[1];
        ASSERT(node->avl_child[0] == NULL && node->avl_child[1] == NULL);
    } else {
        ASSERT(AVL_XBALANCE(node) <= 0);
    }

done:
    if (parent == NULL) {
        *cookie = (void*)CHILDBIT;
        ASSERT(node == tree->avl_root);
    } else {
        *cookie = (void*)((uintptr_t)parent | AVL_XCHILD(node));
    }
    return AVL_NODE2DATA(node, off);
}

/* ARGSUSED */
void avl_destroy(avl_tree_t* tree) {
    ASSERT(tree);
    ASSERT(tree->avl_numnodes == 0);
    ASSERT(tree->avl_root == NULL);
}
