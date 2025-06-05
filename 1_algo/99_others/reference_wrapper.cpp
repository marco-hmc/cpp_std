#include <functional>
#include <iostream>

/*
    1. reference_wrapper怎么用？
        基本上和&的那种引用类型一样用。
    2. 为什么有了引用类型，还要有std::reference_wrapper？
        * **可复制性**
            * 一般引用类型
                ```
                int a = 10, b = 20;
                int& ref = a;
                // ref 不能重新绑定到 b
                // ref = b; 这只是将 b 的值赋给 a，而不是让 ref 引用 b
                ```
            * reference_wrapper
                ```
                #include <functional>
                #include <iostream>

                int main() {
                    int a = 10, b = 20;
                    std::reference_wrapper<int> ref(a);
                    std::reference_wrapper<int> refCopy = ref;
                    refCopy.get() = 30;
                    std::cout << "a: " << a << std::endl; // 输出 30
                    ref = b; // 可以重新绑定到另一个对象
                    ref.get() = 40;
                    std::cout << "b: " << b << std::endl; // 输出 40
                    return 0;
                }
                ```
        * **可存储**
            * 一般引用类型
                ```
                #include <vector>
                int a = 10, b = 20;
                // 以下代码会编译错误，因为 std::vector 不能存储引用
                // std::vector<int&> vec = {a, b}; 
                ```
            * reference_wrapper
                ```
                #include <functional>
                #include <iostream>
                #include <vector>

                int main() {
                    int a = 10, b = 20;
                    std::vector<std::reference_wrapper<int>> vec = {a, b};
                    vec[0].get() = 30;
                    std::cout << "a: " << a << std::endl; // 输出 30
                    return 0;
                }
                ```
        * **按值传递语义的时候，保持引用语义**
            ```
            #include <functional>
            #include <iostream>

            // 模板函数
            template<typename T>
            void modify(T value) {
                value = 100;
            }

            int main() {
                int num = 20;
                // 直接传递引用，模板参数推导会丢失引用语义
                modify(num);
                std::cout << "num after direct call: " << num << std::endl; // 输出 20，num 未被修改

                // 使用 std::reference_wrapper 传递引用
                std::reference_wrapper<int> ref(num);
                modify(ref);
                std::cout << "num after reference_wrapper call: " << num << std::endl; // 输出 100，num 被修改

                return 0;
            }
            ```
*/
namespace reference_wrapperUsage {
    void task() {
        int value = 42;
        std::reference_wrapper<int> ref(value);

        ref.get() = 100;
        std::cout << "Original value: " << value << std::endl;

        auto func = [](std::reference_wrapper<int> r) { r.get() *= 2; };

        func(ref);
        std::cout << "Value after function call: " << value << std::endl;
    }
}  // namespace reference_wrapperUsage

namespace reference_wrapperImpl {
    template <typename T>
    class reference_wrapper {
      public:
        reference_wrapper(T& ref) : ptr(&ref) {}

        operator T&() const { return *ptr; }

        T& get() const { return *ptr; }

      private:
        T* ptr;
    };

    void task() {
        int value = 20;
        reference_wrapper<int> ref(value);

        ref.get() = 30;

        std::cout << "Original value in impl: " << value << std::endl;

        auto func = [](reference_wrapper<int> r) { r.get() *= 3; };

        func(ref);
        std::cout << "Value after function call in impl: " << value
                  << std::endl;
    }
}  // namespace reference_wrapperImpl

int main() {
    std::cout << "Usage of std::reference_wrapper:" << std::endl;
    reference_wrapperUsage::task();

    std::cout << "\nImplementation of reference_wrapper:" << std::endl;
    reference_wrapperImpl::task();

    return 0;
}