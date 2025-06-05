#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>

namespace IterOfFib {

    class fibit {
        size_t i{0};
        size_t a{0};
        size_t b{1};

      public:
        fibit() = default;
        explicit fibit(size_t i_) : i{i_} {}

        size_t operator*() const { return b; }
        fibit& operator++() {
            const size_t old_b{b};
            b += a;
            a = old_b;
            ++i;
            return *this;
        }
        bool operator!=(const fibit& o) const { return i != o.i; }
    };

    class fib_range {
        size_t end_n;

      public:
        fib_range(size_t end_n_) : end_n{end_n_} {}
        fibit begin() const { return fibit{}; }
        fibit end() const { return fibit{end_n}; }
    };

    void task() {
        for (size_t i : fib_range(10)) {
            std::cout << i << ", ";
        }
        std::cout << '\n';
    }
}  // namespace IterOfFib

namespace IteratorMerge {

    template <typename C>
    class MergedIterator {
      public:
        explicit MergedIterator(const std::vector<C>& input) {
            heap_.reserve(input.size());
            int i = 0;
            for (const C& c : input) {
                if (!c.empty()) {
                    heap_.push_back(
                        {.iter = c.cbegin(), .end = c.cend(), .idx = i++});
                }
            }
            std::make_heap(heap_.begin(), heap_.end());
        }

        bool done() const { return heap_.empty(); }

        const typename C::value_type& get() const {
            assert(!done());
            return *heap_.front().iter;
        }

        void next() {
            assert(!done());
            std::pop_heap(heap_.begin(), heap_.end());
            if (++heap_.back().iter == heap_.back().end) {
                heap_.pop_back();
            } else {
                std::push_heap(heap_.begin(), heap_.end());
            }
        }

      private:
        typedef typename C::const_iterator Iterator;

        struct Item {
            Iterator iter, end;
            int idx = 0;

            // TODO: generalize this using C::value_comp ?
            bool operator<(const Item& rhs) const {
                if (iter->first == rhs.iter->first) {
                    return idx > rhs.idx;
                }
                return iter->first > rhs.iter->first;
            }
        };

        std::vector<Item> heap_;
    };

    void task() {
        using Map = std::map<int, std::string>;
        std::vector<Map> maps(4);
        maps[0][1] = "1.a";
        maps[1][1] = "1.b";
        maps[2][1] = "1.c";
        maps[0][2] = "2.a";
        maps[1][3] = "3.b";
        maps[2][4] = "4.c";
        maps[0][9] = "9.a";
        maps[1][8] = "8.b";
        maps[2][7] = "7.c";

        for (MergedIterator<Map> s(maps); !s.done(); s.next()) {
            std::cout << s.get().first << " " << s.get().second << "\n";
        }
    }
}  // namespace IteratorMerge

namespace InsertIterator {

    /*
    // front_insert 插入迭代器
    ```c++
        front_insert_iterator& operator=(const typename Container::value_type& value) {
            container->push_front(value);
            return *this;
        }

        front_insert_iterator& operator=(typename Container::value_type&& value) {
            container->push_front(std::move(value));
            return *this;
        }
    ```

    // insert 插入迭代器
    ```c++
        insert_iterator& operator=(const typename Container::value_type& value) {
            iter = container->insert(iter, value);
            ++iter;
            return *this;
        }

        insert_iterator& operator=(typename Container::value_type&& value) {
            iter = container->insert(iter, std::move(value));
            ++iter;
            return *this;
        }
    ```
    */
    template <typename Container>
    class back_insert_iterator {
      protected:
        Container* container;

      public:
        using iterator_category = std::output_iterator_tag;
        using container_value_type = typename std::remove_reference<
            decltype(*std::declval<Container>().begin())>::type;

        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

        explicit back_insert_iterator(Container& c) : container(&c) {}

        back_insert_iterator& operator=(const container_value_type& value) {
            container->append(value);
            return *this;
        }

        back_insert_iterator& operator=(container_value_type&& value) {
            container->append(std::move(value));
            return *this;
        }

        back_insert_iterator& operator*() { return *this; }
        back_insert_iterator& operator++() { return *this; }
        back_insert_iterator& operator++(int) { return *this; }
    };

    template <typename Container>
    back_insert_iterator<Container> back_inserter(Container& c) {
        return back_insert_iterator<Container>(c);
    }

    // 自定义容器类
    template <typename T>
    class CustomContainer {
      public:
        using value_type = T;

        void append(const T& value) { data.push_back(value); }

        void print() const {
            for (const auto& elem : data) {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }

      public:
        auto begin() { return data.begin(); }
        auto end() { return data.end(); }

      private:
        std::vector<T> data;
    };

    void task() {
        CustomContainer<int> container;
        for (const auto& elem : container) {
            std::cout << elem << " ";
        }

        std::cout << std::endl << " --- " << std::endl;

        std::vector<int> values = {1, 2, 3, 4, 5};
        std::copy(values.begin(), values.end(), back_inserter(container));

        for (const auto& elem : container) {
            std::cout << elem << " ";
        }
    }
}  // namespace InsertIterator

namespace ImplOfTransform {
    // 单一输入范围的 `std::transform`
    template <typename InputIt, typename OutputIt, typename UnaryOperation>
    OutputIt transform(InputIt first, InputIt last, OutputIt d_first,
                       UnaryOperation unary_op) {
        while (first != last) {
            *d_first++ = unary_op(*first++);
        }
        return d_first;
    }

    // 双输入范围的 `std::transform`
    template <typename InputIt1, typename InputIt2, typename OutputIt,
              typename BinaryOperation>
    OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                       OutputIt d_first, BinaryOperation binary_op) {
        while (first1 != last1) {
            *d_first++ = binary_op(*first1++, *first2++);
        }
        return d_first;
    }

}  // namespace ImplOfTransform

namespace TransformWithInsertInterator {
    void task() {
        std::vector<int> vec = {1, 2, 3};
        std::vector<int> vec2;
        std::transform(vec.begin(), vec.end(), std::back_inserter(vec2),
                       [](int n) { return n * 2; });
        std::cout << "After back_inserter: ";

        for (int n : vec2) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }

}  // namespace TransformWithInsertInterator

namespace TransformWithoutInsertInterator {
    void task() {
        std::vector<int> vec = {1, 2, 3};
        std::vector<int> vec2(vec.size());
        std::transform(vec.begin(), vec.end(), vec2.begin(),
                       [](int n) { return n * 2; });

        std::cout << "After transform: ";
        for (int n : vec2) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }

}  // namespace TransformWithoutInsertInterator

int main() {
    IterOfFib::task();
    IteratorMerge::task();
    InsertIterator::task();
    TransformWithInsertInterator::task();
    TransformWithoutInsertInterator::task();
    return 0;
}
