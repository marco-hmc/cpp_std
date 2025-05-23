#include "any.h"

#include <gtest/gtest.h>

TEST(AnyTest, AnyTestImpl1) {
    using namespace MyStd::AnyImpl1;
    {
        Any a = 42;
        EXPECT_EQ(a.get<int>(), 42);

        a = std::string("Hello, World!");
        EXPECT_EQ(a.get<std::string>(), "Hello, World!");

        try {
            std::cout << a.get<int>() << std::endl;
            FAIL() << "Expected std::bad_any_cast";
        } catch (const std::bad_cast& e) {
            EXPECT_STREQ(e.what(), "bad any_cast");
        }
    }

    {
        std::vector<Any> container;
        container.emplace_back(42);
        container.emplace_back(std::string("Hello, World!"));
        container.emplace_back(3.14);

        EXPECT_EQ(container[0].get<int>(), 42);
        EXPECT_EQ(container[1].get<std::string>(), "Hello, World!");
        EXPECT_EQ(container[2].get<double>(), 3.14);

        try {
            std::cout << container[2].get<float>() << std::endl;
            FAIL() << "Expected std::bad_any_cast";
        } catch (const std::bad_cast& e) {
            EXPECT_STREQ(e.what(), "bad any_cast");
        }
    }
}

TEST(AnyTest, AnyTestImpl2) {
    using namespace MyStd::AnyImpl2;
    {
        Any a = 42;
        EXPECT_EQ(a.get<int>(), 42);

        a = std::string("Hello, World!");
        EXPECT_EQ(a.get<std::string>(), "Hello, World!");

        try {
            std::cout << a.get<int>() << std::endl;
            FAIL() << "Expected std::bad_any_cast";
        } catch (const std::bad_cast& e) {
            EXPECT_STREQ(e.what(), "bad any_cast");
        }
    }

    {
        std::vector<Any> container;
        container.emplace_back(42);
        container.emplace_back(std::string("Hello, World!"));
        container.emplace_back(3.14);

        EXPECT_EQ(container[0].get<int>(), 42);
        EXPECT_EQ(container[1].get<std::string>(), "Hello, World!");
        EXPECT_EQ(container[2].get<double>(), 3.14);

        try {
            std::cout << container[2].get<float>() << std::endl;
            FAIL() << "Expected std::bad_any_cast";
        } catch (const std::bad_cast& e) {
            EXPECT_STREQ(e.what(), "bad any_cast");
        }
    }
}

TEST(StdAnyTest, BasicUsage) {
    {
        std::any a = 42;
        EXPECT_TRUE(a.has_value());
        EXPECT_EQ(std::any_cast<int>(a), 42);

        a = std::string("Hello, World!");
        EXPECT_TRUE(a.has_value());
        EXPECT_EQ(std::any_cast<std::string>(a), "Hello, World!");

        try {
            std::any_cast<int>(a);
            FAIL() << "Expected std::bad_any_cast";
        } catch (const std::bad_any_cast& e) {
            EXPECT_STREQ(e.what(), "bad any_cast");
        }
    }
    {
        std::vector<std::any> container;
        container.emplace_back(42);
        container.emplace_back(std::string("Hello"));
        container.emplace_back(3.14);

        EXPECT_EQ(std::any_cast<int>(container[0]), 42);
        EXPECT_EQ(std::any_cast<std::string>(container[1]), "Hello");
        EXPECT_EQ(std::any_cast<double>(container[2]), 3.14);

        try {
            std::any_cast<float>(container[2]);
            FAIL() << "Expected std::bad_any_cast";
        } catch (const std::bad_any_cast& e) {
            EXPECT_STREQ(e.what(), "bad any_cast");
        }
    }
}