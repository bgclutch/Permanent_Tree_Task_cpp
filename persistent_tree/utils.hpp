#pragma once 

#include <algorithm>

namespace utils {

template <class Container>
void print_container(const Container& c) {
    std::for_each(c.begin(), c.end(), 
                [](const typename Container::value_type x) { std::cout << x << " "; });
}

}
