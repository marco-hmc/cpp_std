// std::copy 用法详解和自定义实现

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <deque>

namespace std {
std::ostream &operator<<(std::ostream &os,
                         const std::pair<int, std::string> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}
}  // namespace std

namespace STD_USAGE {
    void task() {
        std::cout << "=== STD::COPY 标准用法 ===" << std::endl;
        
        // 1. 基本复制 - vector 到 vector
        std::vector<int> source = {1, 2, 3, 4, 5};
        std::vector<int> destination(source.size());
        
        std::cout << "基本复制:" << std::endl;
        std::cout << "源数组: ";
        for (int n : source) std::cout << n << " ";
        std::cout << std::endl;
        
        std::copy(source.begin(), source.end(), destination.begin());
        
        std::cout << "目标数组: ";
        for (int n : destination) std::cout << n << " ";
        std::cout << std::endl;
        
        // 2. copy_n - 复制指定数量的元素
        std::vector<int> partial(3);
        std::cout << "\ncopy_n 复制前3个元素:" << std::endl;
        std::copy_n(source.begin(), 3, partial.begin());
        
        std::cout << "部分复制结果: ";
        for (int n : partial) std::cout << n << " ";
        std::cout << std::endl;
        
        // 3. copy_if - 条件复制
        std::vector<int> odds;
        std::cout << "\ncopy_if 复制奇数:" << std::endl;
        std::copy_if(source.begin(), source.end(), 
                    std::back_inserter(odds),
                    [](int x) { return x % 2 != 0; });
        
        std::cout << "奇数复制结果: ";
        for (int n : odds) std::cout << n << " ";
        std::cout << std::endl;
        
        // 4. copy_backward - 反向复制
        std::vector<int> backward_dest(source.size() + 2);
        std::cout << "\ncopy_backward 反向复制:" << std::endl;
        std::copy_backward(source.begin(), source.end(), backward_dest.end());
        
        std::cout << "反向复制结果: ";
        for (int n : backward_dest) std::cout << n << " ";
        std::cout << std::endl;
        
        // 5. 使用 ostream_iterator 输出到控制台
        std::cout << "\n使用 ostream_iterator 直接输出: ";
        std::copy(source.begin(), source.end(), 
                 std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }
}

namespace MY_IMPL {
    // 自定义实现 copy
    template<class InputIt, class OutputIt>
    OutputIt my_copy(InputIt first, InputIt last, OutputIt d_first) {
        while (first != last) {
            *d_first++ = *first++;
        }
        return d_first;
    }
    
    // 自定义实现 copy_n
    template<class InputIt, class Size, class OutputIt>
    OutputIt my_copy_n(InputIt first, Size count, OutputIt result) {
        for (Size i = 0; i < count; ++i) {
            *result++ = *first++;
        }
        return result;
    }
    
    // 自定义实现 copy_if
    template<class InputIt, class OutputIt, class UnaryPredicate>
    OutputIt my_copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred) {
        while (first != last) {
            if (pred(*first)) {
                *d_first++ = *first;
            }
            ++first;
        }
        return d_first;
    }
    
    void task() {
        std::cout << "\n=== 自定义 COPY 实现测试 ===" << std::endl;
        
        // 测试自定义 copy
        std::vector<int> source = {10, 20, 30, 40, 50};
        std::vector<int> dest1(source.size());
        
        std::cout << "自定义 copy 测试:" << std::endl;
        my_copy(source.begin(), source.end(), dest1.begin());
        
        std::cout << "复制结果: ";
        for (int n : dest1) std::cout << n << " ";
        std::cout << std::endl;
        
        // 测试自定义 copy_n
        std::vector<int> dest2(3);
        std::cout << "\n自定义 copy_n 测试:" << std::endl;
        my_copy_n(source.begin(), 3, dest2.begin());
        
        std::cout << "copy_n 结果: ";
        for (int n : dest2) std::cout << n << " ";
        std::cout << std::endl;
        
        // 测试自定义 copy_if
        std::vector<int> dest3;
        std::cout << "\n自定义 copy_if 测试 (复制>25的元素):" << std::endl;
        my_copy_if(source.begin(), source.end(), 
                   std::back_inserter(dest3),
                   [](int x) { return x > 25; });
        
        std::cout << "copy_if 结果: ";
        for (int n : dest3) std::cout << n << " ";
        std::cout << std::endl;
    }
}

namespace MOVE_VS_COPY {
    void task() {
        std::cout << "\n=== MOVE vs COPY 对比 ===" << std::endl;
        
        // 准备测试数据
        std::vector<std::string> original = {"hello", "world", "cpp", "move", "copy"};
        std::vector<std::string> copy_dest;
        std::vector<std::string> move_dest;
        
        std::cout << "原数据: ";
        for (const auto& s : original) std::cout << s << " ";
        std::cout << std::endl;
        
        // copy 操作 - 保留原数据
        std::copy(original.begin(), original.end(), std::back_inserter(copy_dest));
        
        std::cout << "\ncopy 后:" << std::endl;
        std::cout << "原数据: ";
        for (const auto& s : original) std::cout << s << " ";
        std::cout << "\n目标数据: ";
        for (const auto& s : copy_dest) std::cout << s << " ";
        std::cout << std::endl;
        
        // move 操作 - 可能移动原数据
        std::move(original.begin(), original.end(), std::back_inserter(move_dest));
        
        std::cout << "\nmove 后:" << std::endl;
        std::cout << "原数据: ";
        for (const auto& s : original) std::cout << s << " ";
        std::cout << "\n目标数据: ";
        for (const auto& s : move_dest) std::cout << s << " ";
        std::cout << std::endl;
        
        std::cout << "说明:" << std::endl;
        std::cout << "- copy: 复制元素，原数据保持不变" << std::endl;
        std::cout << "- move: 移动元素，原数据可能变为未定义状态" << std::endl;
        std::cout << "- move 通常用于优化性能，避免不必要的复制" << std::endl;
    }
}

namespace ITERATOR_ADAPTERS {
    void task() {
        std::cout << "\n=== 迭代器适配器详解 ===" << std::endl;
        
        std::vector<int> source = {1, 2, 3, 4, 5};
        
        // 1. back_inserter - 在容器末尾插入
        std::vector<int> dest1;
        std::cout << "back_inserter 使用:" << std::endl;
        std::copy(source.begin(), source.end(), std::back_inserter(dest1));
        std::cout << "结果: ";
        for (int n : dest1) std::cout << n << " ";
        std::cout << std::endl;
        
        // 2. front_inserter - 在容器开头插入 (仅支持 deque, list)
        std::deque<int> dest2;
        std::cout << "\nfront_inserter 使用 (deque):" << std::endl;
        std::copy(source.begin(), source.end(), std::front_inserter(dest2));
        std::cout << "结果 (注意顺序): ";
        for (int n : dest2) std::cout << n << " ";
        std::cout << std::endl;
        
        // 3. inserter - 在指定位置插入
        std::vector<int> dest3 = {10, 20};
        std::cout << "\ninserter 使用 (在位置1插入):" << std::endl;
        std::cout << "原容器: ";
        for (int n : dest3) std::cout << n << " ";
        std::cout << std::endl;
        
        std::copy(source.begin(), source.end(), 
                 std::inserter(dest3, dest3.begin() + 1));
        std::cout << "插入后: ";
        for (int n : dest3) std::cout << n << " ";
        std::cout << std::endl;
        
        // 4. ostream_iterator - 输出到流
        std::cout << "\nostream_iterator 使用:" << std::endl;
        std::cout << "直接输出: ";
        std::copy(source.begin(), source.end(),
                 std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;
    }
}

namespace ADVANCED_USAGE {
    void task() {
        std::cout << "\n=== COPY 高级用法 ===" << std::endl;
        
        // 1. 不同容器类型间复制
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::list<int> lst;
        std::deque<int> deq;
        
        std::cout << "不同容器类型间复制:" << std::endl;
        std::copy(vec.begin(), vec.end(), std::back_inserter(lst));
        std::copy(vec.begin(), vec.end(), std::back_inserter(deq));
        
        std::cout << "vector: ";
        for (int n : vec) std::cout << n << " ";
        std::cout << "\nlist: ";
        for (int n : lst) std::cout << n << " ";
        std::cout << "\ndeque: ";
        for (int n : deq) std::cout << n << " ";
        std::cout << std::endl;
        
        // 2. 复制到关联容器
        std::vector<std::pair<int, std::string>> pairs = {
            {1, "one"}, {2, "two"}, {3, "three"}
        };
        std::map<int, std::string> map_dest;
        
        std::cout << "\n复制到关联容器:" << std::endl;
        std::copy(pairs.begin(), pairs.end(), 
                 std::inserter(map_dest, map_dest.begin()));
        
        std::cout << "map 内容: ";
        for (const auto& p : map_dest) {
            std::cout << "(" << p.first << ":" << p.second << ") ";
        }
        std::cout << std::endl;
        
        // 3. 性能考虑
        std::cout << "\n性能考虑:" << std::endl;
        std::cout << "- copy: 适用于所有可复制的类型" << std::endl;
        std::cout << "- move: 适用于可移动的类型，性能更好" << std::endl;
        std::cout << "- 对于大对象或昂贵复制操作，优先考虑 move" << std::endl;
        std::cout << "- 对于基本类型，copy 和 move 性能相当" << std::endl;
    }
}

int main() {
    // ...existing code... (原始示例)
    std::vector<std::pair<int, std::string>> v{
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};

    std::map<int, std::string> m;
    std::copy_n(std::begin(v), 3, std::inserter(m, std::begin(m)));
    auto shell_it(
        std::ostream_iterator<std::pair<int, std::string>>{std::cout, ", "});
    
    std::cout << "=== 原始示例 ===" << std::endl;
    std::cout << "copy_n 结果: ";
    std::copy(std::begin(m), std::end(m), shell_it);
    std::cout << '\n';

    m.clear();
    std::move(std::begin(v), std::end(v), std::inserter(m, std::begin(m)));
    std::cout << "move 结果: ";
    std::copy(std::begin(m), std::end(m), shell_it);
    std::cout << '\n';

    std::cout << "move 后原vector: ";
    std::copy(std::begin(v), std::end(v), shell_it);
    std::cout << '\n';

    // 调用详细示例
    STD_USAGE::task();
    MY_IMPL::task();
    MOVE_VS_COPY::task();
    ITERATOR_ADAPTERS::task();
    ADVANCED_USAGE::task();

    return 0;
}