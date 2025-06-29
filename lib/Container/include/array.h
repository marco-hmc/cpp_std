#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>

#include "utils.h"

namespace MyStd::ArrayImpl1 {

    template <class Tp, size_t N>
    struct Array {
        using value_type = Tp;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = Tp *;
        using reference = Tp &;
        using iterator = Tp *;

        Tp M_elements[N];

        Tp &operator[](size_t i) { return M_elements[i]; }
        Tp &at(size_t i) {
            if (i >= N) throw std::out_of_range("array::at");
            return M_elements[i];
        }
        void fill(Tp &val) {
            for (size_t i = 0; i < N; i++) {
                M_elements[i] = val;
            }
        }
        void swap(Array &that) {
            for (size_t i = 0; i < N; i++) {
                std::swap(M_elements[i], that.M_elements[i]);
            }
        }
        Tp &front() { return M_elements[0]; }
        Tp &back() { return M_elements[N - 1]; }
        bool empty() { return false; }
        size_t size() { return N; }
        size_t max_size() { return N; }
        Tp *data() { return M_elements; }
        Tp *begin() { return M_elements; }
        Tp *end() { return M_elements + N; }
    };

    template <class Tp>
    struct Array<Tp, 0> {
        using value_type = Tp;
        using pointer = Tp *;
        using reference = Tp &;
        using iterator = Tp *;

        Tp &operator[](size_t) { UNREACHABLE(); }
        Tp &at(size_t) { throw std::out_of_range("array::at"); }
        void fill(Tp &) {}
        void swap(Array &) {}
        Tp &front() { UNREACHABLE(); }
        Tp &back() { UNREACHABLE(); }
        bool empty() { return true; }
        size_t size() { return 0; }
        size_t max_size() { return 0; }
        Tp *data() { return nullptr; }
        Tp *begin() { return nullptr; }
        Tp *end() { return nullptr; }
    };

    template <class Tp, class... Ts>
    Array(Tp, Ts...) -> Array<Tp, 1 + sizeof...(Ts)>;

}  // namespace MyStd::ArrayImpl1
