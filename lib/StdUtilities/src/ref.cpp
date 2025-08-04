#include <functional>
#include <iostream>

/*
2. reference_wrapper 怎么用？
    reference_wrapper 是一个模板类，用于包装引用类型。
    它可以用于将引用传递给需要值类型的函数或容器。
*/

namespace referenceWrapperUsage {
    // reference_wrapper 的实现
    template <typename T>
    class reference_wrapper {
      public:
        explicit reference_wrapper(T& ref) : ptr_(std::addressof(ref)) {}

        operator T&() const { return *ptr_; }
        T& get() const { return *ptr_; }

      private:
        T* ptr_;
    };

    // ref 函数模板
    template <typename T>
    reference_wrapper<T> ref(T& ref) {
        return reference_wrapper<T>(ref);
    }

    void test() {
        int x = 5;
        reference_wrapper<int> ref_x = ref(x);

        ref_x.get() = 10;

        std::cout << "x: " << x << '\n';  // 输出: x: 10
    }
}  // namespace referenceWrapperUsage

////////////////////////////////////////////////////////////////////
namespace referenceWrapperImpl {
    // reference_wrapper 的实现
    template <typename T>
    class reference_wrapper {
      public:
        explicit reference_wrapper(T& ref) : ptr_(std::addressof(ref)) {}

        operator T&() const { return *ptr_; }
        T& get() const { return *ptr_; }

      private:
        T* ptr_;
    };

    // ref 函数模板
    template <typename T>
    reference_wrapper<T> ref(T& ref) {
        return reference_wrapper<T>(ref);
    }

    void test() {
        int x = 5;
        reference_wrapper<int> ref_x = ref(x);

        ref_x.get() = 10;

        std::cout << "x: " << x << '\n';  // 输出: x: 10
    }
}  // namespace referenceWrapperImpl

int main() {
    referenceWrapperUsage::test();
    referenceWrapperImpl::test();
    return 0;
}