#include "persistent_tree.hpp"
#include "utils.hpp"

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

TEST(persistant_tree, insert_save_3) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(10);
    ptree.insert_k(12);
    ptree.insert_k(11);
    ptree.insert_k(1);
    ptree.insert_k(2);
    ptree.insert_k(3);

    std::vector<int> answer = ptree.insert_s(1);
    std::vector<int> expected = {};
    std::stringstream out;
    out << ptree;
    EXPECT_EQ(out.str(), "Current version in-order: 1 2 3 10 11 12 Prev version in-order: 1 2 3 10 11 12 ");
    EXPECT_EQ(answer, expected);
}

TEST(persistant_tree, insert_save_4) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(10);
    ptree.insert_k(11);
    ptree.insert_k(12);
    ptree.insert_k(13);
    ptree.insert_k(14);
    ptree.insert_k(15);

    std::vector<int> answer = ptree.insert_s(16);
    std::vector<int> expected = {10, 11, 12, 13, 14, 15};
    std::stringstream out;
    out << ptree;
    EXPECT_EQ(out.str(), "Current version in-order: 10 11 12 13 14 15 16 Prev version in-order: 10 11 12 13 14 15 ");
    EXPECT_EQ(answer, expected);
}

TEST(persistant_tree, rollback_1) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(10);
    ptree.insert_k(12);
    ptree.insert_k(11);
    ptree.insert_k(1);
    ptree.insert_k(2);
    ptree.insert_k(3);

    std::vector<int> answer = ptree.insert_s(9);
    std::vector<int> expected = {10, 1, 2, 3};
    EXPECT_EQ(answer, expected);
    ptree.rollback();
    expected.clear();
    std::stringstream out;
    out << ptree;
    EXPECT_EQ(out.str(), "Current version in-order: 1 2 3 10 11 12 Prev version in-order: 1 2 3 10 11 12 ");
}

TEST(persistant_tree, rollback_2) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(10);
    ptree.insert_k(12);
    ptree.insert_k(11);
    ptree.insert_k(1);
    ptree.insert_k(2);
    ptree.insert_k(3);

    std::vector<int> answer = ptree.insert_s(9);
    std::vector<int> expected = {10, 1, 2, 3};
    EXPECT_EQ(answer, expected);
    ptree.rollback();
    expected.clear();
    answer = ptree.insert_s(13);
    expected = {10, 12};

    std::stringstream out;
    out << ptree;
    EXPECT_EQ(answer, expected);
    EXPECT_EQ(out.str(), "Current version in-order: 1 2 3 10 11 12 13 Prev version in-order: 1 2 3 10 11 12 ");
}

TEST(persistant_tree, rollback_3) {
    tree::PersistentTree<int> ptree;
    ptree.insert_k(10);
    ptree.insert_k(12);
    ptree.insert_k(11);
    ptree.insert_k(1);
    ptree.insert_k(2);
    ptree.insert_k(3);

    std::vector<int> answer = ptree.insert_s(9);
    std::vector<int> expected = {10, 1, 2, 3};
    EXPECT_EQ(answer, expected);
    ptree.rollback();
    expected.clear();
    ptree.insert_k(13);
    ptree.insert_k(16);
    ptree.insert_k(21);
    ptree.insert_k(15);
    answer = ptree.insert_s(18);
    expected = {10, 12, 13, 16, 21};

    std::stringstream out;
    out << ptree;
    EXPECT_EQ(answer, expected);
    EXPECT_EQ(out.str(), "Current version in-order: 1 2 3 10 11 12 13 15 16 18 21 Prev version in-order: 1 2 3 10 11 12 13 15 16 21 ");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
