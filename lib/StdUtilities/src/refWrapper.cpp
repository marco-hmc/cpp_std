#include <iostream>
#include <vector>

template <typename T>
class reference_wrapper {
   public:
    explicit reference_wrapper(T& ref) : ptr_(std::addressof(ref)) {}
    explicit operator T&() const { return *ptr_; }
    T& get() const { return *ptr_; }

   public:
    reference_wrapper<T>& operator=(const T& value) {
        *ptr_ = value;
        return *this;
    }

   private:
    T* ptr_;
};

template <typename T>
reference_wrapper<T> ref(T& ref) {
    return reference_wrapper<T>(ref);
}

int main() {
    int x = 5;
    int y = 10;

    // 使用 reference_wrapper 存储引用
    std::vector<reference_wrapper<int>> vec;
    vec.push_back(ref(x));
    vec.push_back(ref(y));

    // 修改引用的值
    vec[0] = 20;
    vec[1] = 30;

    // 打印原始变量的值
    std::cout << "x: " << x << '\n';  // Output: x: 20
    std::cout << "y: " << y << '\n';  // Output: y: 30

    return 0;
}