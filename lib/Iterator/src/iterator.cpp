#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

namespace iterator_basic_transform {

    void basic_usage() {
        std::cout << "=== Iterator 基础用法 ===" << std::endl;

        std::vector<int> vec = {1, 2, 3, 4, 5};

        // 1. 基本迭代器操作
        std::cout << "正向遍历: ";
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;

        // 2. 反向迭代器
        std::cout << "反向遍历: ";
        for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;

        // 3. 距离和前进
        auto it1 = vec.begin();
        auto it2 = vec.begin();
        std::advance(it2, 3);
        std::cout << "距离: " << std::distance(it1, it2) << std::endl;
    }

    void iterator_categories() {
        std::cout << "\n=== Iterator 类别演示 ===" << std::endl;

        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::map<int, std::string> map_data = {{1, "one"}, {2, "two"}};

        // 随机访问迭代器
        auto vec_it = vec.begin();
        std::cout << "随机访问: vec[2] = " << *(vec_it + 2) << std::endl;

        // 双向迭代器
        auto map_it = map_data.begin();
        ++map_it;
        --map_it;
        std::cout << "双向访问: " << map_it->first << " -> " << map_it->second
                  << std::endl;
    }

}  // namespace iterator_basic_transform

namespace fibonacci_iterator_optimized {

    // 优化的斐波那契迭代器
    class fibonacci_iterator {
      public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

      private:
        std::size_t index_{0};
        mutable std::size_t current_{0};
        mutable std::size_t next_{1};
        mutable bool computed_{false};

      public:
        constexpr fibonacci_iterator() = default;
        constexpr explicit fibonacci_iterator(std::size_t index)
            : index_(index) {}

        constexpr reference operator*() const {
            compute_current();
            return current_;
        }

        constexpr pointer operator->() const {
            compute_current();
            return &current_;
        }

        constexpr fibonacci_iterator& operator++() {
            compute_current();
            std::size_t old_next = next_;
            next_ += current_;
            current_ = old_next;
            ++index_;
            computed_ = true;
            return *this;
        }

        constexpr fibonacci_iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        constexpr bool operator==(
            const fibonacci_iterator& other) const noexcept {
            return index_ == other.index_;
        }

        constexpr bool operator!=(
            const fibonacci_iterator& other) const noexcept {
            return !(*this == other);
        }

        constexpr std::size_t index() const noexcept { return index_; }

      private:
        constexpr void compute_current() const {
            if (!computed_ && index_ > 0) {
                // 快速计算到当前位置
                std::size_t a = 0, b = 1;
                for (std::size_t i = 1; i < index_; ++i) {
                    std::size_t temp = b;
                    b += a;
                    a = temp;
                }
                current_ = b;
                next_ = a + b;
                computed_ = true;
            }
        }
    };

    // 斐波那契范围类
    class fibonacci_range {
      private:
        std::size_t count_;

      public:
        constexpr explicit fibonacci_range(std::size_t count) : count_(count) {}

        constexpr fibonacci_iterator begin() const {
            return fibonacci_iterator{0};
        }
        constexpr fibonacci_iterator end() const {
            return fibonacci_iterator{count_};
        }
        constexpr std::size_t size() const noexcept { return count_; }
        constexpr bool empty() const noexcept { return count_ == 0; }
    };

    void demonstrate() {
        std::cout << "\n=== 优化的斐波那契迭代器 ===" << std::endl;

        std::cout << "前10个斐波那契数: ";
        for (auto fib : fibonacci_range(10)) {
            std::cout << fib << " ";
        }
        std::cout << std::endl;

        // 使用STL算法
        fibonacci_range range(15);
        auto count = std::count_if(range.begin(), range.end(),
                                   [](std::size_t n) { return n % 2 == 0; });
        std::cout << "前15个数中偶数个数: " << count << std::endl;
    }

}  // namespace fibonacci_iterator_optimized

namespace iterator_merge_optimized {

    // 通用合并迭代器模板
    template <typename Container,
              typename Compare = std::less<typename Container::value_type>>
    class merge_iterator {
      public:
        using value_type = typename Container::value_type;
        using reference = const value_type&;
        using pointer = const value_type*;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;

      private:
        struct iterator_item {
            typename Container::const_iterator current;
            typename Container::const_iterator end;
            std::size_t container_index;

            bool operator<(const iterator_item& other) const {
                // 注意：heap是最大堆，所以要反转比较
                if (current == end) return false;
                if (other.current == other.end) return true;

                Compare comp;
                if (comp(*current, *other.current) ||
                    comp(*other.current, *current)) {
                    return comp(*other.current, *current);
                }
                // 相等时，使用容器索引保持稳定性
                return container_index > other.container_index;
            }
        };

        std::vector<iterator_item> heap_;
        Compare compare_;

      public:
        explicit merge_iterator(const std::vector<Container>& containers,
                                Compare comp = Compare{})
            : compare_(comp) {
            heap_.reserve(containers.size());

            for (std::size_t i = 0; i < containers.size(); ++i) {
                if (!containers[i].empty()) {
                    heap_.push_back(
                        {containers[i].cbegin(), containers[i].cend(), i});
                }
            }
            std::make_heap(heap_.begin(), heap_.end());
        }

        bool exhausted() const noexcept { return heap_.empty(); }

        reference operator*() const {
            assert(!exhausted());
            return *heap_.front().current;
        }

        pointer operator->() const {
            assert(!exhausted());
            return &(*heap_.front().current);
        }

        merge_iterator& operator++() {
            assert(!exhausted());

            std::pop_heap(heap_.begin(), heap_.end());

            if (++heap_.back().current == heap_.back().end) {
                heap_.pop_back();
            } else {
                std::push_heap(heap_.begin(), heap_.end());
            }

            return *this;
        }

        merge_iterator operator++(int) {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        // 辅助方法
        std::size_t current_container_index() const {
            assert(!exhausted());
            return heap_.front().container_index;
        }
    };

    void demonstrate() {
        std::cout << "\n=== 优化的合并迭代器 ===" << std::endl;

        using Map = std::map<int, std::string>;
        std::vector<Map> maps(3);

        maps[0] = {{1, "1.a"}, {3, "3.a"}, {5, "5.a"}};
        maps[1] = {{2, "2.b"}, {4, "4.b"}, {6, "6.b"}};
        maps[2] = {{1, "1.c"}, {2, "2.c"}, {7, "7.c"}};

        std::cout << "合并结果: " << std::endl;
        for (merge_iterator iter(maps); !iter.exhausted(); ++iter) {
            std::cout << "  " << iter->first << " -> " << iter->second
                      << " (来自容器 " << iter.current_container_index() << ")"
                      << std::endl;
        }
    }

}  // namespace iterator_merge_optimized

namespace insert_iterator_optimized {

    // 优化的后插入迭代器
    template <typename Container>
    class back_insert_iterator {
      public:
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using container_type = Container;

      private:
        Container* container_;

      public:
        explicit constexpr back_insert_iterator(Container& container) noexcept
            : container_(std::addressof(container)) {}

        template <typename T>
        constexpr back_insert_iterator& operator=(T&& value) {
            if constexpr (requires {
                              container_->push_back(std::forward<T>(value));
                          }) {
                container_->push_back(std::forward<T>(value));
            } else if constexpr (requires {
                                     container_->append(std::forward<T>(value));
                                 }) {
                container_->append(std::forward<T>(value));
            } else {
                static_assert(std::is_same_v<T, void>,
                              "Container must have push_back or append method");
            }
            return *this;
        }

        constexpr back_insert_iterator& operator*() noexcept { return *this; }
        constexpr back_insert_iterator& operator++() noexcept { return *this; }
        constexpr back_insert_iterator& operator++(int) noexcept {
            return *this;
        }
    };

    // 便利函数
    template <typename Container>
    constexpr back_insert_iterator<Container> back_inserter(
        Container& container) {
        return back_insert_iterator<Container>(container);
    }

    // 前插入迭代器
    template <typename Container>
    class front_insert_iterator {
      public:
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using container_type = Container;

      private:
        Container* container_;

      public:
        explicit constexpr front_insert_iterator(Container& container) noexcept
            : container_(std::addressof(container)) {}

        template <typename T>
        constexpr front_insert_iterator& operator=(T&& value) {
            container_->push_front(std::forward<T>(value));
            return *this;
        }

        constexpr front_insert_iterator& operator*() noexcept { return *this; }
        constexpr front_insert_iterator& operator++() noexcept { return *this; }
        constexpr front_insert_iterator& operator++(int) noexcept {
            return *this;
        }
    };

    template <typename Container>
    constexpr front_insert_iterator<Container> front_inserter(
        Container& container) {
        return front_insert_iterator<Container>(container);
    }

    // 通用插入迭代器
    template <typename Container>
    class insert_iterator {
      public:
        using iterator_category = std::output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using container_type = Container;

      private:
        Container* container_;
        typename Container::iterator iter_;

      public:
        constexpr insert_iterator(Container& container,
                                  typename Container::iterator iter)
            : container_(std::addressof(container)), iter_(iter) {}

        template <typename T>
        constexpr insert_iterator& operator=(T&& value) {
            iter_ = container_->insert(iter_, std::forward<T>(value));
            ++iter_;
            return *this;
        }

        constexpr insert_iterator& operator*() noexcept { return *this; }
        constexpr insert_iterator& operator++() noexcept { return *this; }
        constexpr insert_iterator& operator++(int) noexcept { return *this; }
    };

    template <typename Container>
    constexpr insert_iterator<Container> inserter(
        Container& container, typename Container::iterator iter) {
        return insert_iterator<Container>(container, iter);
    }

    // 改进的自定义容器
    template <typename T, typename Allocator = std::allocator<T>>
    class flexible_container {
      public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer =
            typename std::allocator_traits<Allocator>::const_pointer;
        using iterator = typename std::vector<T, Allocator>::iterator;
        using const_iterator =
            typename std::vector<T, Allocator>::const_iterator;

      private:
        std::vector<T, Allocator> data_;

      public:
        // 构造函数
        flexible_container() = default;
        explicit flexible_container(const Allocator& alloc) : data_(alloc) {}

        // 容量
        size_type size() const noexcept { return data_.size(); }
        bool empty() const noexcept { return data_.empty(); }

        // 访问
        reference operator[](size_type pos) { return data_[pos]; }
        const_reference operator[](size_type pos) const { return data_[pos]; }

        // 迭代器
        iterator begin() noexcept { return data_.begin(); }
        const_iterator begin() const noexcept { return data_.begin(); }
        const_iterator cbegin() const noexcept { return data_.cbegin(); }

        iterator end() noexcept { return data_.end(); }
        const_iterator end() const noexcept { return data_.end(); }
        const_iterator cend() const noexcept { return data_.cend(); }

        // 修改操作
        void push_back(const T& value) { data_.push_back(value); }
        void push_back(T&& value) { data_.push_back(std::move(value)); }

        template <typename... Args>
        void emplace_back(Args&&... args) {
            data_.emplace_back(std::forward<Args>(args)...);
        }

        void append(const T& value) { push_back(value); }
        void append(T&& value) { push_back(std::move(value)); }

        iterator insert(const_iterator pos, const T& value) {
            return data_.insert(pos, value);
        }

        iterator insert(const_iterator pos, T&& value) {
            return data_.insert(pos, std::move(value));
        }

        void clear() noexcept { data_.clear(); }

        // 输出
        void print(const std::string& prefix = "") const {
            std::cout << prefix;
            for (const auto& elem : data_) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }
    };

    void demonstrate() {
        std::cout << "\n=== 优化的插入迭代器 ===" << std::endl;

        flexible_container<int> container;
        std::vector<int> source = {1, 2, 3, 4, 5};

        // 后插入
        std::copy(source.begin(), source.end(),
                  insert_iterator_optimized::back_inserter(container));
        container.print("后插入结果: ");

        // 中间插入
        std::vector<int> middle = {10, 20};
        std::copy(middle.begin(), middle.end(),
                  insert_iterator_optimized::inserter(container,
                                                      container.begin() + 2));
        container.print("中间插入结果: ");

        // 使用算法
        flexible_container<int> squared;
        std::transform(source.begin(), source.end(),
                       insert_iterator_optimized::back_inserter(squared),
                       [](int x) { return x * x; });
        squared.print("平方结果: ");
    }

}  // namespace insert_iterator_optimized

namespace transform_optimized {

    // 优化的transform实现
    template <typename InputIt, typename OutputIt, typename UnaryOperation>
    constexpr OutputIt transform(InputIt first, InputIt last, OutputIt d_first,
                                 UnaryOperation unary_op) {
        for (; first != last; ++first, ++d_first) {
            *d_first = unary_op(*first);
        }
        return d_first;
    }

    template <typename InputIt1, typename InputIt2, typename OutputIt,
              typename BinaryOperation>
    constexpr OutputIt transform2(InputIt1 first1, InputIt1 last1,
                                  InputIt2 first2, OutputIt d_first,
                                  BinaryOperation binary_op) {
        for (; first1 != last1; ++first1, ++first2, ++d_first) {
            *d_first = binary_op(*first1, *first2);
        }
        return d_first;
    }

    // 并行版本的transform（概念性实现）
    template <typename ExecutionPolicy, typename InputIt, typename OutputIt,
              typename UnaryOperation>
    OutputIt transform(ExecutionPolicy&& policy, InputIt first, InputIt last,
                       OutputIt d_first, UnaryOperation unary_op) {
        // 这里可以根据执行策略选择不同的实现
        // 为简化，直接调用串行版本
        return transform(first, last, d_first, unary_op);
    }

    void demonstrate() {
        std::cout << "\n=== 优化的transform算法 ===" << std::endl;

        std::vector<int> source = {1, 2, 3, 4, 5};
        std::vector<int> dest1, dest2;

        // 单操作数transform
        transform_optimized::transform(
            source.begin(), source.end(),
            insert_iterator_optimized::back_inserter(dest1),
            [](int x) { return x * x + 1; });

        std::cout << "平方加1: ";
        for (int x : dest1) std::cout << x << " ";
        std::cout << std::endl;

        // 双操作数transform
        std::vector<int> multipliers = {2, 3, 4, 5, 6};
        transform_optimized::transform2(
            source.begin(), source.end(), multipliers.begin(),
            insert_iterator_optimized::back_inserter(dest2),
            std::multiplies<int>{});

        std::cout << "相乘结果: ";
        for (int x : dest2) std::cout << x << " ";
        std::cout << std::endl;
    }

}  // namespace transform_optimized

namespace iterator_utilities {

    // 计数迭代器
    template <typename T>
    class counting_iterator {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

      private:
        T value_;

      public:
        constexpr explicit counting_iterator(T value = T{}) : value_(value) {}

        constexpr reference operator*() const { return value_; }
        constexpr pointer operator->() const { return &value_; }

        constexpr counting_iterator& operator++() {
            ++value_;
            return *this;
        }
        constexpr counting_iterator operator++(int) {
            return counting_iterator{value_++};
        }

        constexpr counting_iterator& operator--() {
            --value_;
            return *this;
        }
        constexpr counting_iterator operator--(int) {
            return counting_iterator{value_--};
        }

        constexpr counting_iterator& operator+=(difference_type n) {
            value_ += n;
            return *this;
        }
        constexpr counting_iterator& operator-=(difference_type n) {
            value_ -= n;
            return *this;
        }

        constexpr counting_iterator operator+(difference_type n) const {
            return counting_iterator{value_ + n};
        }
        constexpr counting_iterator operator-(difference_type n) const {
            return counting_iterator{value_ - n};
        }

        constexpr difference_type operator-(
            const counting_iterator& other) const {
            return value_ - other.value_;
        }

        constexpr reference operator[](difference_type n) const {
            return value_ + n;
        }

        constexpr bool operator==(const counting_iterator& other) const {
            return value_ == other.value_;
        }
        constexpr bool operator!=(const counting_iterator& other) const {
            return value_ != other.value_;
        }
        constexpr bool operator<(const counting_iterator& other) const {
            return value_ < other.value_;
        }
        constexpr bool operator<=(const counting_iterator& other) const {
            return value_ <= other.value_;
        }
        constexpr bool operator>(const counting_iterator& other) const {
            return value_ > other.value_;
        }
        constexpr bool operator>=(const counting_iterator& other) const {
            return value_ >= other.value_;
        }
    };

    // 范围生成器
    template <typename T>
    class range {
      private:
        T begin_, end_, step_;

      public:
        constexpr range(T end) : begin_(T{}), end_(end), step_(T{1}) {}
        constexpr range(T begin, T end)
            : begin_(begin), end_(end), step_(T{1}) {}
        constexpr range(T begin, T end, T step)
            : begin_(begin), end_(end), step_(step) {}

        constexpr counting_iterator<T> begin() const {
            return counting_iterator<T>{begin_};
        }
        constexpr counting_iterator<T> end() const {
            return counting_iterator<T>{end_};
        }
    };

    void demonstrate() {
        std::cout << "\n=== 迭代器工具类 ===" << std::endl;

        std::cout << "计数迭代器 (0-9): ";
        for (auto i : range(10)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "范围 (5-15): ";
        for (auto i : range(5, 15)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "步长范围 (0-20, step=3): ";
        for (auto i : range(0, 20, 3)) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

}  // namespace iterator_utilities

// 演示所有功能
void demonstrate_all_iterator_features() {
    iterator_basic_transform::basic_usage();
    iterator_basic_transform::iterator_categories();

    fibonacci_iterator_optimized::demonstrate();
    iterator_merge_optimized::demonstrate();
    insert_iterator_optimized::demonstrate();
    transform_optimized::demonstrate();
    iterator_utilities::demonstrate();
}

int main() {
    // 运行新的演示
    demonstrate_all_iterator_features();
    std::cout << "\n=== 原始功能测试 ===" << std::endl;
    return 0;
}
