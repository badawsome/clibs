//
// Created by badcw on 2022/3/23.
//

#include <avl.h>
#include <dbg.h>

static const int avl_child2balance[2] = {-1, 1};
static const int avl_balance2child[]  = {0, 0, 1};

/* TODO not public extern */
static int avl_rotation(avl_tree_t* tree, avl_node_t* node, int balance) {
    ASSERT(0);
    return 0;
}

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
    tree->avl_order        = order;
    tree->avl_root         = NULL;
    tree->avl_num_of_nodes = 0;
    tree->avl_size         = size;
    tree->avl_offset       = offset;
}

void* avl_find(avl_tree_t* tree, const void* data, avl_index_t* where) {
    avl_node_t* node;
    avl_node_t* prev;

    int child = 0;
    int diff;

    for (node = tree->avl_root; node != NULL; node = node->avl_child[child]) {
        prev = node;
        diff = tree->avl_order(data, AVL_NODE2DATA(node, tree->avl_offset));
        ASSERT(-1 <= diff && diff <= 1);
        if (diff == 0) return AVL_NODE2DATA(node, tree->avl_offset);
        child = avl_balance2child[1 + diff];
    }
    if (where != NULL) *where = AVL_MKINDEX(prev, child);
    return NULL;
}
void avl_insert(avl_tree_t* tree, void* new_data, avl_index_t where) {
    avl_node_t* node;
    avl_node_t* parent = AVL_INDEX2NODE(where);

    int    old_balance;
    int    new_balance;
    int    which_child = AVL_INDEX2CHILD(where);
    size_t off         = tree->avl_offset;
#ifdef _LP64
    ASSERT(((uintptr_t)new_data & 0x7) == 0);
#endif
    node = AVL_DATA2NODE(new_data, off);

    // insert node
    ++tree->avl_num_of_nodes;
    node->avl_child[0] = node->avl_child[1] = NULL;
    AVL_SETCHILD(node, which_child);
    AVL_SETBALANCE(node, 0);
    AVL_SETPARENT(node, parent);
    if (parent != NULL) {
        ASSERT(parent->avl_child[which_child] == NULL);
        parent->avl_child[which_child] = node;
    } else {
        ASSERT(tree->avl_root == NULL);
        tree->avl_root = node;
    }

    // calculate balance
    for (;;) {
        node = parent;
        if (node == NULL) return;
        old_balance = AVL_XBALANCE(node);
        new_balance = old_balance + avl_child2balance[which_child];
        if (new_balance == 0) {
            AVL_SETBALANCE(node, 0);
            // no need to rebalance
            return;
        }
        if (old_balance != 0) break;
        // old balance is zero, new balance's absolute value not greater than 1, no need rebalance
        AVL_SETBALANCE(node, new_balance);
        parent      = AVL_XPARENT(node);
        which_child = AVL_XCHILD(node);
    }

    // auto balance
    avl_rotation(tree, node, new_balance);
}
void avl_insert_here(avl_tree_t* tree, void* new_data, void* data, int direction) {
    avl_node_t* node;

    int child = direction;
#ifdef CLIBS_DEBUG
    int diff;
    ASSERT(tree != NULL);
    ASSERT(new_data != NULL);
    ASSERT(data != NULL);
    ASSERT(direction == AVL_BEFORE || direction == AVL_AFTER);
    ASSERT(tree->avl_order != NULL);
    diff = tree->avl_order(new_data, data);
    ASSERT(diff == -1 || diff == 1);
    ASSERT(direction == (diff == 1));
#endif /* CLIBS_DEBUG */

    node = AVL_DATA2NODE(data, tree->avl_offset);
    if (node->avl_child[child] != NULL) {
        node  = node->avl_child[child];
        child = 1 - child;
        while (node->avl_child[child] != NULL) { node = node->avl_child[child]; }
    }
    ASSERT(node->avl_child[child] == NULL);
    avl_insert(tree, new_data, AVL_MKINDEX(node, child));
}

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
    return avl_walk(tree, data, direction);
}

void avl_add(avl_tree_t* tree, void* data) {
#ifdef CLIBS_DEBUG
    avl_index_t where = 0;
    ASSERT(avl_find(tree, data, &where) == NULL);
#endif /* CLIBS_DEBUG */
    avl_insert(tree, data, 0);
}
void avl_remove(avl_tree_t* tree, void* data);

#define AVL_REINSERT(tree, node)                                                                                       \
    avl_remove(tree, node);                                                                                            \
    avl_add(tree, node)

boolean_t avl_update(avl_tree_t* tree, void* data) {
    void* neighbor;

    neighbor = AVL_PREV(tree, data);
    if ((neighbor != NULL) && (tree->avl_order(data, neighbor) < 0)) {
        AVL_REINSERT(tree, data);
        return B_TRUE;
    }

    neighbor = AVL_NEXT(tree, data);
    if ((neighbor != NULL) && (tree->avl_order(data, neighbor) > 0)) {
        AVL_REINSERT(tree, data);
        return B_TRUE;
    }

    return B_FALSE;
}
boolean_t avl_update_lt(avl_tree_t* tree, void* data) {
    void* neighbor;

    ASSERT(((neighbor = AVL_NEXT(tree, data))) == NULL || (tree->avl_order(data, neighbor) < 0));

    neighbor = AVL_PREV(tree, data);
    if ((neighbor != NULL) && (tree->avl_order(data, neighbor) < 0)) {
        AVL_REINSERT(tree, data);
        return B_TRUE;
    }
    return B_FALSE;
}
boolean_t avl_update_gt(avl_tree_t* tree, void* data) {
    void* neighbor;

    ASSERT(((neighbor = AVL_PREV(tree, data))) == NULL || (tree->avl_order(data, neighbor) > 0));

    neighbor = AVL_NEXT(tree, data);
    if ((neighbor != NULL) && (tree->avl_order(data, neighbor) > 0)) {
        AVL_REINSERT(tree, data);
        return B_TRUE;
    }
    return B_FALSE;
}

void avl_swap(avl_tree_t* tree1, avl_tree_t* tree2) {
    avl_node_t* temp_node;
    ulong_t     temp_number_of_nodes;

    ASSERT3P(tree1->avl_order, ==, tree2->avl_order);
    ASSERT(tree1->avl_offset == tree2->avl_offset);
    ASSERT(tree1->avl_size == tree2->avl_size);

    temp_node            = tree1->avl_root;
    temp_number_of_nodes = tree1->avl_num_of_nodes;

    tree1->avl_root         = tree2->avl_root;
    tree1->avl_num_of_nodes = tree2->avl_num_of_nodes;

    tree2->avl_root         = temp_node;
    tree2->avl_num_of_nodes = temp_number_of_nodes;
}

ulong_t avl_num_of_nodes(avl_tree_t* tree) {
    ASSERT(tree);
    return tree->avl_num_of_nodes;
}
boolean_t avl_is_empty(avl_tree_t* tree) {
    ASSERT(tree);
    return tree->avl_num_of_nodes == 0;
}

// child bits in index
#define CHILD_BIT (1L)

void* avl_destroy_nodes(avl_tree_t* tree, void** breakpoint) {
    avl_node_t* node;
    avl_node_t* parent;
    int         child;
    void*       first;
    size_t      off = tree->avl_offset;

    if (*breakpoint == NULL) {
        first = avl_first(tree);
        if (first == NULL) {
            *breakpoint = (void*)CHILD_BIT;
            return NULL;
        }

        node   = AVL_DATA2NODE(first, off);
        parent = AVL_XPARENT(node);
        goto check_right_side;
    }
    parent = (avl_node_t*)((uintptr_t)(*breakpoint) & ~CHILD_BIT);
    if (parent == NULL) {
        if (tree->avl_root != NULL) {
            ASSERT(tree->avl_num_of_nodes == 1);
            tree->avl_root         = NULL;
            tree->avl_num_of_nodes = 0;
        }
    }
    child = (int)((uintptr_t)(*breakpoint) & CHILD_BIT);

    parent->avl_child[child] = NULL;
    ASSERT(tree->avl_num_of_nodes > 1);
    --tree->avl_num_of_nodes;

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
        *breakpoint = (void*)CHILD_BIT;
        ASSERT(node == tree->avl_root);
    } else {
        *breakpoint = (void*)((uintptr_t)parent | AVL_XCHILD(node));
    }
    return AVL_NODE2DATA(node, off);
}

/* ARGSUSED */
void avl_destroy(avl_tree_t* tree) {
    ASSERT(tree);
    ASSERT(tree->avl_num_of_nodes == 0);
    ASSERT(tree->avl_root == NULL);
}
