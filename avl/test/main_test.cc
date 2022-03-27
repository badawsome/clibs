#include "gtest/gtest.h"
#include <avl.h>
#include <string>

namespace test_map {
typedef avl_tree_t map;
struct map_node {
    std::string key;
    int         value;
    avl_node_t  node;
};

int map_order(const void* node1, const void* node2) {
    auto n1 = (map_node*)node1;
    auto n2 = (map_node*)node2;
    return n1->key.compare(n2->key);
}

map new_map() {
    map map_t;
    avl_create(&map_t, map_order, sizeof(map_node), offsetof(map_node, node));
    return map_t;
}

void set(map map_t, const std::string& key, const int& value) {
    avl_index_t idx = 0;

    map_node mn{.key = key};

    auto* node = static_cast<map_node*>(avl_find(&map_t, &mn, &idx));
    if (node != nullptr) { node->value = value; }
    node = new map_node{.key = key, .value = value};
    avl_insert(&map_t, node, idx);
}

int get(map map_t, const std::string& key) {
    map_node mn{.key = key};
    auto*    node = static_cast<map_node*>(avl_find(&map_t, &mn, nullptr));
    if (node != nullptr) { return node->value; }
    return 0;
}

}  // namespace test_map

TEST(AVL, UseAVLAsMap) {
    auto mp = test_map::new_map();
    test_map::set(mp, "I", 1);
    test_map::set(mp, "you", 2);
    EXPECT_EQ(test_map::get(mp, "I"), 1);
    EXPECT_EQ(test_map::get(mp, "you"), 2);
    EXPECT_EQ(test_map::get(mp, "not exist"), 0);
}
