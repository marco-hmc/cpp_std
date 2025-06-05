#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    // 它将指定位置的元素放置在其最终排序位置上，并且使得该元素之前的所有元素都不大于它，之后的所有元素都不小于它。
    // 使用 std::nth_element 将第 5 个元素放置在其最终排序位置上
    std::nth_element(numbers.begin(), numbers.begin() + 4, numbers.end());

    std::cout << "部分排序后的数组: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "第 5 个元素: " << numbers[4] << std::endl;

    return 0;
}