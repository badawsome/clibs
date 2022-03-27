//
// Created by badcw on 2022/3/23.
//

#include <avl.h>
#include <dbg.h>

static const int avl_child2balance[2] = {-1, 1};
static const int avl_balance2child[]  = {0, 0, 1};

/* not public extern
 * Input:
 * - balance must be -2 or 2(means need to change to new balance)
 *
 * Output:
 * - return value, means whether balance become low
 * */
static int avl_rotation(avl_tree_t* tree, avl_node_t* node, int balance) {
    ASSERT(tree != NULL);
    ASSERT(tree->avl_root != NULL);
    ASSERT(node != NULL);
    ASSERT(balance == -2 || balance == 2);
    // transform to do left heavy
    int left        = balance >= 0;
    int right       = 1 - left;
    int left_heavy  = balance >> 1;
    int right_heavy = -left_heavy;

    avl_node_t* parent = AVL_XPARENT(node);
    avl_node_t* child  = node->avl_child[left];
    // child's right child
    avl_node_t* cright = child->avl_child[right];
    // child's right child's left
    avl_node_t* crightleft;
    // child's right child's right
    avl_node_t* crightright;

    int which_child   = AVL_XCHILD(node);
    int child_balance = AVL_XBALANCE(child);

    // LL
    if (child_balance != right_heavy) {
        child_balance += right_heavy;
        node->avl_child[left] = cright;
        if (cright != NULL) {
            AVL_SETPARENT(cright, node);
            AVL_SETCHILD(cright, left);
        }
        child->avl_child[right] = node;
        AVL_SETPARENT(node, child);
        AVL_SETCHILD(node, right);
        AVL_SETBALANCE(node, -child_balance);

        AVL_SETBALANCE(child, child_balance);
        AVL_SETCHILD(child, which_child);
        AVL_SETPARENT(child, parent);

        if (parent != NULL)
            parent->avl_child[left] = child;
        else
            tree->avl_root = child;
        return child_balance == 0;
    }

    // LR
    ASSERT(cright != NULL);
    crightleft  = cright->avl_child[left];
    crightright = cright->avl_child[right];

    node->avl_child[left] = crightright;
    if (crightright != NULL) {
        AVL_SETPARENT(crightright, node);
        AVL_SETCHILD(crightright, left);
    }

    child->avl_child[right] = crightleft;
    if (crightleft != NULL) {
        AVL_SETPARENT(crightleft, child);
        AVL_SETCHILD(crightleft, right);
    }

    balance                 = AVL_XBALANCE(cright);
    cright->avl_child[left] = child;
    AVL_SETPARENT(child, cright);
    AVL_SETBALANCE(child, balance == right_heavy ? left_heavy : 0);
    ASSERT(AVL_XCHILD(child) == left);

    cright->avl_child[right] = node;
    AVL_SETPARENT(node, cright);
    AVL_SETBALANCE(node, balance == left_heavy ? right_heavy : 0);
    AVL_SETCHILD(node, right);

    if (parent != NULL) {
        parent->avl_child[which_child] = cright;
    } else {
        tree->avl_root = cright;
    }
    AVL_SETPARENT(cright, parent);
    AVL_SETCHILD(cright, which_child);
    AVL_SETBALANCE(cright, 0);

    return 1;
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
    avl_node_t* prev = NULL;

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
        if (old_balance != 0) {
            /*
             * only when old_balance and new_balance not equal to zero
             * and delta is 1, so new_balance must be -2 or 2
             * */
            break;
        }
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
void avl_remove(avl_tree_t* tree, void* data) {
    avl_node_t* delete;
    avl_node_t* parent;
    avl_node_t* node;
    avl_node_t  tmp;

    int    old_balance;
    int    new_balance;
    int    left;
    int    right;
    int    which_child;
    size_t off = tree->avl_offset;

    delete = AVL_DATA2NODE(data, off);
    if (delete->avl_child[0] != NULL && delete->avl_child[1] != NULL) {
        old_balance = AVL_XBALANCE(delete);
        left        = avl_balance2child[old_balance + 1];
        right       = 1 - left;

        for (node = delete->avl_child[left]; node->avl_child[right] != NULL; node = node->avl_child[right])
            ;
        tmp = *node;

        *node = *delete;
        if (node->avl_child[left] == node) { node->avl_child[left] = &tmp; }
        parent = AVL_XPARENT(node);
        if (parent != NULL)
            parent->avl_child[AVL_XCHILD(node)] = node;
        else
            tree->avl_root = node;
        AVL_SETPARENT(node->avl_child[0], node);
        AVL_SETPARENT(node->avl_child[1], node);
        delete                                = &tmp;
        parent                                = AVL_XPARENT(delete);
        parent->avl_child[AVL_XCHILD(delete)] = delete;
        which_child                           = (delete->avl_child[1] != 0);
        if (delete->avl_child[which_child] != NULL) { AVL_SETPARENT(delete->avl_child[which_child], delete); }
    }
    ASSERT(tree->avl_num_of_nodes > 0);
    --tree->avl_num_of_nodes;
    parent      = AVL_XPARENT(delete);
    which_child = AVL_XCHILD(delete);
    node        = delete->avl_child[delete->avl_child[0] == NULL];
    if (node != NULL) {
        AVL_SETPARENT(node, parent);
        AVL_SETCHILD(node, which_child);
    }
    if (parent == NULL) {
        tree->avl_root = node;
        return;
    }
    parent->avl_child[which_child] = node;

    do {
        node        = parent;
        old_balance = AVL_XBALANCE(node);
        new_balance = old_balance - avl_child2balance[which_child];
        parent      = AVL_XPARENT(node);
        which_child = AVL_XCHILD(node);
        if (old_balance == 0) {
            AVL_SETBALANCE(node, new_balance);
            break;
        }
        if (new_balance == 0)
            AVL_SETBALANCE(node, new_balance);
        else if (!avl_rotation(tree, node, new_balance))
            break;
    } while (parent != NULL);
}

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

/* ARGS-USED */
void avl_destroy(avl_tree_t* tree) {
    ASSERT(tree);
    ASSERT(tree->avl_num_of_nodes == 0);
    ASSERT(tree->avl_root == NULL);
}
