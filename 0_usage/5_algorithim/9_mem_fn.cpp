#include <functional>
#include <iostream>

/*
    std::mem_fn 是一个函数模板，用于生成一个可调用对象，该对象可以调用给定的成员函数指针或成员变量指针。
    它可以用于将成员函数或成员变量绑定到一个对象上，从而可以像普通函数一样调用它们。
*/

namespace memFnUsage {

    class MyClass {
      public:
        int value;
        MyClass(int v) : value(v) {}

        void printValue() const {
            std::cout << "Value: " << value << std::endl;
        }
    };

    void test() {
        MyClass obj(42);

        // 使用 std::mem_fn 访问成员变量
        auto memFnValue = std::mem_fn(&MyClass::value);
        std::cout << "Member variable value: " << memFnValue(obj) << std::endl;

        // 使用 std::mem_fn 访问成员函数
        auto memFnPrint = std::mem_fn(&MyClass::printValue);
        memFnPrint(obj);
    }

}  // namespace memFnUsage

namespace memFnImpl {

    template <typename T>
    class mem_fn_t {
        T ptr;

      public:
        explicit mem_fn_t(T p) : ptr(p) {}

        template <typename Obj, typename... Args>
        auto operator()(Obj&& obj, Args&&... args) const
            -> decltype((std::forward<Obj>(obj).*
                         ptr)(std::forward<Args>(args)...)) {
            return (std::forward<Obj>(obj).*ptr)(std::forward<Args>(args)...);
        }

        template <typename Obj>
        auto operator()(Obj&& obj) const -> decltype((std::forward<Obj>(obj).*
                                                      ptr)) {
            return (std::forward<Obj>(obj).*ptr);
        }
    };

    template <typename T>
    mem_fn_t<T> mem_fn(T p);

    class MyClass {
      public:
        int value;
        explicit MyClass(int v) : value(v) {}

        void printValue() const {
            std::cout << "Value: " << value << std::endl;
        }
    };

    template <typename T>
    mem_fn_t<T> mem_fn(T p) {
        return mem_fn_t<T>(p);
    }

    void test() {
        MyClass obj(42);

        // 使用自定义的 mem_fn 访问成员变量
        auto memFnValue = mem_fn(&MyClass::value);
        std::cout << "Member variable value: " << memFnValue(obj) << std::endl;

        // 使用自定义的 mem_fn 访问成员函数
        auto memFnPrint = mem_fn(&MyClass::printValue);
        memFnPrint(obj);
    }

}  // namespace memFnImpl

int main() {
    memFnUsage::test();
    memFnImpl::test();
    return 0;
}