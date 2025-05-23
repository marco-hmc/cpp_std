#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

#include "utils.h"

namespace MyStd {

    namespace ArrayImpl1 {

        template <class _Tp, std::size_t _N>
        struct Array {
            using value_type = _Tp;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using pointer = _Tp *;
            using const_pointer = _Tp const *;
            using reference = _Tp &;
            using const_reference = _Tp const &;
            using iterator = _Tp *;
            using const_iterator = _Tp const *;
            using reverse_iterator = std::reverse_iterator<_Tp *>;
            using const_reverse_iterator = std::reverse_iterator<_Tp const *>;

            _Tp _M_elements[_N];

            _Tp &operator[](size_t __i) noexcept { return _M_elements[__i]; }

            _Tp const &operator[](size_t __i) const noexcept {
                return _M_elements[__i];
            }

            _Tp &at(size_t __i) {
                if (__i >= _N) [[__unlikely__]]
                    throw std::out_of_range("array::at");
                return _M_elements[__i];
            }

            _Tp const &at(size_t __i) const {
                if (__i >= _N) [[__unlikely__]]
                    throw std::out_of_range("array::at");
                return _M_elements[__i];
            }

            void fill(_Tp const &__val) noexcept(
                std::is_nothrow_copy_assignable_v<_Tp>) {
                for (size_t __i = 0; __i < _N; __i++) {
                    _M_elements[__i] = __val;
                }
            }

            void swap(Array &__that) noexcept(
                std::is_nothrow_swappable_v<_Tp>) {
                for (size_t __i = 0; __i < _N; __i++) {
                    std::swap(_M_elements[__i], __that._M_elements[__i]);
                }
            }

            _Tp &front() noexcept { return _M_elements[0]; }

            _Tp const &front() const noexcept { return _M_elements[0]; }

            _Tp &back() noexcept { return _M_elements[_N - 1]; }

            _Tp const &back() const noexcept { return _M_elements[_N - 1]; }

            static constexpr bool empty() noexcept { return false; }

            static constexpr std::size_t size() noexcept { return _N; }

            static constexpr std::size_t max_size() noexcept { return _N; }

            _Tp const *cdata() const noexcept { return _M_elements; }

            _Tp const *data() const noexcept { return _M_elements; }

            _Tp *data() noexcept { return _M_elements; }

            _Tp const *cbegin() const noexcept { return _M_elements; }

            _Tp const *cend() const noexcept { return _M_elements + _N; }

            _Tp const *begin() const noexcept { return _M_elements; }

            _Tp const *end() const noexcept { return _M_elements + _N; }

            _Tp *begin() noexcept { return _M_elements; }

            _Tp *end() noexcept { return _M_elements + _N; }

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

            _LIBPENGCXX_DEFINE_COMPARISON(Array);
        };

        template <class _Tp>
        struct Array<_Tp, 0> {
            using value_type = _Tp;
            using pointer = _Tp *;
            using const_pointer = _Tp const *;
            using reference = _Tp &;
            using const_reference = _Tp const &;
            using iterator = _Tp *;
            using const_iterator = _Tp const *;
            using reverse_iterator = _Tp *;
            using const_reverse_iterator = _Tp const *;

            _Tp &operator[](size_t __i) noexcept { UNREACHABLE; }

            _Tp const &operator[](size_t __i) const noexcept { UNREACHABLE; }

            _Tp &at(size_t __i) { throw std::out_of_range("array::at"); }

            _Tp const &at(size_t __i) const {
                throw std::out_of_range("array::at");
            }

            void fill(_Tp const &) noexcept {}

            void swap(Array &) noexcept {}

            _Tp &front() noexcept { UNREACHABLE; }

            _Tp const &front() const noexcept { UNREACHABLE; }

            _Tp &back() noexcept { UNREACHABLE; }

            _Tp const &back() const noexcept { UNREACHABLE; }

            static constexpr bool empty() noexcept { return true; }

            static constexpr size_t size() noexcept { return 0; }

            static constexpr size_t max_size() noexcept { return 0; }

            _Tp const *cdata() const noexcept { return nullptr; }

            _Tp const *data() const noexcept { return nullptr; }

            _Tp *data() noexcept { return nullptr; }

            _Tp const *cbegin() const noexcept { return nullptr; }

            _Tp const *cend() const noexcept { return nullptr; }

            _Tp const *begin() const noexcept { return nullptr; }

            _Tp const *end() const noexcept { return nullptr; }

            _Tp *begin() noexcept { return nullptr; }

            _Tp *end() noexcept { return nullptr; }

            _Tp const *crbegin() const noexcept { return nullptr; }

            _Tp const *crend() const noexcept { return nullptr; }

            _Tp const *rbegin() const noexcept { return nullptr; }

            _Tp const *rend() const noexcept { return nullptr; }

            _Tp *rbegin() noexcept { return nullptr; }

            _Tp *rend() noexcept { return nullptr; }

            _LIBPENGCXX_DEFINE_COMPARISON(Array);
        };

        template <class _Tp, class... _Ts>
        Array(_Tp, _Ts...)
            -> Array<std::enable_if_t<(std::is_same_v<_Tp, _Ts> && ...), _Tp>,
                     1 + sizeof...(_Ts)>;

    }  // namespace ArrayImpl1

}  // namespace MyStd
