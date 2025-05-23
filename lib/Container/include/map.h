#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include "RbTree.h"

namespace MyStd {
    namespace MapImpl1 {
        using namespace RbTreeImpl1;

        template <class Compare, class Value, class = void>
        struct RbTreeValueCompare {
          protected:
            [[no_unique_address]] Compare M_comp;

          public:
            RbTreeValueCompare(Compare _comp = Compare()) noexcept
                : M_comp(_comp) {}

            bool operator()(typename Value::first_type const &_lhs,
                            Value const &_rhs) const noexcept {
                return this->M_comp(_lhs, _rhs.first);
            }

            bool operator()(
                Value const &_lhs,
                typename Value::first_type const &_rhs) const noexcept {
                return this->M_comp(_lhs.first, _rhs);
            }

            bool operator()(Value const &_lhs,
                            Value const &_rhs) const noexcept {
                return this->M_comp(_lhs.first, _rhs.first);
            }

            struct RbTreeIsMap;
        };

        template <class Compare, class Value>
        struct RbTreeValueCompare<
            Compare, Value,
            decltype((void)static_cast<typename Compare::is_transparent *>(
                nullptr))> {
            [[no_unique_address]] Compare M_comp;

            RbTreeValueCompare(Compare _comp = Compare()) noexcept
                : M_comp(_comp) {}

            template <class Lhs>
            bool operator()(Lhs &&_lhs, Value const &_rhs) const noexcept {
                return this->M_comp(_lhs, _rhs.first);
            }

            template <class Rhs>
            bool operator()(Value const &_lhs, Rhs &&_rhs) const noexcept {
                return this->M_comp(_lhs.first, _rhs);
            }

            bool operator()(Value const &_lhs,
                            Value const &_rhs) const noexcept {
                return this->M_comp(_lhs.first, _rhs.first);
            }

            using is_transparent = typename Compare::is_transparent;
        };

        template <class Key, class Mapped, class Compare = std::less<Key>,
                  class Alloc = std::allocator<std::pair<Key const, Mapped>>>
        struct Map
            : RbTreeImpl<
                  std::pair<Key const, Mapped>,
                  RbTreeValueCompare<Compare, std::pair<Key const, Mapped>>,
                  Alloc> {
            using key_type = Key;
            using mapped_type = Mapped;
            using value_type = std::pair<Key const, Mapped>;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

          private:
            using ValueComp = RbTreeValueCompare<Compare, value_type>;

          public:
            using typename RbTreeImpl<value_type, ValueComp, Alloc>::iterator;
            using typename RbTreeImpl<value_type, ValueComp,
                                      Alloc>::const_iterator;
            using typename RbTreeImpl<value_type, ValueComp, Alloc>::node_type;

            Map() = default;

            explicit Map(Compare _comp)
                : RbTreeImpl<value_type, ValueComp, Alloc>(_comp) {}

            Map(std::initializer_list<value_type> _ilist) {
                _M_single_insert(_ilist.begin(), _ilist.end());
            }

            explicit Map(std::initializer_list<value_type> _ilist,
                         Compare _comp)
                : RbTreeImpl<value_type, ValueComp, Alloc>(_comp) {
                _M_single_insert(_ilist.begin(), _ilist.end());
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            explicit Map(_InputIt _first, _InputIt _last) {
                _M_single_insert(_first, _last);
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            explicit Map(_InputIt _first, _InputIt _last, Compare _comp)
                : RbTreeImpl<value_type, ValueComp, Alloc>(_comp) {
                _M_single_insert(_first, _last);
            }

            Map(Map &&) = default;
            Map &operator=(Map &&) = default;

            Map(Map const &other) : RbTreeImpl<value_type, ValueComp, Alloc>() {
                this->_M_single_insert(other.begin(), other.end());
            }

            Map &operator=(Map const &other) {
                if (&other != this) {
                    this->assign(other.begin(), other.end());
                }
                return *this;
            }

            Map &operator=(std::initializer_list<value_type> _ilist) {
                this->assign(_ilist);
                return *this;
            }

            void assign(std::initializer_list<value_type> _ilist) {
                this->clear();
                this->_M_single_insert(_ilist.begin(), _ilist.end());
            }

            Compare key_comp() const noexcept { return this->M_comp->M_comp; }

            ValueComp value_comp() const noexcept { return this->M_comp; }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            iterator find(Kv &&_key) noexcept {
                return this->_M_find(_key);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            const_iterator find(Kv &&_key) const noexcept {
                return this->_M_find(_key);
            }

            iterator find(Key const &_key) noexcept {
                return this->_M_find(_key);
            }

            const_iterator find(Key const &_key) const noexcept {
                return this->_M_find(_key);
            }

            std::pair<iterator, bool> insert(value_type &&_value) {
                return this->_M_single_emplace(std::move(_value));
            }

            std::pair<iterator, bool> insert(value_type const &_value) {
                return this->_M_single_emplace(_value);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            Mapped const &at(Kv const &_key) const {
                const_iterator _it = this->_M_find(_key);
                if (_it == this->end()) [[unlikely]] {
                    throw std::out_of_range("map::at");
                }
                return _it->second;
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            Mapped &at(Kv const &_key) {
                iterator _it = this->_M_find(_key);
                if (_it == this->end()) [[unlikely]] {
                    throw std::out_of_range("map::at");
                }
                return _it->second;
            }

            Mapped const &at(Key const &_key) const {
                const_iterator _it = this->_M_find(_key);
                if (_it == this->end()) [[unlikely]] {
                    throw std::out_of_range("map::at");
                }
                return _it->second;
            }

            Mapped &at(Key const &_key) {
                iterator _it = this->_M_find(_key);
                if (_it == this->end()) [[unlikely]] {
                    throw std::out_of_range("map::at");
                }
                return _it->second;
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            Mapped &operator[](Kv const &_key) {
                iterator _it = this->_M_find(_key);
                if (_it == this->end()) {
                    _it = this->_M_single_emplace(std::piecewise_construct,
                                                  std::forward_as_tuple(_key),
                                                  std::forward_as_tuple())
                              .first;
                }
                return _it->second;
            }

            Mapped &operator[](Key const &_key) {
                iterator _it = this->_M_find(_key);
                if (_it == this->end()) {
                    _it = this->_M_single_emplace(std::piecewise_construct,
                                                  std::forward_as_tuple(_key),
                                                  std::forward_as_tuple())
                              .first;
                }
                return _it->second;
            }

            template <class Mp, class = std::enable_if_t<
                                    std::is_convertible_v<Mp, Mapped>>>
            std::pair<iterator, bool> insert_or_assign(Key const &_key,
                                                       Mp &&_mapped) {
                std::pair<iterator, bool> _result = this->_M_single_emplace(
                    std::piecewise_construct, std::forward_as_tuple(_key),
                    std::forward_as_tuple(std::forward<Mp>(_mapped)));
                if (!_result.second) {
                    _result.first->second = std::forward<Mp>(_mapped);
                }
                return _result;
            }

            template <class Mp, class = std::enable_if_t<
                                    std::is_convertible_v<Mp, Mapped>>>
            std::pair<iterator, bool> insert_or_assign(Key &&_key,
                                                       Mp &&_mapped) {
                std::pair<iterator, bool> _result = this->_M_single_emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(std::move(_key)),
                    std::forward_as_tuple(std::forward<Mp>(_mapped)));
                if (!_result.second) {
                    _result.first->second = std::forward<Mp>(_mapped);
                }
                return _result;
            }

            template <class... Vs>
            std::pair<iterator, bool> emplace(Vs &&..._value) {
                return this->_M_single_emplace(std::forward<Vs>(_value)...);
            }

            template <class... Ms>
            std::pair<iterator, bool> try_emplace(Key &&_key, Ms &&..._mapped) {
                return this->_M_single_emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(std::move(_key)),
                    std::forward_as_tuple(std::forward<Ms>(_mapped)...));
            }

            template <class... Ms>
            std::pair<iterator, bool> try_emplace(Key const &_key,
                                                  Ms &&..._mapped) {
                return this->_M_single_emplace(
                    std::piecewise_construct, std::forward_as_tuple(_key),
                    std::forward_as_tuple(std::forward<Ms>(_mapped)...));
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            iterator insert(_InputIt _first, _InputIt _last) {
                return this->_M_single_insert(_first, _last);
            }

            using RbTreeImpl<value_type, ValueComp, Alloc>::assign;

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            iterator assign(_InputIt _first, _InputIt _last) {
                this->clear();
                return this->_M_single_insert(_first, _last);
            }

            using RbTreeImpl<value_type, ValueComp, Alloc>::erase;

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            size_t erase(Kv &&_key) {
                return this->_M_single_erase(_key);
            }

            size_t erase(Key const &_key) {
                return this->_M_single_erase(_key);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            size_t count(Kv &&_value) const noexcept {
                return this->_M_contains(_value) ? 1 : 0;
            }

            size_t count(Key const &_value) const noexcept {
                return this->_M_contains(_value) ? 1 : 0;
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            bool contains(Kv &&_value) const noexcept {
                return this->_M_contains(_value);
            }

            bool contains(Key const &_value) const noexcept {
                return this->_M_contains(_value);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            node_type extract(Kv &&_key) {
                iterator _it = this->_M_find(_key);
                return _it != this->end() ? this->extract(_it) : node_type();
            }

            node_type extract(Key const &_key) {
                iterator _it = this->_M_find(_key);
                return _it != this->end() ? this->extract(_it) : node_type();
            }
        };

        template <class Key, class Mapped, class Compare = std::less<Key>,
                  class _Alloc = std::allocator<std::pair<Key const, Mapped>>>
        struct MultiMap
            : RbTreeImpl<
                  std::pair<Key const, Mapped>,
                  RbTreeValueCompare<Compare, std::pair<Key const, Mapped>>,
                  _Alloc> {
            using key_type = Key;
            using mapped_type = Mapped;
            using value_type = std::pair<Key const, Mapped>;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

          private:
            using ValueComp = RbTreeValueCompare<Compare, value_type>;

          public:
            using typename RbTreeImpl<value_type, ValueComp, _Alloc>::iterator;
            using typename RbTreeImpl<value_type, ValueComp,
                                      _Alloc>::const_iterator;
            using typename RbTreeImpl<value_type, ValueComp, _Alloc>::node_type;

            MultiMap() = default;

            explicit MultiMap(Compare _comp)
                : RbTreeImpl<value_type, ValueComp, _Alloc>(_comp) {}

            MultiMap(std::initializer_list<value_type> _ilist) {
                _M_multi_insert(_ilist.begin(), _ilist.end());
            }

            explicit MultiMap(std::initializer_list<value_type> _ilist,
                              Compare _comp)
                : RbTreeImpl<value_type, ValueComp, _Alloc>(_comp) {
                _M_multi_insert(_ilist.begin(), _ilist.end());
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            explicit MultiMap(_InputIt _first, _InputIt _last) {
                _M_multi_insert(_first, _last);
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            explicit MultiMap(_InputIt _first, _InputIt _last, Compare _comp)
                : RbTreeImpl<value_type, ValueComp, _Alloc>(_comp) {
                _M_multi_insert(_first, _last);
            }

            MultiMap(MultiMap &&) = default;
            MultiMap &operator=(MultiMap &&) = default;

            MultiMap(MultiMap const &other)
                : RbTreeImpl<value_type, ValueComp, _Alloc>() {
                this->_M_multi_insert(other.begin(), other.end());
            }

            MultiMap &operator=(MultiMap const &other) {
                if (&other != this) {
                    this->assign(other.begin(), other.end());
                }
                return *this;
            }

            MultiMap &operator=(std::initializer_list<value_type> _ilist) {
                this->assign(_ilist);
                return *this;
            }

            void assign(std::initializer_list<value_type> _ilist) {
                this->clear();
                this->_M_multi_insert(_ilist.begin(), _ilist.end());
            }

            Compare key_comp() const noexcept { return this->M_comp->M_comp; }

            ValueComp value_comp() const noexcept { return this->M_comp; }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            iterator find(Kv &&_key) noexcept {
                return this->_M_find(_key);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            const_iterator find(Kv &&_key) const noexcept {
                return this->_M_find(_key);
            }

            iterator find(Key const &_key) noexcept {
                return this->_M_find(_key);
            }

            const_iterator find(Key const &_key) const noexcept {
                return this->_M_find(_key);
            }

            std::pair<iterator, bool> insert(value_type &&_value) {
                return this->_M_single_emplace(std::move(_value));
            }

            std::pair<iterator, bool> insert(value_type const &_value) {
                return this->_M_single_emplace(_value);
            }

            template <class... Ts>
            std::pair<iterator, bool> emplace(Ts &&..._value) {
                return this->_M_single_emplace(std::forward<Ts>(_value)...);
            }

            template <class... Ts>
            std::pair<iterator, bool> try_emplace(Key &&_key, Ts &&..._value) {
                return this->_M_single_emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(std::move(_key)),
                    std::forward_as_tuple(std::forward<Ts>(_value)...));
            }

            template <class... Ts>
            std::pair<iterator, bool> try_emplace(Key const &_key,
                                                  Ts &&..._value) {
                return this->_M_single_emplace(
                    std::piecewise_construct, std::forward_as_tuple(_key),
                    std::forward_as_tuple(std::forward<Ts>(_value)...));
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            iterator insert(_InputIt _first, _InputIt _last) {
                return this->_M_single_insert(_first, _last);
            }

            using RbTreeImpl<value_type, ValueComp, _Alloc>::assign;

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            iterator assign(_InputIt _first, _InputIt _last) {
                this->clear();
                return this->_M_single_insert(_first, _last);
            }

            using RbTreeImpl<value_type, ValueComp, _Alloc>::erase;

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            size_t erase(Kv &&_key) {
                return this->_M_single_erase(_key);
            }

            size_t erase(Key const &_key) {
                return this->_M_single_erase(_key);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            size_t count(Kv &&_value) const noexcept {
                return this->_M_multi_count(_value);
            }

            size_t count(Key const &_value) const noexcept {
                return this->_M_multi_count(_value);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            bool contains(Kv &&_value) const noexcept {
                return this->_M_contains(_value);
            }

            bool contains(Key const &_value) const noexcept {
                return this->_M_contains(_value);
            }

            template <class Kv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                    ValueComp, Kv, value_type)>
            node_type extract(Kv &&_key) {
                iterator _it = this->_M_find(_key);
                return _it != this->end() ? this->extract(_it) : node_type();
            }

            node_type extract(Key const &_key) {
                iterator _it = this->_M_find(_key);
                return _it != this->end() ? this->extract(_it) : node_type();
            }
        };

    }  // namespace MapImpl1
}  // namespace MyStd
