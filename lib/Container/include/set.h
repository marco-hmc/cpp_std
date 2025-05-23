#pragma once
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

#include "RbTree.h"

namespace MyStd {
    namespace SetImpl1 {
        using namespace RbTreeImpl1;

        template <class Tp, class Compare = std::less<Tp>,
                  class Alloc = std::allocator<Tp>>
        struct Set : RbTreeImpl<Tp const, Compare, Alloc> {
            using typename RbTreeImpl<Tp const, Compare, Alloc>::const_iterator;
            using typename RbTreeImpl<Tp const, Compare, Alloc>::node_type;
            using iterator = const_iterator;
            using value_type = Tp;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            Set() = default;

            explicit Set(Compare _comp)
                : RbTreeImpl<Tp const, Compare, Alloc>(_comp) {}

            Set(Set &&) = default;
            Set &operator=(Set &&) = default;

            Set(Set const &other) : RbTreeImpl<Tp const, Compare, Alloc>() {
                this->_M_single_insert(other.begin(), other.end());
            }

            Set &operator=(Set const &other) {
                if (&other != this) {
                    this->assign(other.begin(), other.end());
                }
                return *this;
            }

            Set &operator=(std::initializer_list<Tp> _ilist) {
                this->assign(_ilist);
                return *this;
            }

            void assign(std::initializer_list<Tp> _ilist) {
                this->clear();
                this->_M_single_insert(_ilist.begin(), _ilist.end());
            }

            Compare value_comp() const noexcept { return this->_M_comp; }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            const_iterator find(Tp &&_value) const noexcept {
                return this->_M_find(_value);
            }

            const_iterator find(Tp const &_value) const noexcept {
                return this->_M_find(_value);
            }

            std::pair<iterator, bool> insert(Tp &&_value) {
                return this->_M_single_emplace(std::move(_value));
            }

            std::pair<iterator, bool> insert(Tp const &_value) {
                return this->_M_single_emplace(_value);
            }

            template <class... Ts>
            std::pair<iterator, bool> emplace(Ts &&..._value) {
                return this->_M_single_emplace(std::forward<Ts>(_value)...);
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void insert(_InputIt _first, _InputIt _last) {
                return this->_M_single_insert(_first, _last);
            }

            using RbTreeImpl<Tp const, Compare, Alloc>::assign;

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void assign(_InputIt _first, _InputIt _last) {
                this->clear();
                return this->_M_single_insert(_first, _last);
            }

            using RbTreeImpl<Tp const, Compare, Alloc>::erase;

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            std::size_t erase(Tv &&_value) {
                return this->_M_single_erase(_value);
            }

            std::size_t erase(Tp const &_value) {
                return this->_M_single_erase(_value);
            }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            std::size_t count(Tv &&_value) const noexcept {
                return this->_M_contains(_value) ? 1 : 0;
            }

            std::size_t count(Tp const &_value) const noexcept {
                return this->_M_contains(_value) ? 1 : 0;
            }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            bool contains(Tv &&_value) const noexcept {
                return this->_M_contains(_value);
            }

            bool contains(Tp const &_value) const noexcept {
                return this->_M_contains(_value);
            }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            node_type extract(Tv &&_value) {
                iterator __it = this->_M_find(_value);
                return __it != this->end() ? this->extract(__it) : node_type();
            }

            node_type extract(Tp const &_value) {
                iterator __it = this->_M_find(_value);
                return __it != this->end() ? this->extract(__it) : node_type();
            }
        };

        template <class Tp, class Compare = std::less<Tp>,
                  class Alloc = std::allocator<Tp>>
        struct MultiSet : RbTreeImpl<Tp const, Compare, Alloc> {
            using typename RbTreeImpl<Tp const, Compare, Alloc>::const_iterator;
            using typename RbTreeImpl<Tp const, Compare, Alloc>::node_type;
            using iterator = const_iterator;
            using value_type = Tp;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            MultiSet() = default;

            explicit MultiSet(Compare _comp)
                : RbTreeImpl<Tp const, Compare, Alloc>(_comp) {}

            MultiSet(MultiSet &&) = default;
            MultiSet &operator=(MultiSet &&) = default;

            MultiSet(MultiSet const &other)
                : RbTreeImpl<Tp const, Compare, Alloc>() {
                this->_M_multi_insert(other.begin(), other.end());
            }

            MultiSet &operator=(MultiSet const &other) {
                if (&other != this) {
                    this->assign(other.begin(), other.end());
                }
                return *this;
            }

            MultiSet &operator=(std::initializer_list<Tp> _ilist) {
                this->assign(_ilist);
                return *this;
            }

            void assign(std::initializer_list<Tp> _ilist) {
                this->clear();
                this->_M_multi_insert(_ilist.begin(), _ilist.end());
            }

            Compare value_comp() const noexcept { return this->_M_comp; }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            const_iterator find(Tp &&_value) const noexcept {
                return this->_M_find(_value);
            }

            const_iterator find(Tp const &_value) const noexcept {
                return this->_M_find(_value);
            }

            iterator insert(Tp &&_value) {
                return this->_M_multi_emplace(std::move(_value));
            }

            iterator insert(Tp const &_value) {
                return this->_M_multi_emplace(_value);
            }

            template <class... Ts>
            iterator emplace(Ts &&..._value) {
                return this->_M_multi_emplace(std::forward<Ts>(_value)...);
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void insert(_InputIt _first, _InputIt _last) {
                return this->_M_multi_insert(_first, _last);
            }

            using RbTreeImpl<Tp const, Compare, Alloc>::assign;

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void assign(_InputIt _first, _InputIt _last) {
                this->clear();
                return this->_M_multi_insert(_first, _last);
            }

            using RbTreeImpl<Tp const, Compare, Alloc>::erase;

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            std::size_t erase(Tv &&_value) {
                return this->_M_multi_erase(_value);
            }

            std::size_t erase(Tp const &_value) {
                return this->_M_multi_erase(_value);
            }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            std::size_t count(Tv &&_value) const noexcept {
                return this->_M_multi_count(_value);
            }

            std::size_t count(Tp const &_value) const noexcept {
                return this->_M_multi_count(_value);
            }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            bool contains(Tv &&_value) const noexcept {
                return this->_M_contains(_value);
            }

            bool contains(Tp const &_value) const noexcept {
                return this->_M_contains(_value);
            }

            template <class Tv,
                      _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(Compare, Tv, Tp)>
            node_type extract(Tv &&_value) {
                iterator __it = this->_M_find(_value);
                return __it != this->end() ? this->extract(__it) : node_type();
            }

            node_type extract(Tp const &_value) {
                iterator __it = this->_M_find(_value);
                return __it != this->end() ? this->extract(__it) : node_type();
            }
        };
    }  // namespace SetImpl1
}  // namespace MyStd