#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

std::vector<int> selectBySorting(const std::vector<int>& input, int k1,
                                 int k2) {
    assert(0 <= k1 && k1 < input.size());
    assert(0 <= k2 && k2 < input.size());
    assert(k1 <= k2);

    std::vector<int> temp(input);
    std::sort(temp.begin(), temp.end());
    return std::vector<int>(temp.begin() + k1, temp.begin() + k2 + 1);
}

std::vector<int> selectByNthElement(const std::vector<int>& input, int k1,
                                    int k2) {
    assert(0 <= k1 && k1 < input.size());
    assert(0 <= k2 && k2 < input.size());
    assert(k1 <= k2);

    std::vector<int> temp(input);
    std::nth_element(temp.begin(), temp.begin() + k2 + 1, temp.end());
    std::nth_element(temp.begin(), temp.begin() + k1, temp.begin() + k2 + 1);
    std::sort(temp.begin() + k1, temp.begin() + k2 + 1);
    return std::vector<int>(temp.begin() + k1, temp.begin() + k2 + 1);
}

std::vector<int> selectByPartialSort(const std::vector<int>& input, int k1,
                                     int k2) {
    assert(0 <= k1 && k1 < input.size());
    assert(0 <= k2 && k2 < input.size());
    assert(k1 <= k2);

    std::vector<int> temp(input);
    std::nth_element(temp.begin(), temp.begin() + k1, temp.end());
    std::partial_sort(temp.begin() + k1, temp.begin() + k2 + 1, temp.end());
    return std::vector<int>(temp.begin() + k1, temp.begin() + k2 + 1);
}

void print(const std::vector<int>& vec) {
    std::copy(vec.begin(), vec.end(),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
}

int main() {
    std::vector<int> input;
    for (int i = 0; i < 100; ++i) {
        input.push_back(i);
    }
    std::shuffle(input.begin(), input.end(),
                 std::mt19937(std::random_device()()));

    print(input);
    print(selectBySorting(input, 10, 20));
    print(selectByNthElement(input, 10, 20));
    print(selectByPartialSort(input, 10, 20));
}
