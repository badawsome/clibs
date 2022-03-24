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
    ulong_t avl_numnodes;
    size_t  avl_size;
};

// common define for 32bits or 64bits
typedef struct avl_node avl_node_t;
typedef struct avl_tree avl_tree_t;
typedef uintptr_t       avl_index_t;

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

// walk to node's AVL_AFTER or AVL_BEFORE
extern void* avl_walk(avl_tree_t* tree, void* data, int direction);

extern void avl_create(avl_tree_t* tree, int (*avl_order)(const void*, const void*), size_t size, size_t offset);
extern void avl_find(avl_tree_t* tree, const void* node, avl_index_t* where);
extern void avl_insert(avl_tree_t* tree, void* new_data, void* here, int direction);

extern void* avl_first(avl_tree_t* tree);
extern void* avl_last(avl_tree_t* tree);

#define AVL_NEXT(tree, node) avl_walk(tree, node, AVL_AFTER)
#define AVL_PREV(tree, node) avl_walk(tree, node, AVL_BEFORE)

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

#ifdef __cplusplus
}
#endif

#endif  // !_AVL_H
