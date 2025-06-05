#include <algorithm>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

static bool odd(int n) { return n % 2; }

int main() {
    std::vector<int> d(50000000);
    std::mt19937 gen;
    std::uniform_int_distribution<int> dis(0, 100000);
    auto rand_num([=]() mutable { return dis(gen); });

    std::generate(std::execution::par, std::begin(d), std::end(d), rand_num);
    std::sort(std::execution::par, std::begin(d), std::end(d));
    std::reverse(std::execution::par, std::begin(d), std::end(d));

    auto odds(
        std::count_if(std::execution::par, std::begin(d), std::end(d), odd));
    std::cout << (100.0 * odds / d.size()) << "% of the numbers are odd.\n";
}