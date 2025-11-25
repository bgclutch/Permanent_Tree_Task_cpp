#pragma once 

#include <algorithm>
#include <iterator>
#include <iostream>

namespace utils {

template <class Iterator>
void print_container(const Iterator begin, const Iterator end) {
    std::copy(begin, end, std::ostream_iterator<typename std::iterator_traits<Iterator>::value_type>(std::cout, " "));
}

}
