#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

namespace MyStd {
    namespace VectorImpl1 {
#if __cpp_concepts && __cpp_lib_concepts
#define _LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(__category, _Type) \
    __category _Type
#else
#define _LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(__category, _Type)             \
    class _Type, std::enable_if_t<std::is_convertible_v<                      \
                     typename std::iterator_traits<_Type>::iterator_category, \
                     __category##_tag>>
#endif

        template <class Tp, class Alloc = std::allocator<Tp>>
        struct Vector {
          public:
            using value_type = Tp;
            using allocator_type = Alloc;
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

          private:
            Tp *M_data{nullptr};
            std::size_t M_size{0};
            std::size_t M_cap{0};
            [[no_unique_address]] Alloc M_alloc;

          public:
            Vector() noexcept = default;

            Vector(std::initializer_list<Tp> _ilist,
                   Alloc const &alloc = Alloc())
                : Vector(_ilist.begin(), _ilist.end(), alloc) {}

            explicit Vector(std::size_t _n, Alloc const &alloc = Alloc())
                : M_cap(M_size = _n), M_alloc(alloc) {
                M_data = M_alloc.allocate(_n);

                for (std::size_t _i = 0; _i != _n; _i++) {
                    std::construct_at(&M_data[_i]);  // _M_data[i] = 0
                }
            }

            Vector(std::size_t _n, Tp const &val, Alloc const &alloc = Alloc())
                : M_cap(M_size = _n), M_alloc(alloc) {
                M_data = M_alloc.allocate(_n);

                for (std::size_t _i = 0; _i != _n; _i++) {
                    std::construct_at(&M_data[_i],
                                      val);  // _M_data[i] = val
                }
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::random_access_iterator, _InputIt)>
            Vector(_InputIt _first, _InputIt _last,
                   Alloc const &alloc = Alloc())
                : M_alloc(alloc) {
                std::size_t _n = _last - _first;
                M_data = M_alloc.allocate(_n);
                M_cap = M_size = _n;
                for (std::size_t _i = 0; _i != _n; _i++) {
                    std::construct_at(&M_data[_i], *_first);
                    ++_first;
                }
            }

            void clear() noexcept {
                for (std::size_t _i = 0; _i != M_size; _i++) {
                    std::destroy_at(&M_data[_i]);
                }
                M_size = 0;
            }

            void resize(std::size_t _n) {
                if (_n < M_size) {
                    for (std::size_t _i = _n; _i != M_size; _i++) {
                        std::destroy_at(&M_data[_i]);
                    }
                    M_size = _n;
                } else if (_n > M_size) {
                    reserve(_n);
                    for (std::size_t _i = M_size; _i != _n; _i++) {
                        std::construct_at(&M_data[_i]);  // _M_data[i] = 0
                    }
                }
                M_size = _n;
            }

            void resize(std::size_t n, Tp const &val) {
                if (n < M_size) {
                    for (std::size_t i = n; i != M_size; i++) {
                        std::destroy_at(&M_data[i]);
                    }
                    M_size = n;
                } else if (n > M_size) {
                    reserve(n);
                    for (std::size_t i = M_size; i != n; i++) {
                        std::construct_at(&M_data[i],
                                          val);  // _M_data[i] = val
                    }
                }
                M_size = n;
            }

            void shrink_to_fit() noexcept {
                auto _old_data = M_data;
                auto _old_cap = M_cap;
                M_cap = M_size;
                if (M_size == 0) {
                    M_data = nullptr;
                } else {
                    M_data = M_alloc.allocate(M_size);
                }
                if (_old_cap != 0) [[likely]] {
                    for (std::size_t i = 0; i != M_size; i++) {
                        std::construct_at(
                            &M_data[i],
                            std::move_if_noexcept(
                                _old_data
                                    [i]));  // _M_data[i] = std::move(__old_data[i])
                        std::destroy_at(&_old_data[i]);
                    }
                    M_alloc.deallocate(_old_data, _old_cap);
                }
            }

            void reserve(std::size_t n) {
                if (n <= M_cap) return;
                n = std::max(n, M_cap * 2);
                /* printf("grow from %zd to %zd\n", _M_cap, n); */
                auto _old_data = M_data;
                auto _old_cap = M_cap;
                if (n == 0) {
                    M_data = nullptr;
                    M_cap = 0;
                } else {
                    M_data = M_alloc.allocate(n);
                    M_cap = n;
                }
                if (_old_cap != 0) {
                    for (std::size_t i = 0; i != M_size; i++) {
                        std::construct_at(&M_data[i],
                                          std::move_if_noexcept(_old_data[i]));
                    }
                    for (std::size_t i = 0; i != M_size; i++) {
                        std::destroy_at(&_old_data[i]);
                    }
                    M_alloc.deallocate(_old_data, _old_cap);
                }
            }

            std::size_t capacity() const noexcept { return M_cap; }

            std::size_t size() const noexcept { return M_size; }

            bool empty() const noexcept { return M_size == 0; }

            static constexpr std::size_t max_size() noexcept {
                return std::numeric_limits<std::size_t>::max() / sizeof(Tp);
            }

            Tp const &operator[](std::size_t i) const noexcept {
                return M_data[i];
            }

            Tp &operator[](std::size_t i) noexcept { return M_data[i]; }

            Tp const &at(std::size_t i) const {
                if (i >= M_size) [[unlikely]]
                    throw std::out_of_range("vector::at");
                return M_data[i];
            }

            Tp &at(std::size_t i) {
                if (i >= M_size) [[unlikely]]
                    throw std::out_of_range("vector::at");
                return M_data[i];
            }

            Vector(Vector &&other) noexcept
                : M_data(other.M_data),
                  M_size(other.M_size),
                  M_cap(other.M_cap),
                  M_alloc(std::move(other.M_alloc)) {
                other.M_data = nullptr;
                other.M_size = 0;
                other.M_cap = 0;
            }

            Vector(Vector &&other, Alloc const &alloc) noexcept
                : M_data(other.M_data),
                  M_size(other.M_size),
                  M_cap(other.M_cap),
                  M_alloc(alloc) {
                other.M_data = nullptr;
                other.M_size = 0;
                other.M_cap = 0;
            }

            Vector &operator=(Vector &&other) noexcept {
                if (&other == this) [[unlikely]]
                    return *this;
                for (std::size_t i = 0; i != M_size; i++) {
                    std::destroy_at(&M_data[i]);
                }
                if (M_cap != 0) {
                    M_alloc.deallocate(M_data, M_cap);
                }
                M_data = other.M_data;
                M_size = other.M_size;
                M_cap = other.M_cap;
                other.M_data = nullptr;
                other.M_size = 0;
                other.M_cap = 0;
                return *this;
            }

            void swap(Vector &other) noexcept {
                std::swap(M_data, other.M_data);
                std::swap(M_size, other.M_size);
                std::swap(M_cap, other.M_cap);
                std::swap(M_alloc, other.M_alloc);
            }

            Vector(Vector const &other)
                : M_cap(M_size = other.M_size), M_alloc(other.M_alloc) {
                if (M_size != 0) {
                    M_data = M_alloc.allocate(M_size);
                    for (std::size_t i = 0; i != M_size; i++) {
                        std::construct_at(&M_data[i],
                                          std::as_const(other.M_data[i]));
                    }
                } else {
                    M_data = nullptr;
                }
            }

            Vector(Vector const &other, Alloc const &alloc)
                : M_cap(M_size = other.M_size), M_alloc(alloc) {
                if (M_size != 0) {
                    M_data = M_alloc.allocate(M_size);
                    for (std::size_t i = 0; i != M_size; i++) {
                        std::construct_at(&M_data[i],
                                          std::as_const(other.M_data[i]));
                    }
                } else {
                    M_data = nullptr;
                }
            }

            Vector &operator=(Vector const &other) {
                if (&other == this) [[unlikely]]
                    return *this;
                reserve(other.M_size);
                M_size = other.M_size;
                for (std::size_t i = 0; i != M_size; i++) {
                    std::construct_at(&M_data[i],
                                      std::as_const(other.M_data[i]));
                }
                return *this;
            }

            Tp const &front() const noexcept { return *M_data; }

            Tp &front() noexcept { return *M_data; }

            Tp const &back() const noexcept { return M_data[M_size - 1]; }

            Tp &back() noexcept { return M_data[M_size - 1]; }

            void push_back(Tp const &val) {
                if (M_size + 1 >= M_cap) [[unlikely]]
                    reserve(M_size + 1);
                std::construct_at(&M_data[M_size], val);
                M_size = M_size + 1;
            }

            void push_back(Tp &&val) {
                if (M_size + 1 >= M_cap) [[unlikely]]
                    reserve(M_size + 1);
                std::construct_at(&M_data[M_size], std::move(val));
                M_size = M_size + 1;
            }

            template <class... Args>
            Tp &emplace_back(Args &&..._args) {
                if (M_size + 1 >= M_cap) [[unlikely]]
                    reserve(M_size + 1);
                Tp *_p = &M_data[M_size];
                std::construct_at(_p, std::forward<Args>(_args)...);
                M_size = M_size + 1;
                return *_p;
            }

            Tp *data() noexcept { return M_data; }

            Tp const *data() const noexcept { return M_data; }

            Tp const *cdata() const noexcept { return M_data; }

            Tp *begin() noexcept { return M_data; }

            Tp *end() noexcept { return M_data + M_size; }

            Tp const *begin() const noexcept { return M_data; }

            Tp const *end() const noexcept { return M_data + M_size; }

            Tp const *cbegin() const noexcept { return M_data; }

            Tp const *cend() const noexcept { return M_data + M_size; }

            std::reverse_iterator<Tp *> rbegin() noexcept {
                return std::make_reverse_iterator(M_data + M_size);
            }

            std::reverse_iterator<Tp *> rend() noexcept {
                return std::make_reverse_iterator(M_data);
            }

            std::reverse_iterator<Tp const *> rbegin() const noexcept {
                return std::make_reverse_iterator(M_data + M_size);
            }

            std::reverse_iterator<Tp const *> rend() const noexcept {
                return std::make_reverse_iterator(M_data);
            }

            std::reverse_iterator<Tp const *> crbegin() const noexcept {
                return std::make_reverse_iterator(M_data + M_size);
            }

            std::reverse_iterator<Tp const *> crend() const noexcept {
                return std::make_reverse_iterator(M_data);
            }

            void pop_back() noexcept {
                M_size -= 1;
                std::destroy_at(&M_data[M_size]);
            }

            Tp *erase(Tp const *_it) noexcept(
                std::is_nothrow_move_assignable_v<Tp>) {
                std::size_t i = _it - M_data;
                for (std::size_t _j = i + 1; _j != M_size; _j++) {
                    M_data[_j - 1] = std::move(M_data[_j]);
                }
                M_size -= 1;
                std::destroy_at(&M_data[M_size]);
                return const_cast<Tp *>(_it);
            }

            Tp *erase(Tp const *_first, Tp const *_last) noexcept(
                std::is_nothrow_move_assignable_v<Tp>) {
                std::size_t diff = _last - _first;
                for (std::size_t _j = _last - M_data; _j != M_size; _j++) {
                    M_data[_j - diff] = std::move(M_data[_j]);
                }
                M_size -= diff;
                for (std::size_t _j = M_size; _j != M_size + diff; _j++) {
                    std::destroy_at(&M_data[_j]);
                }
                return const_cast<Tp *>(_first);
            }

            void assign(std::size_t n, Tp const &val) {
                clear();
                reserve(n);
                M_size = n;
                for (std::size_t i = 0; i != n; i++) {
                    std::construct_at(&M_data[i], val);
                }
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::random_access_iterator, _InputIt)>
            void assign(_InputIt first, _InputIt last) {
                clear();
                std::size_t n = last - first;
                reserve(n);
                M_size = n;
                for (std::size_t i = 0; i != n; i++) {
                    std::construct_at(M_data[i], *first);
                    ++first;
                }
            }

            void assign(std::initializer_list<Tp> ilist) {
                assign(ilist.begin(), ilist.end());
            }

            Vector &operator=(std::initializer_list<Tp> ilist) {
                assign(ilist.begin(), ilist.end());
                return *this;
            }

            template <class... Args>
            Tp *emplace(Tp const *_it, Args &&..._args) {
                std::size_t _j = _it - M_data;
                reserve(M_size + 1);
                // __j ~ _M_size => __j + 1 ~ _M_size + 1
                for (std::size_t i = M_size; i != _j; i--) {
                    std::construct_at(&M_data[i], std::move(M_data[i - 1]));
                    std::destroy_at(&M_data[i - 1]);
                }
                M_size += 1;
                std::construct_at(&M_data[_j], std::forward<Args>(_args)...);
                return M_data + _j;
            }

            Tp *insert(Tp const *_it, Tp &&val) {
                std::size_t _j = _it - M_data;
                reserve(M_size + 1);
                // __j ~ _M_size => __j + 1 ~ _M_size + 1
                for (std::size_t i = M_size; i != _j; i--) {
                    std::construct_at(&M_data[i], std::move(M_data[i - 1]));
                    std::destroy_at(&M_data[i - 1]);
                }
                M_size += 1;
                std::construct_at(&M_data[_j], std::move(val));
                return M_data + _j;
            }

            Tp *insert(Tp const *_it, Tp const &val) {
                std::size_t _j = _it - M_data;
                reserve(M_size + 1);
                // __j ~ _M_size => __j + 1 ~ _M_size + 1
                for (std::size_t i = M_size; i != _j; i--) {
                    std::construct_at(&M_data[i], std::move(M_data[i - 1]));
                    std::destroy_at(&M_data[i - 1]);
                }
                M_size += 1;
                std::construct_at(&M_data[_j], val);
                return M_data + _j;
            }

            Tp *insert(Tp const *_it, std::size_t n, Tp const &val) {
                std::size_t _j = _it - M_data;
                if (n == 0) [[unlikely]]
                    return const_cast<Tp *>(_it);
                reserve(M_size + n);
                // __j ~ _M_size => __j + n ~ _M_size + n
                for (std::size_t i = M_size; i != _j; i--) {
                    std::construct_at(&M_data[i + n - 1],
                                      std::move(M_data[i - 1]));
                    std::destroy_at(&M_data[i - 1]);
                }
                M_size += n;
                for (std::size_t i = _j; i != _j + n; i++) {
                    std::construct_at(&M_data[i], val);
                }
                return M_data + _j;
            }

            template <_LIBPENGCXX_REQUIRES_ITERATOR_CATEGORY(
                std::random_access_iterator, _InputIt)>
            Tp *insert(Tp const *_it, _InputIt first, _InputIt last) {
                std::size_t _j = _it - M_data;
                std::size_t n = last - first;
                if (n == 0) [[unlikely]]
                    return const_cast<Tp *>(_it);
                reserve(M_size + n);
                // __j ~ _M_size => __j + n ~ _M_size + n
                for (std::size_t i = M_size; i != _j; i--) {
                    std::construct_at(&M_data[i + n - 1],
                                      std::move(M_data[i - 1]));
                    std::destroy_at(&M_data[i - 1]);
                }
                M_size += n;
                for (std::size_t i = _j; i != _j + n; i++) {
                    std::construct_at(&M_data[i], *first);
                    ++first;
                }
                return M_data + _j;
            }

            Tp *insert(Tp const *_it, std::initializer_list<Tp> ilist) {
                return insert(_it, ilist.begin(), ilist.end());
            }

            ~Vector() noexcept {
                for (std::size_t i = 0; i != M_size; i++) {
                    std::destroy_at(&M_data[i]);
                }
                if (M_cap != 0) {
                    M_alloc.deallocate(M_data, M_cap);
                }
            }

            Alloc get_allocator() const noexcept { return M_alloc; }
        };

    }  // namespace VectorImpl1
}  // namespace MyStd