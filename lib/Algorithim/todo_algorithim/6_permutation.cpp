#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> v{std::istream_iterator<std::string>{std::cin},
                               {}};
    std::sort(std::begin(v), std::end(v));

    do {
        std::copy(std::begin(v), std::end(v),
                  std::ostream_iterator<std::string>{std::cout, ", "});
        std::cout << '\n';
    } while (std::next_permutation(std::begin(v), std::end(v)));

    return 0;
}