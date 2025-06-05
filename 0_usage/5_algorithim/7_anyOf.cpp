#include <algorithm>
#include <cassert>
#include <vector>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    bool anyEven = std::any_of(numbers.begin(), numbers.end(),
                               [](int i) { return i % 2 == 0; });
    assert(anyEven == true);

    return 0;
}