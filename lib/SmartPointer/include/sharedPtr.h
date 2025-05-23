#pragma once

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace MyStd {

    namespace SharedPtrImpl1 {
        template <typename T>
        class SharedPtr {
          public:
            explicit SharedPtr(T *p) : ptr(p), refCount(new int(1)) {}
            ~SharedPtr() { reset(); }

            SharedPtr(const SharedPtr &other)
                : ptr(other.ptr), refCount(other.refCount) {
                ++(*refCount);
            }

            SharedPtr &operator=(const SharedPtr &other) {
                if (this != &other) {
                    reset();
                    ptr = other.ptr;
                    refCount = other.refCount;
                    ++(*refCount);
                }
                return *this;
            }

            T &operator*() const { return *ptr; }
            T *operator->() const { return ptr; }

            T *get() const { return ptr; }

            int use_count() const { return *refCount; }

            void reset() {
                if (--(*refCount) == 0) {
                    delete ptr;
                    delete refCount;
                }
                ptr = nullptr;
                refCount = nullptr;
            }

          public:
            T *ptr;
            int *refCount;
        };
    }  // namespace SharedPtrImpl1

    namespace SharedPtrImpl2 {
        struct SpCounter {
            std::atomic<int64_t> M_refcnt;

            SpCounter() noexcept : M_refcnt(1) {}

            SpCounter(SpCounter &&) = delete;

            void M_incref() noexcept {
                M_refcnt.fetch_add(1, std::memory_order_relaxed);
            }

            void M_decref() noexcept {
                if (M_refcnt.fetch_sub(1, std::memory_order_relaxed) == 1) {
                    delete this;
                }
            }

            int64_t M_cntref() const noexcept {
                return M_refcnt.load(std::memory_order_relaxed);
            }

            virtual ~SpCounter() = default;
        };

        template <class Tp, class Deleter>
        struct SpCounterImpl final : SpCounter {
            Tp *M_ptr;
            [[no_unique_address]] Deleter M_deleter;

            explicit SpCounterImpl(Tp *_ptr) noexcept : M_ptr(_ptr) {}

            explicit SpCounterImpl(Tp *_ptr, Deleter _deleter) noexcept
                : M_ptr(_ptr), M_deleter(std::move(_deleter)) {}

            ~SpCounterImpl() noexcept override { M_deleter(M_ptr); }
        };

        template <class Tp, class Deleter>
        struct _SpCounterImplFused final : SpCounter {
            Tp *M_ptr;
            void *M_mem;
            [[no_unique_address]] Deleter M_deleter;

            explicit _SpCounterImplFused(Tp *_ptr, void *_mem,
                                         Deleter _deleter) noexcept
                : M_ptr(_ptr), M_mem(_mem), M_deleter(std::move(_deleter)) {}

            ~_SpCounterImplFused() noexcept override { M_deleter(M_ptr); }

            void operator delete(void *_mem) noexcept {
#if __cpp_aligned_new
                ::operator delete(
                    _mem, std::align_val_t(std::max(
                              alignof(Tp), alignof(_SpCounterImplFused))));
#else
                ::operator delete(_mem);
#endif
            }
        };

        template <class Tp>
        struct SharedPtr {
          private:
            Tp *M_ptr;
            SpCounter *M_owner{};

            template <class>
            friend struct SharedPtr;

            explicit SharedPtr(Tp *_ptr, SpCounter *_owner) noexcept
                : M_ptr(_ptr), M_owner(_owner) {}

          public:
            using element_type = Tp;
            using pointer = Tp *;

            SharedPtr(std::nullptr_t = nullptr) noexcept : M_owner(nullptr) {}

            template <class Yp, std::enable_if_t<
                                    std::is_convertible_v<Yp *, Tp *>, int> = 0>
            explicit SharedPtr(Yp *ptr)
                : M_ptr(ptr),
                  _M_owner(new _SpCounterImpl<Yp, DefaultDeleter<Yp>>(ptr)) {
                _S_setupEnableSharedFromThis(M_ptr, M_owner);
            }

            template <
                class Yp, class _Deleter,
                std::enable_if_t<std::is_convertible_v<Yp *, Tp *>, int> = 0>
            explicit SharedPtr(Yp *ptr, _Deleter _deleter)
                : M_ptr(ptr),
                  M_owner(new SpCounterImpl<Yp, _Deleter>(
                      ptr, std::move(_deleter))) {
                _S_setupEnableSharedFromThis(M_ptr, M_owner);
            }

            template <
                class Yp, class _Deleter,
                std::enable_if_t<std::is_convertible_v<Yp *, Tp *>, int> = 0>
            explicit SharedPtr(UniquePtr<Yp, _Deleter> &&ptr)
                : SharedPtr(ptr.release(), ptr.get_deleter()) {}

            template <class Yp>
            inline friend SharedPtr<Yp> _S_makeSharedFused(
                Yp *ptr, _SpCounter *owner) noexcept;

            SharedPtr(SharedPtr const &other) noexcept
                : M_ptr(other.M_ptr), M_owner(other.M_owner) {
                if (M_owner) {
                    M_owner->M_incref();
                }
            }

            template <class Yp, std::enable_if_t<
                                    std::is_convertible_v<Yp *, Tp *>, int> = 0>
            SharedPtr(SharedPtr<Yp> const &other) noexcept
                : M_ptr(other._M_ptr), M_owner(other._M_owner) {
                if (M_owner) {
                    M_owner->M_incref();
                }
            }

            SharedPtr(SharedPtr &&other) noexcept
                : M_ptr(other.M_ptr), M_owner(other.M_owner) {
                other.M_ptr = nullptr;
                other.M_owner = nullptr;
            }

            template <class Yp, std::enable_if_t<
                                    std::is_convertible_v<Yp *, Tp *>, int> = 0>
            SharedPtr(SharedPtr<Yp> &&other) noexcept
                : M_ptr(other._M_ptr), M_owner(other._M_owner) {
                other._M_ptr = nullptr;
                other._M_owner = nullptr;
            }

            template <class Yp>
            SharedPtr(SharedPtr<Yp> const &other, Tp *ptr) noexcept
                : M_ptr(ptr), M_owner(other._M_owner) {
                if (M_owner) {
                    M_owner->M_incref();
                }
            }

            template <class Yp>
            SharedPtr(SharedPtr<Yp> &&other, Tp *ptr) noexcept
                : M_ptr(ptr), M_owner(other._M_owner) {
                other._M_ptr = nullptr;
                other._M_owner = nullptr;
            }

            SharedPtr &operator=(SharedPtr const &other) noexcept {
                if (this == &other) {
                    return *this;
                }
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_ptr = other.M_ptr;
                M_owner = other.M_owner;
                if (M_owner) {
                    M_owner->M_incref();
                }
                return *this;
            }

            SharedPtr &operator=(SharedPtr &&other) noexcept {
                if (this == &other) {
                    return *this;
                }
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_ptr = other.M_ptr;
                M_owner = other.M_owner;
                other.M_ptr = nullptr;
                other.M_owner = nullptr;
                return *this;
            }

            template <class Yp, std::enable_if_t<
                                    std::is_convertible_v<Yp *, Tp *>, int> = 0>
            SharedPtr &operator=(SharedPtr<Yp> const &other) noexcept {
                if (this == &other) {
                    return *this;
                }
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_ptr = other._M_ptr;
                M_owner = other._M_owner;
                if (M_owner) {
                    M_owner->M_incref();
                }
                return *this;
            }

            template <class Yp, std::enable_if_t<
                                    std::is_convertible_v<Yp *, Tp *>, int> = 0>
            SharedPtr &operator=(SharedPtr<Yp> &&other) noexcept {
                if (this == &other) {
                    return *this;
                }
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_ptr = other._M_ptr;
                M_owner = other._M_owner;
                other._M_ptr = nullptr;
                other._M_owner = nullptr;
                return *this;
            }

            void reset() noexcept {
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_owner = nullptr;
                M_ptr = nullptr;
            }

            template <class Yp>
            void reset(Yp *ptr) {
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_ptr = nullptr;
                M_owner = nullptr;
                M_ptr = ptr;
                _M_owner = new _SpCounterImpl<Yp, DefaultDeleter<Yp>>(ptr);
                _S_setupEnableSharedFromThis(M_ptr, M_owner);
            }

            template <class Yp, class _Deleter>
            void reset(Yp *ptr, _Deleter _deleter) {
                if (M_owner) {
                    M_owner->M_decref();
                }
                M_ptr = nullptr;
                M_owner = nullptr;
                M_ptr = ptr;
                M_owner =
                    new SpCounterImpl<Yp, _Deleter>(ptr, std::move(_deleter));
                _S_setupEnableSharedFromThis(M_ptr, M_owner);
            }

            ~SharedPtr() noexcept {
                if (M_owner) {
                    M_owner->M_decref();
                }
            }

            int64_t use_count() noexcept {
                return M_owner ? M_owner->M_cntref() : 0;
            }

            bool unique() noexcept {
                return M_owner ? M_owner->M_cntref() == 1 : true;
            }

            template <class Yp>
            bool operator==(SharedPtr<Yp> const &other) const noexcept {
                return M_ptr == other._M_ptr;
            }

            template <class Yp>
            bool operator!=(SharedPtr<Yp> const &other) const noexcept {
                return M_ptr != other._M_ptr;
            }

            template <class Yp>
            bool operator<(SharedPtr<Yp> const &other) const noexcept {
                return M_ptr < other._M_ptr;
            }

            template <class Yp>
            bool operator<=(SharedPtr<Yp> const &other) const noexcept {
                return M_ptr <= other._M_ptr;
            }

            template <class Yp>
            bool operator>(SharedPtr<Yp> const &other) const noexcept {
                return M_ptr > other._M_ptr;
            }

            template <class Yp>
            bool operator>=(SharedPtr<Yp> const &other) const noexcept {
                return M_ptr >= other._M_ptr;
            }

            template <class Yp>
            bool owner_before(SharedPtr<Yp> const &other) const noexcept {
                return M_owner < other._M_owner;
            }

            template <class Yp>
            bool owner_equal(SharedPtr<Yp> const &other) const noexcept {
                return M_owner == other._M_owner;
            }

            void swap(SharedPtr &other) noexcept {
                std::swap(M_ptr, other.M_ptr);
                std::swap(M_owner, other.M_owner);
            }

            Tp *get() const noexcept { return M_ptr; }

            Tp *operator->() const noexcept { return M_ptr; }

            std::add_lvalue_reference_t<Tp> operator*() const noexcept {
                return *M_ptr;
            }

            explicit operator bool() const noexcept { return M_ptr != nullptr; }
        };

        template <class Tp>
        inline SharedPtr<Tp> _S_makeSharedFused(Tp *ptr,
                                                SpCounter *owner) noexcept {
            return SharedPtr<Tp>(ptr, owner);
        }

        template <class Tp>
        struct SharedPtr<Tp[]> : SharedPtr<Tp> {
            using SharedPtr<Tp>::SharedPtr;

            std::add_lvalue_reference_t<Tp> operator[](std::size_t __i) {
                return this->get()[__i];
            }
        };

        template <class Tp>
        struct EnableSharedFromThis {
          private:
            SpCounter *_M_owner;

          protected:
            EnableSharedFromThis() noexcept : _M_owner(nullptr) {}

            SharedPtr<Tp> shared_from_this() {
                static_assert(std::is_base_of_v<EnableSharedFromThis, Tp>,
                              "must be derived class");
                if (!_M_owner) {
                    throw std::bad_weak_ptr();
                }
                _M_owner->M_incref();
                return _S_makeSharedFused(static_cast<Tp *>(this), _M_owner);
            }

            SharedPtr<Tp const> shared_from_this() const {
                static_assert(std::is_base_of_v<EnableSharedFromThis, Tp>,
                              "must be derived class");
                if (!_M_owner) {
                    throw std::bad_weak_ptr();
                }
                _M_owner->M_incref();
                return _S_makeSharedFused(static_cast<Tp const *>(this),
                                          _M_owner);
            }

            template <class Up>
            inline friend void S_setEnableSharedFromThisOwner(
                EnableSharedFromThis<Up> *, SpCounter *);
        };

        template <class Up>
        inline void S_setEnableSharedFromThisOwner(
            EnableSharedFromThis<Up> *ptr, SpCounter *owner) {
            ptr->_M_owner = owner;
        }

        template <class Tp,
                  std::enable_if_t<
                      std::is_base_of_v<EnableSharedFromThis<Tp>, Tp>, int> = 0>
        void S_setupEnableSharedFromThis(Tp *ptr, SpCounter *owner) {
            _S_setEnableSharedFromThisOwner(
                static_cast<EnableSharedFromThis<Tp> *>(ptr), owner);
        }

        template <
            class Tp,
            std::enable_if_t<!std::is_base_of_v<EnableSharedFromThis<Tp>, Tp>,
                             int> = 0>
        void S_setupEnableSharedFromThis(Tp * /*unused*/,
                                         SpCounter * /*unused*/) {}

        template <class Tp, class... Args,
                  std::enable_if_t<!std::is_unbounded_array_v<Tp>, int> = 0>
        SharedPtr<Tp> makeShared(Args &&..._args) {
            auto const _deleter = [](Tp *ptr) noexcept { ptr->~Tp(); };
            using Counter = _SpCounterImplFused<Tp, decltype(_deleter)>;
            constexpr std::size_t _offset =
                std::max(alignof(Tp), sizeof(Counter));
            constexpr std::size_t _align =
                std::max(alignof(Tp), alignof(Counter));
            constexpr std::size_t _size = _offset + sizeof(Tp);
#if __cpp_aligned_new
            void *_mem = ::operator new(_size, std::align_val_t(_align));
            Counter *_counter = reinterpret_cast<Counter *>(_mem);
#else
            void *_mem = ::operator new(_size + _align);
            Counter *_counter = reinterpret_cast<_SpC *>(
                reinterpret_cast<std::size_t>(_mem) & _align);
#endif
            Tp *_object = reinterpret_cast<Tp *>(
                reinterpret_cast<char *>(_counter) + _offset);
            try {
                new (_object) Tp(std::forward<Args>(_args)...);
            } catch (...) {
#if __cpp_aligned_new
                ::operator delete(_mem, std::align_val_t(_align));
#else
                ::operator delete(_mem);
#endif
                throw;
            }
            new (_counter) Counter(_object, _mem, _deleter);
            _S_setupEnableSharedFromThis(_object, _counter);
            return _S_makeSharedFused(_object, _counter);
        }

        template <class Tp,
                  std::enable_if_t<!std::is_unbounded_array_v<Tp>, int> = 0>
        SharedPtr<Tp> makeSharedForOverwrite() {
            auto const _deleter = [](Tp *ptr) noexcept { ptr->~Tp(); };
            using Counter = _SpCounterImplFused<Tp, decltype(_deleter)>;
            constexpr std::size_t _offset =
                std::max(alignof(Tp), sizeof(Counter));
            constexpr std::size_t _align =
                std::max(alignof(Tp), alignof(Counter));
            constexpr std::size_t _size = _offset + sizeof(Tp);
#if __cpp_aligned_new
            void *_mem = ::operator new(_size, std::align_val_t(_align));
            auto *_counter = reinterpret_cast<Counter *>(_mem);
#else
            void *_mem = ::operator new(_size + _align);
            _SpC *_counter = reinterpret_cast<_SpC *>(
                reinterpret_cast<std::size_t>(_mem) & _align);
#endif
            Tp *_object = reinterpret_cast<Tp *>(
                reinterpret_cast<char *>(_counter) + _offset);
            try {
                new (_object) Tp;
            } catch (...) {
#if __cpp_aligned_new
                ::operator delete(_mem, std::align_val_t(_align));
#else
                ::operator delete(_mem);
#endif
                throw;
            }
            new (_counter) Counter(_object, _mem, _deleter);
            _S_setupEnableSharedFromThis(_object, _counter);
            return _S_makeSharedFused(_object, _counter);
        }

        template <class Tp, class... Args,
                  std::enable_if_t<std::is_unbounded_array_v<Tp>, int> = 0>
        SharedPtr<Tp> makeShared(std::size_t _len) {
            auto *_p = new std::remove_extent_t<Tp>[_len];
            try {
                return SharedPtr<Tp>(_p);
            } catch (...) {
                delete[] _p;
                throw;
            }
        }

        template <class Tp,
                  std::enable_if_t<std::is_unbounded_array_v<Tp>, int> = 0>
        SharedPtr<Tp> makeSharedForOverwrite(std::size_t _len) {
            auto *_p = new std::remove_extent_t<Tp>[_len];
            try {
                return SharedPtr<Tp>(_p);
            } catch (...) {
                delete[] _p;
                throw;
            }
        }

        template <class Tp, class Up>
        SharedPtr<Tp> staticPointerCast(SharedPtr<Up> const &ptr) {
            return SharedPtr<Tp>(ptr, static_cast<Tp *>(ptr.get()));
        }

        template <class Tp, class Up>
        SharedPtr<Tp> constPointerCast(SharedPtr<Up> const &ptr) {
            return SharedPtr<Tp>(ptr, const_cast<Tp *>(ptr.get()));
        }

        template <class Tp, class Up>
        SharedPtr<Tp> reinterpretPointerCast(SharedPtr<Up> const &ptr) {
            return SharedPtr<Tp>(ptr, reinterpret_cast<Tp *>(ptr.get()));
        }

        template <class Tp, class Up>
        SharedPtr<Tp> dynamicPointerCast(SharedPtr<Up> const &ptr) {
            Tp *_p = dynamic_cast<Tp *>(ptr.get());
            if (_p != nullptr) {
                return SharedPtr<Tp>(ptr, _p);
            }
            return nullptr;
        }

    }  // namespace SharedPtrImpl2
}  // namespace MyStd