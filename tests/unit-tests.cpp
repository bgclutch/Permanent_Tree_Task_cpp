#include "persistent_tree.hpp"

#include <gtest/gtest.h>

TEST(binary_search_tree, insert_1) {
    tree::BinarySearchTree<int> tree{};
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    std::stringstream out; 
    out << tree;
    EXPECT_EQ(out.str(), "1 2 3 ");
}

TEST(binary_search_tree, insert_2) {
    tree::BinarySearchTree<int> tree{};
    tree.insert(5);
    tree.insert(2);
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    std::stringstream out; 
    out << tree;
    EXPECT_EQ(out.str(), "1 2 3 5 ");
}

TEST(binary_search_tree, insert_3) {
    tree::BinarySearchTree<int, std::greater<int>> tree{};
    tree.insert(5);
    tree.insert(2);
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    std::stringstream out; 
    out << tree;
    EXPECT_EQ(out.str(), "5 3 2 1 ");
}

TEST(persistant_tree, insert_key_1) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(5);
    ptree.insert_k(6);
    ptree.insert_k(1);

    std::stringstream out;
    out << ptree; 

    EXPECT_EQ(out.str(), "Current version in-order: 1 5 6 Prev version in-order: empty");
}


TEST(persistant_tree, insert_key_2) {
    tree::PersistentTree<int, std::greater<int>> ptree;
    ptree.insert_k(5);
    ptree.insert_k(6);
    ptree.insert_k(1);

    std::stringstream out;
    out << ptree; 

    EXPECT_EQ(out.str(), "Current version in-order: 6 5 1 Prev version in-order: empty");
}

TEST(persistant_tree, insert_save_1) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(5);
    ptree.insert_k(6);
    ptree.insert_k(1);
    ptree.insert_s(2);

    std::stringstream out;
    out << ptree; 

    EXPECT_EQ(out.str(), "Current version in-order: 1 2 5 6 Prev version in-order: 1 5 6 ");
}

TEST(persistant_tree, insert_save_2) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(4);
    ptree.insert_k(3);
    ptree.insert_k(8);
    ptree.insert_k(2);
    ptree.insert_k(7);
    ptree.insert_k(10);

    std::vector<int> answer = ptree.insert_s(5);
    std::vector<int> expected = {4, 8, 7};
    std::stringstream out;
    out << ptree;
    EXPECT_EQ(out.str(), "Current version in-order: 2 3 4 5 7 8 10 Prev version in-order: 2 3 4 7 8 10 ");
    EXPECT_EQ(answer, expected);
}

