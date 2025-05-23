#pragma once
/*
红黑树是如何保证最深叶子和最浅叶子的深度差不超过 2 倍的呢？
他设定了这样 5 条规则：

1. 节点可以是红色或黑色的。
2. 根节点总是黑色的。
3. 所有叶子节点都是黑色（叶子节点就是 NULL）。
4. 红色节点的两个子节点必须都是黑色的。
5. 从任一节点到其所有叶子节点的路径都包含相同数量的黑色节点。

看起来好像很复杂，但实际上大多是废话，有用的只是 4 和 5 这两条。
规则 4
翻译一下就是：不得出现相邻的红色节点（相邻指两个节点是父子关系）。这条规则还有一个隐含的信息：黑色节点可以相邻！
规则 5
翻译一下就是：从根节点到所有底层叶子的距离（以黑色节点数量计），必须相等。
因为规则 4
的存在，红色节点不可能相邻，也就是说最深的枝干只能是：红-黑-红-黑-红-黑-红-黑。
结合规则 5 来看，也就是说每条枝干上的黑色节点数量必须相同，因为最深的枝干是 4
个黑节点了，所以最浅的枝干至少也得有 4 个节点全是黑色的：黑-黑-黑-黑。
可以看到，规则 4 和规则 5
联合起来实际上就保证了：最深枝干的深度不会超过最浅枝干的 2 倍。
*/

#include <cassert>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace MyStd {

    namespace RbTreeImpl1 {

#if __cpp_concepts && __cpp_lib_concepts
#define _LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(__category, _Type) \
    __category _Type
#else
#define _LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(__category, _Type)             \
    class _Type, std::enable_if_t<std::is_convertible_v<                      \
                     typename std::iterator_traits<_Type>::iterator_category, \
                     __category##_tag>>
#endif
#define _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(_Compare, _Tv, Tp)           \
    class _Compare##Tp = _Compare,                                            \
          class = typename _Compare##Tp::is_transparent,                      \
          class =                                                             \
              decltype(std::declval<bool &>() = std::declval<_Compare##Tp>()( \
                           std::declval<_Tv>(), std::declval<Tp>()) =         \
                           std::declval<_Compare##Tp>()(std::declval<Tp>(),   \
                                                        std::declval<_Tv>()))

#if __cpp_lib_three_way_comparison
#define _LIBPENGCXX_DEFINE_COMPARISON(_Type)                         \
    bool operator==(_Type const &other) const noexcept {             \
        return std::equal(this->begin(), this->end(), other.begin(), \
                          other.end());                              \
    }                                                                \
                                                                     \
    auto operator<=>(_Type const &other) const noexcept {            \
        return std::lexicographical_compare_three_way(               \
            this->begin(), this->end(), other.begin(), other.end()); \
    }
#else
#define _LIBPENGCXX_DEFINE_COMPARISON(_Type)                             \
    bool operator==(_Type const &other) const noexcept {                 \
        return std::equal(this->begin(), this->end(), other.begin(),     \
                          other.end());                                  \
    }                                                                    \
                                                                         \
    bool operator!=(_Type const &other) const noexcept {                 \
        return !(*this == other);                                        \
    }                                                                    \
                                                                         \
    bool operator<(_Type const &other) const noexcept {                  \
        return std::lexicographical_compare(this->begin(), this->end(),  \
                                            other.begin(), other.end()); \
    }                                                                    \
                                                                         \
    bool operator>(_Type const &other) const noexcept {                  \
        return other < *this;                                            \
    }                                                                    \
                                                                         \
    bool operator<=(_Type const &other) const noexcept {                 \
        return !(other < *this);                                         \
    }                                                                    \
                                                                         \
    bool operator>=(_Type const &other) const noexcept {                 \
        return !(*this < other);                                         \
    }
#endif

        enum RbTreeColor {
            S_black,
            S_red,
        };

        enum RbTreeChildDir {
            S_left,
            S_right,
        };

        struct RbTreeNode {
            RbTreeNode *M_left;      // 左子节点指针
            RbTreeNode *M_right;     // 右子节点指针
            RbTreeNode *M_parent;    // 父节点指针
            RbTreeNode **M_pparent;  // 父节点中指向本节点指针的指针
            RbTreeColor M_color;     // 红或黑
        };

        template <class Tp>
        struct RbTreeNodeImpl : RbTreeNode {
            union {
                Tp M_value;
            };  // union 可以阻止里面成员的自动初始化，方便不支持 Tp() 默认构造的类型

            template <class... Ts>
            void M_construct(Ts &&..._value) noexcept {
                new (const_cast<std::remove_const_t<Tp> *>(
                    std::addressof(M_value))) Tp(std::forward<Ts>(_value)...);
            }

            void M_destruct() noexcept { M_value.~Tp(); }

            RbTreeNodeImpl() noexcept {}

            ~RbTreeNodeImpl() noexcept {}
        };

        template <bool>
        struct RbTreeIteratorBase;

        template <>
        struct RbTreeIteratorBase<false> {
          protected:
            union {
                RbTreeNode *M_node;
                RbTreeNode **M_proot;
            };

            bool M_off_by_one;

            RbTreeIteratorBase(RbTreeNode *_node) noexcept
                : M_node(_node), M_off_by_one(false) {}

            RbTreeIteratorBase(RbTreeNode **__proot) noexcept
                : M_proot(__proot), M_off_by_one(true) {}

            template <class, class, class, class>
            friend struct RbTreeImpl;

            template <class, class, bool>
            friend struct RbTreeIterator;

          public:
            bool operator==(RbTreeIteratorBase const &other) const noexcept {
                return (!M_off_by_one && !other.M_off_by_one &&
                        M_node == other.M_node) ||
                       (M_off_by_one && other.M_off_by_one);
            }

            bool operator!=(RbTreeIteratorBase const &other) const noexcept {
                return !(*this == other);
            }

            void operator++() noexcept {  // ++__it
                // 为了支持 ++rbegin()
                if (M_off_by_one) {
                    M_off_by_one = false;
                    M_node = *M_proot;
                    assert(M_node);
                    while (M_node->M_left != nullptr) {
                        M_node = M_node->M_left;
                    }
                    return;
                }
                assert(M_node);
                if (M_node->M_right != nullptr) {
                    M_node = M_node->M_right;
                    while (M_node->M_left != nullptr) {
                        M_node = M_node->M_left;
                    }
                } else {
                    while (M_node->M_parent != nullptr &&
                           M_node->M_pparent == &M_node->M_parent->M_right) {
                        M_node = M_node->M_parent;
                    }
                    if (M_node->M_parent == nullptr) {
                        M_off_by_one = true;
                        return;
                    }
                    M_node = M_node->M_parent;
                }
            }

            void operator--() noexcept {  // --__it
                // 为了支持 --end()
                if (M_off_by_one) {
                    M_off_by_one = false;
                    M_node = *M_proot;
                    assert(M_node);
                    while (M_node->M_right != nullptr) {
                        M_node = M_node->M_right;
                    }
                    return;
                }
                assert(M_node);
                if (M_node->M_left != nullptr) {
                    M_node = M_node->M_left;
                    while (M_node->M_right != nullptr) {
                        M_node = M_node->M_right;
                    }
                } else {
                    while (M_node->M_parent != nullptr &&
                           M_node->M_pparent == &M_node->M_parent->M_left) {
                        M_node = M_node->M_parent;
                    }
                    if (M_node->M_parent == nullptr) {
                        M_off_by_one = true;
                        return;
                    }
                    M_node = M_node->M_parent;
                }
            }

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
        };

        template <>
        struct RbTreeIteratorBase<true> : RbTreeIteratorBase<false> {
          protected:
            using RbTreeIteratorBase<false>::RbTreeIteratorBase;

          public:
            void operator++() noexcept {  // ++__it
                RbTreeIteratorBase<false>::operator--();
            }

            void operator--() noexcept {  // --__it
                RbTreeIteratorBase<false>::operator++();
            }
        };

        template <class _NodeImpl, class Tp, bool _Reverse>
        struct RbTreeIterator : RbTreeIteratorBase<_Reverse> {
          protected:
            using RbTreeIteratorBase<_Reverse>::RbTreeIteratorBase;

          public:
            template <class T0 = Tp>
            explicit operator std::enable_if_t<
                std::is_const_v<T0>,
                RbTreeIterator<_NodeImpl, std::remove_const_t<T0>, _Reverse>>()
                const noexcept {
                if (!this->_M_off_by_one) {
                    return this->_M_node;
                } else {
                    return this->_M_proot;
                }
            }

            template <class T0 = Tp>
            operator std::enable_if_t<
                !std::is_const_v<T0>,
                RbTreeIterator<_NodeImpl, std::add_const_t<T0>, _Reverse>>()
                const noexcept {
                if (!this->_M_off_by_one) {
                    return this->_M_node;
                } else {
                    return this->_M_proot;
                }
            }

            RbTreeIterator &operator++() noexcept {  // ++__it
                RbTreeIteratorBase<_Reverse>::operator++();
                return *this;
            }

            RbTreeIterator &operator--() noexcept {  // --__it
                RbTreeIteratorBase<_Reverse>::operator--();
                return *this;
            }

            RbTreeIterator operator++(int) noexcept {  // __it++
                RbTreeIterator __tmp = *this;
                ++*this;
                return __tmp;
            }

            RbTreeIterator operator--(int) noexcept {  // __it--
                RbTreeIterator __tmp = *this;
                --*this;
                return __tmp;
            }

            Tp *operator->() const noexcept {
                assert(!this->_M_off_by_one);
                return std::addressof(
                    static_cast<_NodeImpl *>(this->_M_node)->M_value);
            }

            Tp &operator*() const noexcept {
                assert(!this->_M_off_by_one);
                return static_cast<_NodeImpl *>(this->_M_node)->M_value;
            }

#ifndef NDEBUG
            _NodeImpl *_M_node_ptr() const noexcept {
                return this->_M_off_by_one
                           ? nullptr
                           : static_cast<_NodeImpl *>(this->_M_node);
            }
#endif

            using value_type = std::remove_const_t<Tp>;
            using reference = Tp &;
            using pointer = Tp *;
        };

        struct _RbTreeRoot {
            RbTreeNode *_M_root;
        };

        struct _RbTreeBase {
          protected:
            _RbTreeRoot *_M_block;

            explicit _RbTreeBase(_RbTreeRoot *__block) : _M_block(__block) {}

            template <class _Type, class _Alloc>
            static _Type *_M_allocate(_Alloc __alloc) {
                typename std::allocator_traits<_Alloc>::template rebind_alloc<
                    _Type>
                    __rebind_alloc(__alloc);
                return std::allocator_traits<_Alloc>::template rebind_traits<
                    _Type>::allocate(__rebind_alloc, sizeof(_Type));
            }

            template <class _Type, class _Alloc>
            static void _M_deallocate(_Alloc __alloc, void *__ptr) noexcept {
                typename std::allocator_traits<_Alloc>::template rebind_alloc<
                    _Type>
                    __rebind_alloc(__alloc);
                std::allocator_traits<_Alloc>::template rebind_traits<
                    _Type>::deallocate(__rebind_alloc,
                                       static_cast<_Type *>(__ptr),
                                       sizeof(_Type));
            }

            static void _M_rotate_left(RbTreeNode *_node) noexcept {
                RbTreeNode *__right = _node->M_right;
                _node->M_right = __right->M_left;
                if (__right->M_left != nullptr) {
                    __right->M_left->M_parent = _node;
                    __right->M_left->M_pparent = &_node->M_right;
                }
                __right->M_parent = _node->M_parent;
                __right->M_pparent = _node->M_pparent;
                *_node->M_pparent = __right;
                __right->M_left = _node;
                _node->M_parent = __right;
                _node->M_pparent = &__right->M_left;
            }

            static void _M_rotate_right(RbTreeNode *_node) noexcept {
                RbTreeNode *__left = _node->M_left;
                _node->M_left = __left->M_right;
                if (__left->M_right != nullptr) {
                    __left->M_right->M_parent = _node;
                    __left->M_right->M_pparent = &_node->M_left;
                }
                __left->M_parent = _node->M_parent;
                __left->M_pparent = _node->M_pparent;
                *_node->M_pparent = __left;
                __left->M_right = _node;
                _node->M_parent = __left;
                _node->M_pparent = &__left->M_right;
            }

            static void _M_fix_violation(RbTreeNode *_node) noexcept {
                while (true) {
                    RbTreeNode *__parent = _node->M_parent;
                    if (__parent ==
                        nullptr) {  // 根节点的 __parent 总是 nullptr
                        // 情况 0: _node == root
                        _node->M_color = S_black;
                        return;
                    }
                    if (_node->M_color == S_black ||
                        __parent->M_color == S_black) {
                        return;
                    }
                    RbTreeNode *__uncle;
                    RbTreeNode *__grandpa = __parent->M_parent;
                    assert(__grandpa);
                    RbTreeChildDir __parent_dir =
                        __parent->M_pparent == &__grandpa->M_left ? S_left
                                                                  : S_right;
                    if (__parent_dir == S_left) {
                        __uncle = __grandpa->M_right;
                    } else {
                        assert(__parent->M_pparent == &__grandpa->M_right);
                        __uncle = __grandpa->M_left;
                    }
                    RbTreeChildDir __node_dir =
                        _node->M_pparent == &__parent->M_left ? S_left
                                                              : S_right;
                    if (__uncle != nullptr && __uncle->M_color == S_red) {
                        // 情况 1: 叔叔是红色人士
                        __parent->M_color = S_black;
                        __uncle->M_color = S_black;
                        __grandpa->M_color = S_red;
                        _node = __grandpa;
                    } else if (__node_dir == __parent_dir) {
                        if (__node_dir == S_right) {
                            assert(_node->M_pparent == &__parent->M_right);
                            // 情况 2: 叔叔是黑色人士（RR）
                            _RbTreeBase::_M_rotate_left(__grandpa);
                        } else {
                            // 情况 3: 叔叔是黑色人士（LL）
                            _RbTreeBase::_M_rotate_right(__grandpa);
                        }
                        std::swap(__parent->M_color, __grandpa->M_color);
                        _node = __grandpa;
                    } else {
                        if (__node_dir == S_right) {
                            assert(_node->M_pparent == &__parent->M_right);
                            // 情况 4: 叔叔是黑色人士（LR）
                            _RbTreeBase::_M_rotate_left(__parent);
                        } else {
                            // 情况 5: 叔叔是黑色人士（RL）
                            _RbTreeBase::_M_rotate_right(__parent);
                        }
                        _node = __parent;
                    }
                }
            }

            RbTreeNode *_M_min_node() const noexcept {
                RbTreeNode *__current = _M_block->_M_root;
                if (__current != nullptr) {
                    while (__current->M_left != nullptr) {
                        __current = __current->M_left;
                    }
                }
                return __current;
            }

            RbTreeNode *_M_max_node() const noexcept {
                RbTreeNode *__current = _M_block->_M_root;
                if (__current != nullptr) {
                    while (__current->M_right != nullptr) {
                        __current = __current->M_right;
                    }
                }
                return __current;
            }

            template <class _NodeImpl, class _Tv, class _Compare>
            RbTreeNode *_M_find_node(_Tv &&_value,
                                     _Compare __comp) const noexcept {
                RbTreeNode *__current = _M_block->_M_root;
                while (__current != nullptr) {
                    if (__comp(_value,
                               static_cast<_NodeImpl *>(__current)->M_value)) {
                        __current = __current->M_left;
                        continue;
                    }
                    if (__comp(static_cast<_NodeImpl *>(__current)->M_value,
                               _value)) {
                        __current = __current->M_right;
                        continue;
                    }
                    // _value == curent->M_value
                    return __current;
                }
                return nullptr;
            }

            template <class _NodeImpl, class _Tv, class _Compare>
            RbTreeNode *_M_lower_bound(_Tv &&_value,
                                       _Compare __comp) const noexcept {
                RbTreeNode *__current = _M_block->_M_root;
                RbTreeNode *__result = nullptr;
                while (__current != nullptr) {
                    if (!(__comp(static_cast<_NodeImpl *>(__current)->M_value,
                                 _value))) {  // __current->M_value >= _value
                        __result = __current;
                        __current = __current->M_left;
                    } else {
                        __current = __current->M_right;
                    }
                }
                return __result;
            }

            template <class _NodeImpl, class _Tv, class _Compare>
            RbTreeNode *_M_upper_bound(_Tv &&_value,
                                       _Compare __comp) const noexcept {
                RbTreeNode *__current = _M_block->_M_root;
                RbTreeNode *__result = nullptr;
                while (__current != nullptr) {
                    if (__comp(_value,
                               static_cast<_NodeImpl *>(__current)
                                   ->M_value)) {  // __current->M_value > _value
                        __result = __current;
                        __current = __current->M_left;
                    } else {
                        __current = __current->M_right;
                    }
                }
                return __result;
            }

            template <class _NodeImpl, class _Tv, class _Compare>
            std::pair<RbTreeNode *, RbTreeNode *> _M_equal_range(
                _Tv &&_value, _Compare __comp) const noexcept {
                return {this->_M_lower_bound<_NodeImpl>(_value, __comp),
                        this->_M_upper_bound<_NodeImpl>(_value, __comp)};
            }

            static void _M_transplant(RbTreeNode *_node,
                                      RbTreeNode *__replace) noexcept {
                *_node->M_pparent = __replace;
                if (__replace != nullptr) {
                    __replace->M_parent = _node->M_parent;
                    __replace->M_pparent = _node->M_pparent;
                }
            }

            static void _M_delete_fixup(RbTreeNode *_node) noexcept {
                if (_node == nullptr) {
                    return;
                }
                while (_node->M_parent != nullptr &&
                       _node->M_color == S_black) {
                    RbTreeChildDir __dir =
                        _node->M_pparent == &_node->M_parent->M_left ? S_left
                                                                     : S_right;
                    RbTreeNode *__sibling = __dir == S_left
                                                ? _node->M_parent->M_right
                                                : _node->M_parent->M_left;
                    if (__sibling->M_color == S_red) {
                        __sibling->M_color = S_black;
                        _node->M_parent->M_color = S_red;
                        if (__dir == S_left) {
                            _RbTreeBase::_M_rotate_left(_node->M_parent);
                        } else {
                            _RbTreeBase::_M_rotate_right(_node->M_parent);
                        }
                        __sibling = __dir == S_left ? _node->M_parent->M_right
                                                    : _node->M_parent->M_left;
                    }
                    if (__sibling->M_left->M_color == S_black &&
                        __sibling->M_right->M_color == S_black) {
                        __sibling->M_color = S_red;
                        _node = _node->M_parent;
                    } else {
                        if (__dir == S_left &&
                            __sibling->M_right->M_color == S_black) {
                            __sibling->M_left->M_color = S_black;
                            __sibling->M_color = S_red;
                            _RbTreeBase::_M_rotate_right(__sibling);
                            __sibling = _node->M_parent->M_right;
                        } else if (__dir == S_right &&
                                   __sibling->M_left->M_color == S_black) {
                            __sibling->M_right->M_color = S_black;
                            __sibling->M_color = S_red;
                            _RbTreeBase::_M_rotate_left(__sibling);
                            __sibling = _node->M_parent->M_left;
                        }
                        __sibling->M_color = _node->M_parent->M_color;
                        _node->M_parent->M_color = S_black;
                        if (__dir == S_left) {
                            __sibling->M_right->M_color = S_black;
                            _RbTreeBase::_M_rotate_left(_node->M_parent);
                        } else {
                            __sibling->M_left->M_color = S_black;
                            _RbTreeBase::_M_rotate_right(_node->M_parent);
                        }
                        // while (_node->_M_parent != nullptr) {
                        //     _node = _node->_M_parent;
                        // }
                        break;
                    }
                }
                _node->M_color = S_black;
            }

            static void _M_erase_node(RbTreeNode *_node) noexcept {
                if (_node->M_left == nullptr) {
                    RbTreeNode *__right = _node->M_right;
                    RbTreeColor __color = _node->M_color;
                    _RbTreeBase::_M_transplant(_node, __right);
                    if (__color == S_black) {
                        _RbTreeBase::_M_delete_fixup(__right);
                    }
                } else if (_node->M_right == nullptr) {
                    RbTreeNode *__left = _node->M_left;
                    RbTreeColor __color = _node->M_color;
                    _RbTreeBase::_M_transplant(_node, __left);
                    if (__color == S_black) {
                        _RbTreeBase::_M_delete_fixup(__left);
                    }
                } else {
                    RbTreeNode *__replace = _node->M_right;
                    while (__replace->M_left != nullptr) {
                        __replace = __replace->M_left;
                    }
                    RbTreeNode *__right = __replace->M_right;
                    RbTreeColor __color = __replace->M_color;
                    if (__replace->M_parent == _node) {
                        if (__right != nullptr) {
                            __right->M_parent = __replace;
                            __right->M_pparent = &__replace->M_right;
                        }
                    } else {
                        _RbTreeBase::_M_transplant(__replace, __right);
                        __replace->M_right = _node->M_right;
                        __replace->M_right->M_parent = __replace;
                        __replace->M_right->M_pparent = &__replace->M_right;
                    }
                    _RbTreeBase::_M_transplant(_node, __replace);
                    __replace->M_left = _node->M_left;
                    __replace->M_left->M_parent = __replace;
                    __replace->M_left->M_pparent = &__replace->M_left;
                    if (__color == S_black) {
                        _RbTreeBase::_M_delete_fixup(__right);
                    }
                }
            }

            template <class _NodeImpl, class _Compare>
            RbTreeNode *_M_single_insert_node(RbTreeNode *_node,
                                              _Compare __comp) {
                RbTreeNode **__pparent = &_M_block->_M_root;
                RbTreeNode *__parent = nullptr;
                while (*__pparent != nullptr) {
                    __parent = *__pparent;
                    if (__comp(static_cast<_NodeImpl *>(_node)->M_value,
                               static_cast<_NodeImpl *>(__parent)->M_value)) {
                        __pparent = &__parent->M_left;
                        continue;
                    }
                    if (__comp(static_cast<_NodeImpl *>(__parent)->M_value,
                               static_cast<_NodeImpl *>(_node)->M_value)) {
                        __pparent = &__parent->M_right;
                        continue;
                    }
                    return __parent;
                }

                _node->M_left = nullptr;
                _node->M_right = nullptr;
                _node->M_color = S_red;

                _node->M_parent = __parent;
                _node->M_pparent = __pparent;
                *__pparent = _node;
                _RbTreeBase::_M_fix_violation(_node);
                return nullptr;
            }

            template <class _NodeImpl, class _Compare>
            void _M_multi_insert_node(RbTreeNode *_node, _Compare __comp) {
                RbTreeNode **__pparent = &_M_block->_M_root;
                RbTreeNode *__parent = nullptr;
                while (*__pparent != nullptr) {
                    __parent = *__pparent;
                    if (__comp(static_cast<_NodeImpl *>(_node)->M_value,
                               static_cast<_NodeImpl *>(__parent)->M_value)) {
                        __pparent = &__parent->M_left;
                        continue;
                    }
                    if (__comp(static_cast<_NodeImpl *>(__parent)->M_value,
                               static_cast<_NodeImpl *>(_node)->M_value)) {
                        __pparent = &__parent->M_right;
                        continue;
                    }
                    __pparent = &__parent->M_right;
                }

                _node->M_left = nullptr;
                _node->M_right = nullptr;
                _node->M_color = S_red;

                _node->M_parent = __parent;
                _node->M_pparent = __pparent;
                *__pparent = _node;
                _RbTreeBase::_M_fix_violation(_node);
            }
        };

        template <class Tp, class _Compare, class _Alloc, class _NodeImpl,
                  class = void>
        struct _RbTreeNodeHandle {
          protected:
            _NodeImpl *_M_node;
            [[no_unique_address]] _Alloc _M_alloc;

            _RbTreeNodeHandle(_NodeImpl *_node, _Alloc __alloc) noexcept
                : _M_node(_node), _M_alloc(__alloc) {}

            template <class, class, class, class>
            friend struct RbTreeImpl;

          public:
            _RbTreeNodeHandle() noexcept : _M_node(nullptr) {}

            _RbTreeNodeHandle(_RbTreeNodeHandle &&other) noexcept
                : _M_node(other._M_node) {
                other._M_node = nullptr;
            }

            _RbTreeNodeHandle &operator=(_RbTreeNodeHandle &&other) noexcept {
                std::swap(_M_node, other._M_node);
                return *this;
            }

            Tp &value() const noexcept {
                return static_cast<_NodeImpl *>(_M_node)->M_value;
            }

            ~_RbTreeNodeHandle() noexcept {
                if (_M_node) {
                    _RbTreeBase::_M_deallocate<_NodeImpl>(_M_alloc, _M_node);
                }
            }
        };

        template <class Tp, class _Compare, class _Alloc, class _NodeImpl>
        struct _RbTreeNodeHandle<
            Tp, _Compare, _Alloc, _NodeImpl,
            decltype((void)static_cast<typename _Compare::_RbTreeIsMap *>(
                nullptr))>
            : _RbTreeNodeHandle<Tp, _Compare, _Alloc, _NodeImpl, void *> {
            typename Tp::first_type &key() const noexcept {
                return this->value().first;
            }

            typename Tp::second_type &mapped() const noexcept {
                return this->value().second;
            }
        };

        template <class Tp, class _Compare, class _Alloc,
                  class _NodeImpl = RbTreeNodeImpl<Tp>>
        struct RbTreeImpl : protected _RbTreeBase {
          protected:
            [[no_unique_address]] _Compare _M_comp;
            [[no_unique_address]] _Alloc _M_alloc;

          public:
            RbTreeImpl() noexcept
                : _RbTreeBase(_RbTreeBase::_M_allocate<_RbTreeRoot>(_M_alloc)) {
                _M_block->_M_root = nullptr;
            }

            ~RbTreeImpl() noexcept {
                this->clear();
                _RbTreeBase::_M_deallocate<_RbTreeRoot>(_M_alloc, _M_block);
            }

            explicit RbTreeImpl(_Compare __comp) noexcept
                : _RbTreeBase(_RbTreeBase::_M_allocate<_RbTreeRoot>(_M_alloc)),
                  _M_comp(__comp) {
                _M_block->_M_root = nullptr;
            }

            explicit RbTreeImpl(_Alloc alloc,
                                _Compare __comp = _Compare()) noexcept
                : _RbTreeBase(_RbTreeBase::_M_allocate<_RbTreeRoot>(_M_alloc)),
                  _M_alloc(alloc),
                  _M_comp(__comp) {
                _M_block->_M_root = nullptr;
            }

            RbTreeImpl(RbTreeImpl &&other) noexcept
                : _RbTreeBase(other._M_block) {
                other._M_block =
                    _RbTreeBase::_M_allocate<_RbTreeRoot>(_M_alloc);
                other._M_block->_M_root = nullptr;
            }

            RbTreeImpl &operator=(RbTreeImpl &&other) noexcept {
                std::swap(_M_block, other._M_block);
                return *this;
            }

          protected:
            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void _M_single_insert(_InputIt __first, _InputIt __last) {
                while (__first != __last) {
                    this->_M_single_insert(*__first);
                    ++__first;
                }
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void _M_multi_insert(_InputIt __first, _InputIt __last) {
                while (__first != __last) {
                    this->_M_multi_insert(*__first);
                    ++__first;
                }
            }

          public:
            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::input_iterator, _InputIt)>
            void assign(_InputIt __first, _InputIt __last) {
                this->clear();
                this->_M_multi_insert(__first, __last);
            }

            using iterator = RbTreeIterator<_NodeImpl, Tp, false>;
            using reverse_iterator = RbTreeIterator<_NodeImpl, Tp, true>;
            using const_iterator = RbTreeIterator<_NodeImpl, Tp const, false>;
            using const_reverse_iterator =
                RbTreeIterator<_NodeImpl, Tp const, true>;

          protected:
            template <class _Tv>
            const_iterator _M_find(_Tv &&_value) const noexcept {
                return this->_M_prevent_end(
                    this->_M_find_node<_NodeImpl>(_value, _M_comp));
            }

            template <class _Tv>
            iterator _M_find(_Tv &&_value) noexcept {
                return this->_M_prevent_end(
                    this->_M_find_node<_NodeImpl>(_value, _M_comp));
            }

            template <class... Ts>
            iterator _M_multi_emplace(Ts &&..._value) {
                _NodeImpl *_node =
                    _RbTreeBase::_M_allocate<_NodeImpl>(_M_alloc);
                _node->M_construct(std::forward<Ts>(_value)...);
                this->_M_multi_insert_node<_NodeImpl>(_node, _M_comp);
                return _node;
            }

            template <class... Ts>
            std::pair<iterator, bool> _M_single_emplace(Ts &&..._value) {
                RbTreeNode *_node =
                    _RbTreeBase::_M_allocate<_NodeImpl>(_M_alloc);
                static_cast<_NodeImpl *>(_node)->M_construct(
                    std::forward<Ts>(_value)...);
                RbTreeNode *__conflict =
                    this->_M_single_insert_node<_NodeImpl>(_node, _M_comp);
                if (__conflict) {
                    static_cast<_NodeImpl *>(_node)->M_destruct();
                    _RbTreeBase::_M_deallocate<_NodeImpl>(_M_alloc, _node);
                    return {__conflict, false};
                } else {
                    return {_node, true};
                }
            }

          public:
            void clear() noexcept {
                iterator __it = this->begin();
                while (__it != this->end()) {
                    __it = this->erase(__it);
                }
            }

            iterator erase(const_iterator __it) noexcept {
                assert(__it != this->end());
                iterator __tmp(__it);
                ++__tmp;
                RbTreeNode *_node = __it._M_node;
                RbTreeImpl::_M_erase_node(_node);
                static_cast<_NodeImpl *>(_node)->M_destruct();
                _RbTreeBase::_M_deallocate<_NodeImpl>(_M_alloc, _node);
                return __tmp;
            }

            using node_type =
                _RbTreeNodeHandle<Tp, _Compare, _Alloc, _NodeImpl>;

            template <class... Ts>
            std::pair<iterator, bool> insert(node_type __nh) {
                _NodeImpl *_node = __nh._M_node;
                RbTreeNode *__conflict =
                    this->_M_single_insert_node<_NodeImpl>(_node, _M_comp);
                if (__conflict) {
                    static_cast<_NodeImpl *>(_node)->M_destruct();
                    return {__conflict, false};
                } else {
                    return {_node, true};
                }
            }

            node_type extract(const_iterator __it) noexcept {
                RbTreeNode *_node = __it._M_node;
                RbTreeImpl::_M_erase_node(_node);
                return {_node, _M_alloc};
            }

          protected:
            template <class _Tv>
            size_t _M_single_erase(_Tv &&_value) noexcept {
                RbTreeNode *_node =
                    this->_M_find_node<_NodeImpl>(_value, _M_comp);
                if (_node != nullptr) {
                    this->_M_erase_node(_node);
                    static_cast<_NodeImpl *>(_node)->M_destruct();
                    _RbTreeBase::_M_deallocate<_NodeImpl>(_M_alloc, _node);
                    return 1;
                } else {
                    return 0;
                }
            }

            std::pair<iterator, size_t> _M_erase_range(
                const_iterator __first, const_iterator __last) noexcept {
                size_t __num = 0;
                iterator __it(__first);
                while (__it != __last) {
                    __it = this->erase(__it);
                    ++__num;
                }
                return {__it, __num};
            }

            template <class _Tv>
            size_t _M_multi_erase(_Tv &&_value) noexcept {
                std::pair<iterator, iterator> __range =
                    this->equal_range(_value);
                return this->_M_erase_range(__range.first, __range.second)
                    .second;
            }

          public:
            iterator erase(const_iterator __first,
                           const_iterator __last) noexcept {
                return RbTreeImpl::_M_erase_range(__first, __last).first;
            }

            template <class _Tv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                     _Compare, _Tv, Tp)>
            iterator lower_bound(_Tv &&_value) noexcept {
                return this->_M_lower_bound<_NodeImpl>(_value, _M_comp);
            }

            template <class _Tv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                     _Compare, _Tv, Tp)>
            const_iterator lower_bound(_Tv &&_value) const noexcept {
                return this->_M_lower_bound<_NodeImpl>(_value, _M_comp);
            }

            template <class _Tv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                     _Compare, _Tv, Tp)>
            iterator upper_bound(_Tv &&_value) noexcept {
                return this->_M_upper_bound<_NodeImpl>(_value, _M_comp);
            }

            template <class _Tv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                     _Compare, _Tv, Tp)>
            const_iterator upper_bound(_Tv &&_value) const noexcept {
                return this->_M_upper_bound<_NodeImpl>(_value, _M_comp);
            }

            template <class _Tv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                     _Compare, _Tv, Tp)>
            std::pair<iterator, iterator> equal_range(_Tv &&_value) noexcept {
                return {this->lower_bound(_value), this->upper_bound(_value)};
            }

            template <class _Tv, _LIBPENGCXX_REQUIRES_TRANSPARENT_COMPARE(
                                     _Compare, _Tv, Tp)>
            std::pair<const_iterator, const_iterator> equal_range(
                _Tv &&_value) const noexcept {
                return {this->lower_bound(_value), this->upper_bound(_value)};
            }

            iterator lower_bound(Tp const &_value) noexcept {
                return this->_M_prevent_end(
                    this->_M_lower_bound<_NodeImpl>(_value, _M_comp));
            }

            const_iterator lower_bound(Tp const &_value) const noexcept {
                return this->_M_prevent_end(
                    this->_M_lower_bound<_NodeImpl>(_value, _M_comp));
            }

            iterator upper_bound(Tp const &_value) noexcept {
                return this->_M_prevent_end(
                    this->_M_upper_bound<_NodeImpl>(_value, _M_comp));
            }

            const_iterator upper_bound(Tp const &_value) const noexcept {
                return this->_M_prevent_end(
                    this->_M_upper_bound<_NodeImpl>(_value, _M_comp));
            }

            std::pair<iterator, iterator> equal_range(
                Tp const &_value) noexcept {
                return {this->lower_bound(_value), this->upper_bound(_value)};
            }

            std::pair<const_iterator, const_iterator> equal_range(
                Tp const &_value) const noexcept {
                return {this->lower_bound(_value), this->upper_bound(_value)};
            }

          protected:
            template <class _Tv>
            size_t _M_multi_count(_Tv &&_value) const noexcept {
                const_iterator __it = this->lower_bound(_value);
                return __it != end() ? std::distance(__it, this->upper_bound())
                                     : 0;
            }

            template <class _Tv>
            bool _M_contains(_Tv &&_value) const noexcept {
                return this->template _M_find_node<_NodeImpl>(
                           _value, _M_comp) != nullptr;
            }

            iterator _M_prevent_end(RbTreeNode *_node) noexcept {
                return _node == nullptr ? end() : _node;
            }

            reverse_iterator _M_prevent_rend(RbTreeNode *_node) noexcept {
                return _node == nullptr ? rend() : _node;
            }

            const_iterator _M_prevent_end(RbTreeNode *_node) const noexcept {
                return _node == nullptr ? end() : _node;
            }

            const_reverse_iterator _M_prevent_rend(
                RbTreeNode *_node) const noexcept {
                return _node == nullptr ? rend() : _node;
            }

          public:
            iterator begin() noexcept {
                return this->_M_prevent_end(this->_M_min_node());
            }

            reverse_iterator rbegin() noexcept {
                return this->_M_prevent_rend(this->_M_max_node());
            }

            iterator end() noexcept { return &_M_block->_M_root; }

            reverse_iterator rend() noexcept { return &_M_block->_M_root; }

            const_iterator begin() const noexcept {
                return this->_M_prevent_end(this->_M_min_node());
            }

            const_reverse_iterator rbegin() const noexcept {
                return this->_M_prevent_rend(this->_M_max_node());
            }

            const_iterator end() const noexcept { return &_M_block->_M_root; }

            const_reverse_iterator rend() const noexcept {
                return &_M_block->_M_root;
            }

#ifndef NDEBUG
            template <class Ostream>
            void M_print(Ostream &_os, RbTreeNode *_node) {
                if (_node) {
                    Tp &_value = static_cast<_NodeImpl *>(_node)->M_value;
                    _os << '(';
#if __cpp_concepts && __cpp_if_constexpr
                    if constexpr (requires(Tp _t) {
                                      _t.first;
                                      _t.second;
                                  }) {
                        _os << _value.first << ':' << _value.second;
                    } else {
                        _os << _value;
                    }
                    _os << ' ';
#endif
                    _os << (_node->M_color == S_black ? 'B' : 'R');
                    _os << ' ';
                    if (_node->M_left) {
                        if (_node->M_left->M_parent != _node ||
                            _node->M_left->M_pparent != &_node->M_left) {
                            _os << '*';
                        }
                    }
                    M_print(_os, _node->M_left);
                    _os << ' ';
                    if (_node->M_right) {
                        if (_node->M_right->M_parent != _node ||
                            _node->M_right->M_pparent != &_node->M_right) {
                            _os << '*';
                        }
                    }
                    M_print(_os, _node->M_right);
                    _os << ')';
                } else {
                    _os << '.';
                }
            }

            template <class Ostream>
            void M_print(Ostream &_os) {
                M_print(_os, this->_M_block->_M_root);
                _os << '\n';
            }
#endif

            bool empty() const noexcept {
                return this->_M_block->_M_root == nullptr;
            }

            size_t size() const noexcept {
                return std::distance(this->begin(), this->end());
            }
        };

    }  // namespace RbTreeImpl1

}  // namespace MyStd
