#pragma once

#include <type_traits>
#include <utility>
#include <memory>
#include <functional>

namespace MyStd {

// 默认删除器
template <typename T>
struct DefaultDelete {
    constexpr DefaultDelete() noexcept = default;
    
    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
    DefaultDelete(const DefaultDelete<U>&) noexcept {}
    
    void operator()(T* ptr) const noexcept {
        static_assert(sizeof(T) > 0, "Cannot delete incomplete type");
        delete ptr;
    }
};

// 数组特化版本的默认删除器
template <typename T>
struct DefaultDelete<T[]> {
    constexpr DefaultDelete() noexcept = default;
    
    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U(*)[], T(*)[]>>>
    DefaultDelete(const DefaultDelete<U[]>&) noexcept {}
    
    template <typename U>
    std::enable_if_t<std::is_convertible_v<U(*)[], T(*)[]>>
    operator()(U* ptr) const noexcept {
        static_assert(sizeof(U) > 0, "Cannot delete incomplete type");
        delete[] ptr;
    }
};

// UniquePtr 主实现
template <typename T, typename Deleter = DefaultDelete<T>>
class UniquePtr {
public:
    using pointer = T*;
    using element_type = T;
    using deleter_type = Deleter;

private:
    pointer ptr_;
    [[no_unique_address]] Deleter deleter_;

public:
    // 构造函数
    constexpr UniquePtr() noexcept : ptr_(nullptr), deleter_() {
        static_assert(!std::is_pointer_v<Deleter>, "Deleter cannot be a pointer type");
    }
    
    constexpr UniquePtr(std::nullptr_t) noexcept : ptr_(nullptr), deleter_() {
        static_assert(!std::is_pointer_v<Deleter>, "Deleter cannot be a pointer type");
    }
    
    explicit UniquePtr(pointer ptr) noexcept : ptr_(ptr), deleter_() {
        static_assert(!std::is_pointer_v<Deleter>, "Deleter cannot be a pointer type");
    }
    
    UniquePtr(pointer ptr, const Deleter& deleter) noexcept
        : ptr_(ptr), deleter_(deleter) {}
    
    UniquePtr(pointer ptr, std::remove_reference_t<Deleter>&& deleter) noexcept
        : ptr_(ptr), deleter_(std::move(deleter)) {
        static_assert(!std::is_reference_v<Deleter>, "Deleter cannot be an rvalue reference");
    }
    
    // 移动构造函数
    UniquePtr(UniquePtr&& other) noexcept
        : ptr_(other.release()), deleter_(std::forward<Deleter>(other.deleter_)) {}
    
    // 类型转换移动构造函数
    template <typename U, typename E,
              typename = std::enable_if_t<
                  std::is_convertible_v<typename UniquePtr<U, E>::pointer, pointer> &&
                  !std::is_array_v<U> &&
                  ((std::is_reference_v<Deleter> && std::is_same_v<E, Deleter>) ||
                   (!std::is_reference_v<Deleter> && std::is_convertible_v<E, Deleter>))>>
    UniquePtr(UniquePtr<U, E>&& other) noexcept
        : ptr_(other.release()), deleter_(std::forward<E>(other.get_deleter())) {}
    
    // 禁止拷贝
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    // 析构函数
    ~UniquePtr() {
        if (ptr_) {
            deleter_(ptr_);
        }
    }
    
    // 移动赋值操作符
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset(other.release());
            deleter_ = std::forward<Deleter>(other.deleter_);
        }
        return *this;
    }
    
    template <typename U, typename E>
    std::enable_if_t<
        std::is_convertible_v<typename UniquePtr<U, E>::pointer, pointer> &&
        std::is_assignable_v<Deleter&, E&&>,
        UniquePtr&>
    operator=(UniquePtr<U, E>&& other) noexcept {
        reset(other.release());
        deleter_ = std::forward<E>(other.deleter_);
        return *this;
    }
    
    UniquePtr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }
    
    // 访问操作符
    pointer get() const noexcept {
        return ptr_;
    }
    
    deleter_type& get_deleter() noexcept {
        return deleter_;
    }
    
    const deleter_type& get_deleter() const noexcept {
        return deleter_;
    }
    
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
    
    // 解引用操作符（非数组版本）
    template <typename U = T>
    std::enable_if_t<!std::is_array_v<U>, std::add_lvalue_reference_t<T>>
    operator*() const noexcept(noexcept(*std::declval<pointer>())) {
        return *ptr_;
    }
    
    template <typename U = T>
    std::enable_if_t<!std::is_array_v<U>, pointer>
    operator->() const noexcept {
        return ptr_;
    }
    
    // 数组访问操作符（数组版本）
    template <typename U = T>
    std::enable_if_t<std::is_array_v<U>, std::add_lvalue_reference_t<element_type>>
    operator[](std::size_t idx) const {
        return ptr_[idx];
    }
    
    // 修改操作
    pointer release() noexcept {
        pointer result = ptr_;
        ptr_ = nullptr;
        return result;
    }
    
    void reset(pointer ptr = pointer{}) noexcept {
        pointer old_ptr = ptr_;
        ptr_ = ptr;
        if (old_ptr) {
            deleter_(old_ptr);
        }
    }
    
    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }
    
    // 比较操作符
    template <typename U, typename E>
    bool operator==(const UniquePtr<U, E>& other) const noexcept {
        return ptr_ == other.get();
    }
    
    template <typename U, typename E>
    bool operator!=(const UniquePtr<U, E>& other) const noexcept {
        return ptr_ != other.get();
    }
    
    template <typename U, typename E>
    bool operator<(const UniquePtr<U, E>& other) const noexcept {
        return std::less<void*>{}(ptr_, other.get());
    }
    
    template <typename U, typename E>
    bool operator<=(const UniquePtr<U, E>& other) const noexcept {
        return !(other < *this);
    }
    
    template <typename U, typename E>
    bool operator>(const UniquePtr<U, E>& other) const noexcept {
        return other < *this;
    }
    
    template <typename U, typename E>
    bool operator>=(const UniquePtr<U, E>& other) const noexcept {
        return !(*this < other);
    }
    
    // 与 nullptr 的比较
    bool operator==(std::nullptr_t) const noexcept {
        return ptr_ == nullptr;
    }
    
    bool operator!=(std::nullptr_t) const noexcept {
        return ptr_ != nullptr;
    }
    
    bool operator<(std::nullptr_t) const noexcept {
        return std::less<pointer>{}(ptr_, nullptr);
    }
    
    bool operator<=(std::nullptr_t) const noexcept {
        return ptr_ == nullptr;
    }
    
    bool operator>(std::nullptr_t) const noexcept {
        return ptr_ != nullptr;
    }
    
    bool operator>=(std::nullptr_t) const noexcept {
        return true;
    }
};

// 数组特化版本
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    using pointer = T*;
    using element_type = T;
    using deleter_type = Deleter;

private:
    pointer ptr_;
    [[no_unique_address]] Deleter deleter_;

public:
    // 构造函数
    constexpr UniquePtr() noexcept : ptr_(nullptr), deleter_() {}
    constexpr UniquePtr(std::nullptr_t) noexcept : ptr_(nullptr), deleter_() {}
    
    template <typename U>
    explicit UniquePtr(U ptr,
                      std::enable_if_t<std::is_same_v<U, pointer> ||
                                      (std::is_same_v<U, std::nullptr_t>) ||
                                      (std::is_pointer_v<U> && 
                                       std::is_same_v<std::remove_cv_t<std::remove_pointer_t<U>>, element_type>),
                                      std::nullptr_t> = nullptr) noexcept
        : ptr_(ptr), deleter_() {}
    
    template <typename U>
    UniquePtr(U ptr, const Deleter& deleter,
             std::enable_if_t<std::is_same_v<U, pointer> ||
                             std::is_same_v<U, std::nullptr_t>, std::nullptr_t> = nullptr) noexcept
        : ptr_(ptr), deleter_(deleter) {}
    
    template <typename U>
    UniquePtr(U ptr, std::remove_reference_t<Deleter>&& deleter,
             std::enable_if_t<std::is_same_v<U, pointer> ||
                             std::is_same_v<U, std::nullptr_t>, std::nullptr_t> = nullptr) noexcept
        : ptr_(ptr), deleter_(std::move(deleter)) {}
    
    // 移动构造函数
    UniquePtr(UniquePtr&& other) noexcept
        : ptr_(other.release()), deleter_(std::forward<Deleter>(other.deleter_)) {}
    
    template <typename U, typename E,
              typename = std::enable_if_t<
                  std::is_array_v<U> &&
                  std::is_same_v<pointer, typename UniquePtr<U, E>::pointer> &&
                  std::is_convertible_v<E, Deleter>>>
    UniquePtr(UniquePtr<U, E>&& other) noexcept
        : ptr_(other.release()), deleter_(std::forward<E>(other.get_deleter())) {}
    
    // 禁止拷贝
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    // 析构函数
    ~UniquePtr() {
        if (ptr_) {
            deleter_(ptr_);
        }
    }
    
    // 移动赋值操作符
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset(other.release());
            deleter_ = std::forward<Deleter>(other.deleter_);
        }
        return *this;
    }
    
    template <typename U, typename E>
    std::enable_if_t<
        std::is_array_v<U> &&
        std::is_same_v<pointer, typename UniquePtr<U, E>::pointer> &&
        std::is_assignable_v<Deleter&, E&&>,
        UniquePtr&>
    operator=(UniquePtr<U, E>&& other) noexcept {
        reset(other.release());
        deleter_ = std::forward<E>(other.deleter_);
        return *this;
    }
    
    UniquePtr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }
    
    // 访问操作符
    pointer get() const noexcept {
        return ptr_;
    }
    
    deleter_type& get_deleter() noexcept {
        return deleter_;
    }
    
    const deleter_type& get_deleter() const noexcept {
        return deleter_;
    }
    
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
    
    // 数组访问操作符
    T& operator[](std::size_t idx) const {
        return ptr_[idx];
    }
    
    // 修改操作
    pointer release() noexcept {
        pointer result = ptr_;
        ptr_ = nullptr;
        return result;
    }
    
    template <typename U>
    std::enable_if_t<std::is_same_v<U, pointer> || std::is_same_v<U, std::nullptr_t>>
    reset(U ptr = pointer{}) noexcept {
        pointer old_ptr = ptr_;
        ptr_ = ptr;
        if (old_ptr) {
            deleter_(old_ptr);
        }
    }
    
    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }
    
    // 比较操作符（与非数组版本相同）
    template <typename U, typename E>
    bool operator==(const UniquePtr<U, E>& other) const noexcept {
        return ptr_ == other.get();
    }
    
    template <typename U, typename E>
    bool operator!=(const UniquePtr<U, E>& other) const noexcept {
        return ptr_ != other.get();
    }
    
    template <typename U, typename E>
    bool operator<(const UniquePtr<U, E>& other) const noexcept {
        return std::less<void*>{}(ptr_, other.get());
    }
    
    bool operator==(std::nullptr_t) const noexcept {
        return ptr_ == nullptr;
    }
    
    bool operator!=(std::nullptr_t) const noexcept {
        return ptr_ != nullptr;
    }
};

// 工厂函数
template <typename T, typename... Args>
std::enable_if_t<!std::is_array_v<T>, UniquePtr<T>>
make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
std::enable_if_t<std::is_unbounded_array_v<T>, UniquePtr<T>>
make_unique(std::size_t size) {
    return UniquePtr<T>(new std::remove_extent_t<T>[size]());
}

template <typename T, typename... Args>
std::enable_if_t<std::is_bounded_array_v<T>> make_unique(Args&&...) = delete;

// make_unique_for_overwrite (C++20)
template <typename T>
std::enable_if_t<!std::is_array_v<T>, UniquePtr<T>>
make_unique_for_overwrite() {
    return UniquePtr<T>(new T);
}

template <typename T>
std::enable_if_t<std::is_unbounded_array_v<T>, UniquePtr<T>>
make_unique_for_overwrite(std::size_t size) {
    return UniquePtr<T>(new std::remove_extent_t<T>[size]);
}

template <typename T, typename... Args>
std::enable_if_t<std::is_bounded_array_v<T>> make_unique_for_overwrite(Args&&...) = delete;

// 全局交换函数
template <typename T, typename Deleter>
void swap(UniquePtr<T, Deleter>& lhs, UniquePtr<T, Deleter>& rhs) noexcept {
    lhs.swap(rhs);
}

// nullptr 比较的全局操作符
template <typename T, typename Deleter>
bool operator==(std::nullptr_t, const UniquePtr<T, Deleter>& ptr) noexcept {
    return ptr == nullptr;
}

template <typename T, typename Deleter>
bool operator!=(std::nullptr_t, const UniquePtr<T, Deleter>& ptr) noexcept {
    return ptr != nullptr;
}

template <typename T, typename Deleter>
bool operator<(std::nullptr_t, const UniquePtr<T, Deleter>& ptr) noexcept {
    return std::less<typename UniquePtr<T, Deleter>::pointer>{}(nullptr, ptr.get());
}

template <typename T, typename Deleter>
bool operator<=(std::nullptr_t, const UniquePtr<T, Deleter>& ptr) noexcept {
    return nullptr == ptr.get();
}

template <typename T, typename Deleter>
bool operator>(std::nullptr_t, const UniquePtr<T, Deleter>& ptr) noexcept {
    return std::less<typename UniquePtr<T, Deleter>::pointer>{}(ptr.get(), nullptr);
}

template <typename T, typename Deleter>
bool operator>=(std::nullptr_t, const UniquePtr<T, Deleter>& ptr) noexcept {
    return !(nullptr < ptr);
}

} // namespace MyStd
