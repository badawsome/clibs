#include "gtest/gtest.h"

#include <string>
#include <vector>

#include <avl.h>
#include <mt_19937_64.h>
#include <utils.h>

namespace test_map {
typedef avl_tree_t map;
template <typename K, typename V>
struct map_node {
    K key;
    V value;

    avl_node_t node;
};

template <typename V>
int map_order(const void* node1, const void* node2) {
    auto n1 = (map_node<std::string, V>*)node1;
    auto n2 = (map_node<std::string, V>*)node2;

    int compare = n1->key.compare(n2->key);
    if (compare == 0) return 0;
    return compare > 0 ? 1 : -1;
}

template <typename K, typename V>
int map_order(const void* node1, const void* node2) {
    auto n1 = (map_node<K, V>*)node1;
    auto n2 = (map_node<K, V>*)node2;

    if (n1->key == n2->key) return 0;
    return (n1->key < n2->key) ? -1 : 1;
}

template <typename K, typename V>
map new_map() {
    map map_t;
    avl_create(&map_t, map_order<K, V>, sizeof(map_node<K, V>), offset_of(&map_node<K, V>::node));
    return map_t;
}

template <typename K, typename V>
void set(map& map_t, const K& key, const V& value) {
    avl_index_t idx = 0;

    map_node<K, V> mn{.key = key};

    auto* node = static_cast<map_node<K, V>*>(avl_find(&map_t, &mn, &idx));
    if (node != nullptr) {
        node->value = value;
    } else {
        node = new map_node<K, V>{.key = key, .value = value};
        avl_insert(&map_t, node, idx);
    }
}

template <typename K, typename V>
V get(map& map_t, const K& key) {
    map_node<K, V> mn{.key = key};

    auto* node = static_cast<map_node<K, V>*>(avl_find(&map_t, &mn, nullptr));
    if (node != nullptr) { return node->value; }
    return 0;
}

template <typename K, typename V>
void remove(map& map_t, const K& key) {
    map_node<K, V> mn{.key = key};
    avl_remove(&map_t, &mn);
}

size_t count(map& map_t) {
    return map_t.avl_num_of_nodes;
}

}  // namespace test_map

// basic test
TEST(AVL_AS_MAP, BASIC_INSERT_QUERY_DELETE) {
    auto mp = test_map::new_map<std::string, int>();
    test_map::set(mp, std::string("I"), 1);
    test_map::set(mp, std::string("you"), 2);
    EXPECT_EQ((test_map::get<std::string, int>(mp, std::string("I"))), 1);
    EXPECT_EQ((test_map::get<std::string, int>(mp, std::string("you"))), 2);
    EXPECT_EQ((test_map::get<std::string, int>(mp, std::string("not exist"))), 0);
    test_map::remove<std::string, int>(mp, std::string("I"));
    EXPECT_EQ((test_map::get<std::string, int>(mp, std::string("I"))), 0);
}

// 1e4 insert
TEST(AVL_AS_MAP, TEN_THOUSAND_MILLION_INSERT) {
    auto mp = test_map::new_map<std::string, int>();

    std::map<std::string, int> std_map;

    //    size_t ten_thousand = 100;
    size_t ten_thousand = 10'000;

    init_genrand64(12442234833586032);
    for (size_t i = 0; i < ten_thousand; ++i) {
        //        int str   = genrand64_int64();
        //        int value = genrand64_int64();
        //        printf("dbg %zu %d\n", i, str);
        auto str     = convert_uint64_2_string(std::vector<u_longlong_t>{genrand64_int64(), genrand64_int64()});
        int  value   = static_cast<int>(genrand64_int64() % (INT_MAX));
        std_map[str] = value;
        test_map::set(mp, str, value);
    }

    EXPECT_EQ(std_map.size(), test_map::count(mp));
    for (auto& i : std_map) { EXPECT_EQ((test_map::get<std::string, int>(mp, i.first)), i.second); }
}
