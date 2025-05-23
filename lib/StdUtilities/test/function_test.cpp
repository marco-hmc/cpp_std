#include "function.h"

#include <gtest/gtest.h>

#include <functional>
#include <string>

TEST(FunctionTest, StdFunctionBasicUsage) {
    std::function<int(int, int)> add = [](int a, int b) { return a + b; };
    EXPECT_TRUE(add);
    EXPECT_EQ(add(2, 3), 5);

    add = nullptr;
    EXPECT_FALSE(add);

    try {
        add(1, 1);
        FAIL() << "Expected std::bad_function_call";
    } catch (const std::bad_function_call& e) {
        SUCCEED();
    }
}

TEST(FunctionTest, FunctionImpl1BasicUsage) {
    using namespace MyStd::FunctionImpl1;

    Function<int(int, int)> add = [](int a, int b) { return a + b; };
    EXPECT_TRUE(add);
    EXPECT_EQ(add(2, 3), 5);

    add = nullptr;
    EXPECT_FALSE(add);

    try {
        add(1, 1);
        FAIL() << "Expected std::bad_function_call";
    } catch (const std::bad_function_call& e) {
        SUCCEED();
    }
}

TEST(FunctionTest, FunctionImpl2BasicUsage) {
    using namespace MyStd::FunctionImpl2;

    Function<int(int, int)> add = [](int a, int b) { return a + b; };
    EXPECT_TRUE(add);
    EXPECT_EQ(add(2, 3), 5);

    add = nullptr;
    EXPECT_FALSE(add);

    try {
        add(1, 1);
        FAIL() << "Expected std::bad_function_call";
    } catch (const std::bad_function_call& e) {
        SUCCEED();
    }
}

TEST(FunctionTest, FunctionImpl2AdvancedUsage) {
    using namespace MyStd::FunctionImpl2;

    Function<void(std::string&)> appendHello = [](std::string& str) {
        str += " Hello";
    };

    std::string testStr = "World";
    appendHello(testStr);
    EXPECT_EQ(testStr, "World Hello");

    appendHello = nullptr;
    EXPECT_FALSE(appendHello);

    try {
        appendHello(testStr);
        FAIL() << "Expected std::bad_function_call";
    } catch (const std::bad_function_call& e) {
        SUCCEED();
    }
}