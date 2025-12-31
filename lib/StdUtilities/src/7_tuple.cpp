// /home/marco/0_noteWork/4_subProjects/2_std/lib/StdUtilities/src/7_tuple.cpp
// 示例：C++ std::tuple 的常见用法 （需要 C++17 及以上）
#include <tuple>
#include <string>
#include <iostream>
#include <utility> // std::apply
#include <vector>

int add_three(int a, int b, int c) {
    return a + b + c;
}

int main() {
    // 1. 创建 tuple
    std::tuple<int, std::string, double> t1(42, "hello", 3.14);
    auto t2 = std::make_tuple(1, std::string("world"), 2.718);

    // 2. 访问元素：std::get
    int v0 = std::get<0>(t1);
    std::string s1 = std::get<1>(t1);
    double d1 = std::get<2>(t1);
    std::cout << "t1: " << v0 << ", " << s1 << ", " << d1 << "\n";

    // 3. 结构化绑定（C++17）
    auto [a, b, c] = t2;
    std::cout << "t2 unpacked: " << a << ", " << b << ", " << c << "\n";

    // 4. std::tie：把 tuple 拆到已有变量，配合 std::ignore 忽略某些元素
    int x; std::string y; double z;
    std::tie(x, y, z) = t1;
    std::cout << "tied: " << x << ", " << y << ", " << z << "\n";

    // 忽略中间元素
    std::tie(x, std::ignore, z) = t2;
    std::cout << "ignored middle: " << x << ", " << z << "\n";

    // 5. std::apply：把 tuple 展开作为函数参数
    auto args = std::make_tuple(10, 20, 30);
    int sum = std::apply(add_three, args);
    std::cout << "sum via apply: " << sum << "\n";

    // 6. tuple_cat：合并 tuples
    auto cat = std::tuple_cat(t1, t2); // 长度为 6 的 tuple
    std::cout << "cat first element: " << std::get<0>(cat) << "\n";

    // 7. 比较和交换
    auto t3 = std::make_tuple(1, std::string("z"), 0.5);
    auto t4 = std::make_tuple(2, std::string("a"), 0.5);
    std::cout << "t3 < t4 ? " << (t3 < t4) << "\n";
    std::swap(t3, t4);
    std::cout << "after swap, t3 first: " << std::get<0>(t3) << "\n";

    // 8. 与 std::pair 的互操作（pair<->tuple）
    std::pair<int, std::string> p = {7, "seven"};
    auto tup_from_pair = std::tuple<int, std::string>(p.first, p.second);
    std::cout << "from pair: " << std::get<0>(tup_from_pair) << ", " << std::get<1>(tup_from_pair) << "\n";

    // 9. 用 tuple 存放不同类型并放入容器（要包装为对象）
    std::vector<std::tuple<int, std::string>> vec;
    vec.emplace_back(100, "one-hundred");
    for (const auto &it : vec) {
        std::cout << "vec element: " << std::get<0>(it) << ", " << std::get<1>(it) << "\n";
    }

    return 0;
}