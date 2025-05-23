#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

/*
1. 如果不使用back_inserter，需要自己保证output容器的大小等同于input容器大小。

*/

namespace identifier {
    void transform_test() {
        std::vector<int> input = {1, 2, 3, 4, 5};
        std::vector<int> output;

        std::transform(input.begin(), input.end(), std::back_inserter(output),
                       [](int x) { return x * 2; });

        // 输出结果
        for (int n : output) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
}  // namespace identifier

namespace identifier2 {
    void transform_test() {
        std::vector<int> input = {1, 2, 3, 4, 5};
        std::vector<int> output(input.size());

        std::transform(input.begin(), input.end(), output.begin(),
                       [](int x) { return x * 2; });

        for (int n : output) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
}  // namespace identifier2

int main() { return 0; }