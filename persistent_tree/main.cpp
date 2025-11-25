#include "persistent_tree.hpp"
#include "utils.hpp"

#include <iostream>
#include <limits>

void clearInput();

int main() {
    tree::PersistentTree<int> tree;
    char request;

    while (std::cin >> request) {
        if (request == tree::key_request) {
            int newKey;
            while (!(std::cin >> newKey)) {
                std::cerr << "WRONG GIVEN KEY -> " << newKey << "\n";
                clearInput();
            }
            tree.insert_k(newKey);
        }
        else if (request == tree::save_request) {
            std::cin >> request;
            if (request != tree::key_request) {
                std::cerr << "WRONG REQUEST -> " << request << "\n";
                clearInput();
            } 
            int newKey;
            while (!(std::cin >> newKey)) {
                std::cerr << "WRONG GIVEN KEY -> " << newKey << "\n";
                clearInput();
            }
            utils::print_container(tree.insert_s(newKey));
        }
        else if (request == tree::rollback_request) {
            tree.rollback();
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            clearInput();
        }
    }
    std::cout << std::endl;
    return EXIT_SUCCESS;
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
