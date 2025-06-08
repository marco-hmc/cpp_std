#include <algorithm>
#include <iostream>
#include <iterator>

bool isOdd(int x) { return x % 2 != 0; }

int main() {
    int oddEven[] = {1, 2, 3, 4, 5, 6};

    // 使用 std::stable_partition 重新排列数组，使得奇数在前，偶数在后
    std::stable_partition(std::begin(oddEven), std::end(oddEven), isOdd);

    std::copy(std::begin(oddEven), std::end(oddEven),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;

    return 0;
}