#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// each element < 10000
// LSB first, ie. little endian
using BigInt = std::vector<int>;

BigInt factorial(int n) {
    // 9999 * 9999 < 10000 * 10000 = 100000000 < 1073741824  = 2 ** 30
    assert(n >= 0 && n <= 10000);

    BigInt result;
    result.emplace_back(1);
    for (int factor = 1; factor <= n; ++factor) {
        int carry = 0;
        for (auto& item : result) {
            int product = item * factor + carry;
            item = product % 10000;
            carry = product / 10000;
        }
        if (carry > 0) {
            result.emplace_back(carry);
        }
    }
    return result;
}

void printBigInt(const BigInt& number) {
    if (number.empty()) {
        std::cout << "0\n";
    } else {
        std::cout << number.back();
        for (auto it = number.rbegin() + 1; it != number.rend(); ++it) {
            std::cout
                << std::to_string(*it + 10000).substr(1);  // 保证输出4位数
        }
        std::cout << "\n";
    }
}

int main() {
    /*
    for (int i = 0; i <= 10000; ++i) {
        BigInt result = factorial(i);
        std::cout << i << ": ";
        printBigInt(result);
    }
    */
    BigInt result = factorial(10000);
    printBigInt(result);
}