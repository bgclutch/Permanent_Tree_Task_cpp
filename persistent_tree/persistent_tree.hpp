#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include <stack>

namespace tree {

static constexpr char key_request      = 'k';
static constexpr char rollback_request = 'r';
static constexpr char save_request     = 's';

enum class FindFlags {
    left,
    right,
    exists
};

template <typename KeyType, typename Compare = std::less<KeyType>>
class BinarySearchTree final {
private:
    class Node : public std::enable_shared_from_this<Node> {
    private:
        const KeyType value_;
        std::weak_ptr<Node> parent_;
        std::shared_ptr<Node> left_;
        std::shared_ptr<Node> right_;

    public:
        Node(const KeyType& value,
             std::weak_ptr<Node> parent = std::weak_ptr<Node>(),
             std::shared_ptr<Node> left = nullptr, 
             std::shared_ptr<Node> right = nullptr)
            : value_{value}, parent_{parent}, left_{left}, right_{right} {}

        KeyType get_value() const {
            return value_;
        } 

        void set_right(std::shared_ptr<Node> right) noexcept {
            right_ = right;
            if (right_) {
                right_->parent_ = this->weak_from_this();
            }
        }

        void set_left(std::shared_ptr<Node> left) noexcept {
            left_ = left;
            if (left_) {
                left_->parent_ = this->weak_from_this();
            }
        }

        std::shared_ptr<Node> get_right() const {
            return right_;
        }

        std::shared_ptr<Node> get_left() const {
            return left_;
        }
    };

    Compare comp_;
    std::shared_ptr<Node> root_ = nullptr;

public:
    BinarySearchTree() = default;
    BinarySearchTree(std::shared_ptr<Node> root) : root_{std::move(root)} {}

    using find_flag = FindFlags;
    using find_res  = std::pair<std::shared_ptr<Node>, find_flag>;

private:
    std::shared_ptr<Node> copy_path(const KeyType& key, std::vector<KeyType>& path) const {
        if (!root_) {
            return std::make_shared<Node>(key);
        }

        std::vector<std::shared_ptr<Node>> old_nodes;
        std::vector<std::shared_ptr<Node>> new_nodes;

        std::vector<bool> directions;

        auto current_old = root_;
        std::shared_ptr<Node> new_root = nullptr;

        while (current_old) {
            old_nodes.push_back(current_old);
            auto new_node = std::make_shared<Node>(current_old->get_value());
            new_nodes.push_back(new_node);
            path.push_back(new_node->get_value());

            if (!new_root) {
                new_root = new_node;
            } else {
                auto parent_new = new_nodes[new_nodes.size() - 2];
                if (directions.back()) {
                    parent_new->set_left(new_node);
                } else {
                    parent_new->set_right(new_node);
                }
            }

            if (comp_(key, current_old->get_value())) {
                current_old = current_old->get_left();
                directions.push_back(true);
            } else if (comp_(current_old->get_value(), key)) {
                current_old = current_old->get_right();
                directions.push_back(false);
            } else {
                path.clear();
                return root_;
            }
        }

        if (!old_nodes.empty()) {
            auto parent_new = new_nodes.back();
            auto new_node = std::make_shared<Node>(key);
            
            if (directions.back()) {
                parent_new->set_left(new_node);
            } else {
                parent_new->set_right(new_node);
            }
        }

        for (size_t i = 0; i < new_nodes.size(); ++i) {
            auto old_node = old_nodes[i];
            auto new_node = new_nodes[i];
            
            if (i < directions.size()) {
                if (directions[i]) {
                    new_node->set_right(old_node->get_right());
                } else {
                    new_node->set_left(old_node->get_left());
                }
            }
        }

        return new_root;
    }

public:
    void insert(const KeyType& value) {
        if (!root_) {
            root_ = std::make_shared<Node>(value);
            return;
        }

        auto [parent, where_to_insert] = find(value);
        if (where_to_insert == find_flag::exists)
            return;

        auto new_node = std::make_shared<Node>(value, parent);

        if (where_to_insert == find_flag::right)
            parent->set_right(new_node);
        else if (where_to_insert == find_flag::left)
            parent->set_left(new_node);
    }

    find_res find(const KeyType& value_to_find) const {
        std::shared_ptr<Node> current = root_;
        std::shared_ptr<Node> parent = nullptr;
        find_flag where_found = find_flag::exists;

        while (current) {
            if (value_to_find == current->get_value())
                return {current, find_flag::exists};

            parent = current;

            if (comp_(current->get_value(), value_to_find)) {
                current = current->get_right();
                where_found = find_flag::right;
            } else {
                current = current->get_left();
                where_found = find_flag::left;
            }
        }

        return {parent, where_found};
    }

    std::pair<BinarySearchTree, std::vector<KeyType>> insert_with_path_copying(const KeyType& key) const {
        if (!root_) {
            auto new_root = std::make_shared<Node>(key);
            return { BinarySearchTree{new_root}, {} };
        }
        
        std::vector<KeyType> path;
        auto new_root = copy_path(key, path);
        return { BinarySearchTree{new_root}, path };
    }

    void dump(std::ostream& os) const {
        std::stack<std::shared_ptr<Node>> stack;
        auto current = root_;
        while (current || !stack.empty()) {
            while (current) {
                stack.push(current);
                current = current->get_left();
            }
            current = stack.top();
            stack.pop();
            os << current->get_value() << " ";
            current = current->get_right();
        }
    }
};

template <typename KeyType, typename Compare>
std::ostream& operator<<(std::ostream& os, const BinarySearchTree<KeyType, Compare>& tree_for_dump) {
    tree_for_dump.dump(os);
    return os;
}

template <typename KeyType, typename Compare = std::less<KeyType>>
class PersistentTree final {
private:
    using BST = BinarySearchTree<KeyType, Compare>;
    std::shared_ptr<BST> current_version_ = std::make_shared<BST>();
    std::shared_ptr<BST> prev_version_ = nullptr;

public:
    PersistentTree() = default;

    void insert_k(const KeyType& key) { 
        prev_version_ = nullptr;
        auto [new_tree, path] = current_version_->insert_with_path_copying(key);
        current_version_ = std::make_shared<BST>(std::move(new_tree));
    }

    std::vector<KeyType> insert_s(const KeyType& key) {
        prev_version_ = nullptr;
        prev_version_ = current_version_;
        auto [new_tree, path] = current_version_->insert_with_path_copying(key);
        current_version_ = std::make_shared<BST>(std::move(new_tree));
        return path;
    }

    void rollback() {
        if (prev_version_) {
            current_version_ = prev_version_;
        }
    }

    void dump(std::ostream& os) const {
        os << "Current version in-order: ";
        current_version_->dump(os);
        os << "Prev version in-order: ";
        if (prev_version_) {
            prev_version_->dump(os);
        } else {
            os << "empty";
        }
    }
};

template <typename KeyType, typename Compare>
std::ostream& operator<<(std::ostream& os, const PersistentTree<KeyType, Compare>& tree_for_dump){
    tree_for_dump.dump(os);
    return os;
}

} // namespace tree