#ifndef _AVL_H
#define _AVL_H

// headers
#include <stddef.h>    // define size_t
#include <stdint.h>    // define uintptr_t
#include <stdtypes.h>  // define boolean_t

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _LP64
// TODO !_LP64 support bit optimiztion
//  struct avl_node {
//    struct avl_node *avl_child[2];
//    uintptr_t avl_pcb; /* parent, child_index, balance */
//  };
#endif

struct avl_node {
    struct avl_node* avl_child[2];
    struct avl_node* avl_parent;
    unsigned short   avl_child_index;
    short            avl_balance; /* balance -1,0,+1 */
};

#define AVL_XPARENT(n) ((n)->avl_parent)
#define AVL_SETPARENT(n, p) ((n)->avl_parent = (p))

#define AVL_XCHILD(n) ((n)->avl_child_index)
#define AVL_SETCHILD(n, c) ((n)->avl_child_index = (c))

#define AVL_XBALANCE(n) ((n)->avl_balance)
#define AVL_SETBALANCE(n, b) ((n)->avl_balance = (b))

struct avl_tree {
    struct avl_node* avl_root;

    int (*avl_order)(const void*, const void*);

    size_t  avl_offset;
    ulong_t avl_num_of_nodes;
    size_t  avl_size;
};

// common define for 32bits or 64bits
typedef struct avl_node avl_node_t;
typedef struct avl_tree avl_tree_t;
typedef uintptr_t       avl_index_t;

/*
 * Node: an AVL node pointer
 * Data: a struct provides by user
 * Child: left child or right child
 * Offset: offset of the node in data
 * Index: an index to get to give information, represents node with child
 * Direction: AVL_BEFORE or AVL_AFTER
 * Order: order function of two data, -1 means less than, 0 means equal, 1 means greater than
 * */

#define AVL_NODE2DATA(n, o) ((void*)(uintptr_t)(n) - (o))
#define AVL_DATA2NODE(d, o) ((struct avl_node*)((uintptr_t)(d) + (o)))

#define AVL_INDEX2NODE(x) ((avl_node_t*)((x) & ~1))
#define AVL_INDEX2CHILD(x) ((x)&1)
#define AVL_MKINDEX(n, c) ((avl_index_t)(n) | (c))

#define TREE_ISIGN(a) (((a) > 0) - ((a) < 0))
#define TREE_CMP(a, b) (((a) > (b)) - ((a) < (b)))
#define TREE_PCMP(a, b) (((uintptr_t)(a) > (uintptr_t)(b)) - ((uintptr_t)(a) < (uintptr_t)(b)))

#define AVL_BEFORE (0)
#define AVL_AFTER (1)

// interface need be implied

// walk to data's AVL_AFTER or AVL_BEFORE
extern void* avl_walk(avl_tree_t* tree, void* data, int direction);

extern void avl_create(avl_tree_t* tree, int (*avl_order)(const void*, const void*), size_t size, size_t offset);
/* find data in avl tree, if not found, value of (where) is the index you should insert the data.
 *
 * where: Optional param(push NULL if no need), if you want to get index to insert a data.
 *
 * Return:
 *  1. if return NULL means no found, if where is not NULL where give the index
 *  2. return not NULL means found, will not change where
 * */
extern void* avl_find(avl_tree_t* tree, const void* data, avl_index_t* where);
extern void  avl_insert(avl_tree_t* tree, void* new_data, avl_index_t where);
/* OPTIONAL: insert new_data to data node's some direction */
extern void avl_insert_here(avl_tree_t* tree, void* new_data, void* data, int direction);

extern void* avl_first(avl_tree_t* tree);
extern void* avl_last(avl_tree_t* tree);

#define AVL_NEXT(tree, data) avl_walk(tree, data, AVL_AFTER)
#define AVL_PREV(tree, data) avl_walk(tree, data, AVL_BEFORE)

extern void* avl_nearest(avl_tree_t* tree, avl_index_t where, int direction);
extern void  avl_add(avl_tree_t* tree, void* data);
extern void  avl_remove(avl_tree_t* tree, void* data);

extern boolean_t avl_update(avl_tree_t* tree, void* data);
extern boolean_t avl_update_lt(avl_tree_t* tree, void* data);
extern boolean_t avl_update_gt(avl_tree_t* tree, void* data);

extern void avl_swap(avl_tree_t* tree1, avl_tree_t* tree2);

extern ulong_t   avl_num_of_nodes(avl_tree_t* tree);
extern boolean_t avl_is_empty(avl_tree_t* tree);

extern void* avl_destroy_nodes(avl_tree_t* tree, void** breakpoint);
/* do nothing in release, debug mode will check whether the tree is totally deleted */
extern void avl_destroy(avl_tree_t* tree);

#ifdef __cplusplus
}
#endif

#endif  // !_AVL_H
