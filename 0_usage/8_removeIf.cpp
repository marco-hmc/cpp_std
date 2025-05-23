#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto it = std::remove_if(numbers.begin(), numbers.end(),
                             [](int x) { return x % 2 == 0; });

    numbers.erase(it, numbers.end());
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}