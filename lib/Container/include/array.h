#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>

#include "utils.h"

namespace MyStd {

    namespace ArrayImpl1 {

        template <class Tp, size_t N>
        struct Array {
            using value_type = Tp;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using pointer = Tp *;
            using const_pointer = Tp const *;
            using reference = Tp &;
            using const_reference = Tp const &;
            using iterator = Tp *;
            using const_iterator = Tp const *;
            using reverse_iterator = std::reverse_iterator<Tp *>;
            using const_reverse_iterator = std::reverse_iterator<Tp const *>;

            Tp M_elements[N];

            Tp &operator[](size_t _i) noexcept { return M_elements[_i]; }

            Tp const &operator[](size_t _i) const noexcept {
                return M_elements[_i];
            }

            Tp &at(size_t _i) {
                if (_i >= N) [[__unlikely__]]
                    throw std::out_of_range("array::at");
                return M_elements[_i];
            }

            Tp const &at(size_t _i) const {
                if (_i >= N) [[__unlikely__]]
                    throw std::out_of_range("array::at");
                return M_elements[_i];
            }

            void fill(Tp const &_val) noexcept(
                std::is_nothrow_copy_assignable_v<Tp>) {
                for (size_t _i = 0; _i < N; _i++) {
                    M_elements[_i] = _val;
                }
            }

            void swap(Array &_that) noexcept(std::is_nothrow_swappable_v<Tp>) {
                for (size_t _i = 0; _i < N; _i++) {
                    std::swap(M_elements[_i], _that.M_elements[_i]);
                }
            }

            Tp &front() noexcept { return M_elements[0]; }

            Tp const &front() const noexcept { return M_elements[0]; }

            Tp &back() noexcept { return M_elements[N - 1]; }

            Tp const &back() const noexcept { return M_elements[N - 1]; }

            static constexpr bool empty() noexcept { return false; }

            static constexpr size_t size() noexcept { return N; }

            static constexpr size_t max_size() noexcept { return N; }

            Tp const *cdata() const noexcept { return M_elements; }

            Tp const *data() const noexcept { return M_elements; }

            Tp *data() noexcept { return M_elements; }

            Tp const *cbegin() const noexcept { return M_elements; }

            Tp const *cend() const noexcept { return M_elements + N; }

            Tp const *begin() const noexcept { return M_elements; }

            Tp const *end() const noexcept { return M_elements + N; }

            Tp *begin() noexcept { return M_elements; }

            Tp *end() noexcept { return M_elements + N; }

            const_reverse_iterator crbegin() const noexcept {
                return const_reverse_iterator(end());
            }

            const_reverse_iterator crend() const noexcept {
                return const_reverse_iterator(begin());
            }

            const_reverse_iterator rbegin() const noexcept {
                return const_reverse_iterator(end());
            }

            const_reverse_iterator rend() const noexcept {
                return const_reverse_iterator(begin());
            }

            reverse_iterator rbegin() noexcept {
                return reverse_iterator(end());
            }

            reverse_iterator rend() noexcept {
                return reverse_iterator(begin());
            }
        };

        template <class Tp>
        struct Array<Tp, 0> {
            using value_type = Tp;
            using pointer = Tp *;
            using const_pointer = Tp const *;
            using reference = Tp &;
            using const_reference = Tp const &;
            using iterator = Tp *;
            using const_iterator = Tp const *;
            using reverse_iterator = Tp *;
            using const_reverse_iterator = Tp const *;

            Tp &operator[](size_t /*__i*/) noexcept { UNREACHABLE(); }

            Tp const &operator[](size_t /*__i*/) const noexcept {
                UNREACHABLE();
            }

            Tp &at(size_t /*__i*/) { throw std::out_of_range("array::at"); }

            Tp const &at(size_t /*__i*/) const {
                throw std::out_of_range("array::at");
            }

            void fill(Tp const & /*unused*/) noexcept {}

            void swap(Array & /*unused*/) noexcept {}

            Tp &front() noexcept { UNREACHABLE(); }

            Tp const &front() const noexcept { UNREACHABLE(); }

            Tp &back() noexcept { UNREACHABLE(); }

            Tp const &back() const noexcept { UNREACHABLE(); }

            static constexpr bool empty() noexcept { return true; }

            static constexpr size_t size() noexcept { return 0; }

            static constexpr size_t max_size() noexcept { return 0; }

            Tp const *cdata() const noexcept { return nullptr; }

            Tp const *data() const noexcept { return nullptr; }

            Tp *data() noexcept { return nullptr; }

            Tp const *cbegin() const noexcept { return nullptr; }

            Tp const *cend() const noexcept { return nullptr; }

            Tp const *begin() const noexcept { return nullptr; }

            Tp const *end() const noexcept { return nullptr; }

            Tp *begin() noexcept { return nullptr; }

            Tp *end() noexcept { return nullptr; }

            Tp const *crbegin() const noexcept { return nullptr; }

            Tp const *crend() const noexcept { return nullptr; }

            Tp const *rbegin() const noexcept { return nullptr; }

            Tp const *rend() const noexcept { return nullptr; }

            Tp *rbegin() noexcept { return nullptr; }

            Tp *rend() noexcept { return nullptr; }
        };

        template <class Tp, class... Ts>
        Array(Tp, Ts...)
            -> Array<std::enable_if_t<(std::is_same_v<Tp, Ts> && ...), Tp>,
                     1 + sizeof...(Ts)>;

    }  // namespace ArrayImpl1

}  // namespace MyStd