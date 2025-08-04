#include "Function.h"

#include <gtest/gtest.h>

TEST(FunctionTest, FunctionTestCase) {
    using namespace MyStd::FunctionImpl;

    Function<void()> f = []() { std::cout << "Hello, World!" << std::endl; };
    f();

    Function<int(int, int)> add = [](int a, int b) { return a + b; };
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;
}