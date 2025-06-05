#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <class ForwardIt>
ForwardIt unique(ForwardIt first, ForwardIt last) {
    if (first == last) return last;

    ForwardIt result = first;
    while (++first != last) {
        if (!(*result == *first)) {
            *(++result) = std::move(*first);
        }
    }
    return ++result;
}

// 这个谓语的作用是什么？
template <class ForwardIt, class BinaryPredicate>
ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p) {
    if (first == last) return last;

    ForwardIt result = first;
    while (++first != last)
        if (!p(*result, *first) && ++result != first)
            *result = std::move(*first);

    return ++result;
}

int main() {
    std::vector<int> numbers = {1, 2, 2, 3, 4, 4, 5, 6, 6};

    auto it = std::unique(numbers.begin(), numbers.end());

    std::cout << "去重后的数组: ";
    std::copy(numbers.begin(), it, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}