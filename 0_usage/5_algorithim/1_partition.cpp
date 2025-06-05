#include <algorithm>
#include <iostream>
#include <vector>

template <class ForwardIt, class UnaryPred>
ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPred p) {
    // 找到第一个不满足条件的元素
    first = std::find_if_not(first, last, p);
    if (first == last) return first;

    // 从剩下的再找满足条件的，不满足条件的迭代器是first，再后退。
    for (auto i = std::next(first); i != last; ++i)
        if (p(*i)) {
            std::iter_swap(i, first);
            ++first;
        }

    return first;
}

bool isOdd(int n) { return n % 2 != 0; }

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

    auto it = std::partition(numbers.begin(), numbers.end(), isOdd);

    std::cout << "重新排列后的数组: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    std::cout << "第一个不满足条件的元素的位置: "
              << std::distance(numbers.begin(), it) << std::endl;

    return 0;
}