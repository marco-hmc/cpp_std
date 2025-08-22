#pragma once

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace MyStd {

    // 引用计数控制块基类
    struct RefCountBase {
        std::atomic<std::int64_t> ref_count_;
        std::atomic<std::int64_t> weak_count_;

        RefCountBase() noexcept : ref_count_(1), weak_count_(1) {}

        RefCountBase(const RefCountBase&) = delete;
        RefCountBase& operator=(const RefCountBase&) = delete;

        void add_ref() noexcept {
            ref_count_.fetch_add(1, std::memory_order_relaxed);
        }

        void release() noexcept {
            if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                destroy_object();
                weak_release();
            }
        }

        void add_weak_ref() noexcept {
            weak_count_.fetch_add(1, std::memory_order_relaxed);
        }

        void weak_release() noexcept {
            if (weak_count_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete this;
            }
        }

        std::int64_t use_count() const noexcept {
            return ref_count_.load(std::memory_order_relaxed);
        }

        bool expired() const noexcept {
            return ref_count_.load(std::memory_order_relaxed) == 0;
        }

        virtual void destroy_object() noexcept = 0;
        virtual ~RefCountBase() = default;
    };

    // 带删除器的控制块实现
    template <typename T, typename Deleter>
    class RefCountDeleter final : public RefCountBase {
      private:
        T* ptr_;
        [[no_unique_address]] Deleter deleter_;

      public:
        explicit RefCountDeleter(T* ptr, Deleter deleter) noexcept
            : ptr_(ptr), deleter_(std::move(deleter)) {}

        void destroy_object() noexcept override {
            deleter_(ptr_);
            ptr_ = nullptr;
        }
    };

// make_shared 优化的控制块（对象和控制块在同一内存块中）
template <typename T>
class RefCountInPlace final : public RefCountBase {
  private:
    alignas(T) char storage_[sizeof(T)];

  public:
    template <typename... Args>
    explicit RefCountInPlace(Args&&... args) {
        new (storage_) T(std::forward<Args>(args)...);
    }

    T* get_object() noexcept { return reinterpret_cast<T*>(storage_); }

    void destroy_object() noexcept override { get_object()->~T(); }
};

// 前向声明
template <typename T>
class SharedPtr;
template <typename T>
class WeakPtr;

// EnableSharedFromThis 基类
template <typename T>
class EnableSharedFromThis {
  private:
    mutable WeakPtr<T> weak_this_;

    template <typename U>
    friend class SharedPtr;

  public:
    constexpr EnableSharedFromThis() noexcept = default;
    constexpr EnableSharedFromThis(const EnableSharedFromThis&) noexcept =
        default;
    EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept =
        default;

    SharedPtr<T> shared_from_this() { return SharedPtr<T>(weak_this_); }

    SharedPtr<const T> shared_from_this() const {
        return SharedPtr<const T>(weak_this_);
    }

    WeakPtr<T> weak_from_this() noexcept { return weak_this_; }

    WeakPtr<const T> weak_from_this() const noexcept { return weak_this_; }

  protected:
    ~EnableSharedFromThis() = default;
};

// SharedPtr 主实现
template <typename T>
class SharedPtr {
  public:
    using element_type = std::remove_extent_t<T>;
    using weak_type = WeakPtr<T>;

  private:
    element_type* ptr_;
    RefCountBase* control_block_;

    template <typename U>
    friend class SharedPtr;
    template <typename U>
    friend class WeakPtr;

    // 内部构造函数
    SharedPtr(element_type* ptr, RefCountBase* control) noexcept
        : ptr_(ptr), control_block_(control) {}

public:
    // 默认构造函数
    constexpr SharedPtr() noexcept : ptr_(nullptr), control_block_(nullptr) {}
    constexpr SharedPtr(std::nullptr_t) noexcept
        : ptr_(nullptr), control_block_(nullptr) {}

    // 从原始指针构造
    template <typename Y,
              typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
    explicit SharedPtr(Y* ptr) : ptr_(ptr) {
        try {
            control_block_ = new RefCountDeleter<Y, std::default_delete<Y>>(
                ptr, std::default_delete<Y>{});
            enable_shared_from_this(ptr);
        } catch (...) {
            delete ptr;
            throw;
        }
  }

  // 带自定义删除器构造
  template <typename Y, typename Deleter,
            typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
  SharedPtr(Y* ptr, Deleter deleter) : ptr_(ptr) {
      try {
          control_block_ =
              new RefCountDeleter<Y, Deleter>(ptr, std::move(deleter));
          enable_shared_from_this(ptr);
      } catch (...) {
          deleter(ptr);
          throw;
      }
  }

    // 拷贝构造函数
    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        if (control_block_) {
            control_block_->add_ref();
        }
    }

    // 移动构造函数
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        other.ptr_ = nullptr;
        other.control_block_ = nullptr;
    }

    // 类型转换构造函数
    template <typename Y,
              typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
    SharedPtr(const SharedPtr<Y>& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        if (control_block_) {
            control_block_->add_ref();
        }
    }

    template <typename Y,
              typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
    SharedPtr(SharedPtr<Y>&& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        other.ptr_ = nullptr;
        other.control_block_ = nullptr;
    }

    // 别名构造函数
    template <typename Y>
    SharedPtr(const SharedPtr<Y>& other, element_type* ptr) noexcept
        : ptr_(ptr), control_block_(other.control_block_) {
        if (control_block_) {
            control_block_->add_ref();
        }
    }

    // 从 WeakPtr 构造
    template <typename Y,
              typename = std::enable_if_t<std::is_convertible_v<Y*, T*>>>
    explicit SharedPtr(const WeakPtr<Y>& weak)
        : ptr_(nullptr), control_block_(nullptr) {
        if (!weak.expired()) {
            ptr_ = weak.ptr_;
            control_block_ = weak.control_block_;
            if (control_block_) {
                control_block_->add_ref();
            }
        } else {
            throw std::bad_weak_ptr{};
        }
    }

    // 析构函数
    ~SharedPtr() {
        if (control_block_) {
            control_block_->release();
        }
    }

    // 赋值操作符
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        SharedPtr(other).swap(*this);
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        SharedPtr(std::move(other)).swap(*this);
        return *this;
    }

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& other) noexcept {
        SharedPtr(other).swap(*this);
        return *this;
    }

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& other) noexcept {
        SharedPtr(std::move(other)).swap(*this);
        return *this;
    }

    // 重置函数
    void reset() noexcept { SharedPtr().swap(*this); }

    template <typename Y>
    void reset(Y* ptr) {
        SharedPtr(ptr).swap(*this);
    }

    template <typename Y, typename Deleter>
    void reset(Y* ptr, Deleter deleter) {
        SharedPtr(ptr, std::move(deleter)).swap(*this);
    }

    // 访问操作符
    element_type* get() const noexcept { return ptr_; }

    element_type& operator*() const noexcept { return *ptr_; }

    element_type* operator->() const noexcept { return ptr_; }

    // 数组访问（当T是数组类型时）
    template <typename U = T>
    std::enable_if_t<std::is_array_v<U>, element_type&> operator[](
        std::ptrdiff_t idx) const {
        return ptr_[idx];
    }

    // 引用计数操作
    long use_count() const noexcept {
        return control_block_ ? control_block_->use_count() : 0;
    }

    bool unique() const noexcept { return use_count() == 1; }

    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    // 交换
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(control_block_, other.control_block_);
    }

    // 比较操作符
    template <typename Y>
    bool operator==(const SharedPtr<Y>& other) const noexcept {
        return ptr_ == other.ptr_;
    }

    template <typename Y>
    bool operator!=(const SharedPtr<Y>& other) const noexcept {
        return ptr_ != other.ptr_;
    }

    template <typename Y>
    bool operator<(const SharedPtr<Y>& other) const noexcept {
        return std::less<void*>{}(ptr_, other.ptr_);
    }

    // 所有权比较
    template <typename Y>
    bool owner_before(const SharedPtr<Y>& other) const noexcept {
        return control_block_ < other.control_block_;
    }

    template <typename Y>
    bool owner_before(const WeakPtr<Y>& other) const noexcept {
        return control_block_ < other.control_block_;
    }

  private:
    // 为 EnableSharedFromThis 设置 weak_ptr
    template <typename Y>
    void enable_shared_from_this(Y* ptr) {
        if constexpr (std::is_base_of_v<EnableSharedFromThis<Y>, Y>) {
            if (ptr) {
                ptr->weak_this_ = SharedPtr<Y>(*this, ptr);
            }
        }
    }
};

// WeakPtr 实现
template <typename T>
class WeakPtr {
  public:
    using element_type = std::remove_extent_t<T>;

  private:
    element_type* ptr_;
    RefCountBase* control_block_;

    template <typename U>
    friend class SharedPtr;
    template <typename U>
    friend class WeakPtr;

  public:
    constexpr WeakPtr() noexcept : ptr_(nullptr), control_block_(nullptr) {}

    WeakPtr(const WeakPtr& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        if (control_block_) {
            control_block_->add_weak_ref();
        }
    }

    template <typename Y>
    WeakPtr(const SharedPtr<Y>& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        if (control_block_) {
            control_block_->add_weak_ref();
        }
    }

    WeakPtr(WeakPtr&& other) noexcept
        : ptr_(other.ptr_), control_block_(other.control_block_) {
        other.ptr_ = nullptr;
        other.control_block_ = nullptr;
    }

    ~WeakPtr() {
        if (control_block_) {
            control_block_->weak_release();
        }
    }

    WeakPtr& operator=(const WeakPtr& other) noexcept {
        WeakPtr(other).swap(*this);
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) noexcept {
        WeakPtr(std::move(other)).swap(*this);
        return *this;
    }

    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other) noexcept {
        WeakPtr(other).swap(*this);
        return *this;
    }

    void reset() noexcept { WeakPtr().swap(*this); }

    void swap(WeakPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(control_block_, other.control_block_);
    }

    long use_count() const noexcept {
        return control_block_ ? control_block_->use_count() : 0;
    }

    bool expired() const noexcept { return use_count() == 0; }

    SharedPtr<T> lock() const noexcept {
        if (expired()) {
            return SharedPtr<T>{};
        }
        return SharedPtr<T>(*this);
    }

    template <typename Y>
    bool owner_before(const WeakPtr<Y>& other) const noexcept {
        return control_block_ < other.control_block_;
    }

    template <typename Y>
    bool owner_before(const SharedPtr<Y>& other) const noexcept {
        return control_block_ < other.control_block_;
    }
};

// 工厂函数
template <typename T, typename... Args>
std::enable_if_t<!std::is_array_v<T>, SharedPtr<T>> make_shared(
    Args&&... args) {
    auto control = new RefCountInPlace<T>(std::forward<Args>(args)...);
    try {
        SharedPtr<T> result(control->get_object(), control);
        // 为 EnableSharedFromThis 设置 weak_ptr
        if constexpr (std::is_base_of_v<EnableSharedFromThis<T>, T>) {
            control->get_object()->weak_this_ = result;
        }
        return result;
    } catch (...) {
        delete control;
        throw;
    }
}

// 数组版本的 make_shared
template <typename T>
std::enable_if_t<std::is_unbounded_array_v<T>, SharedPtr<T>> make_shared(
    std::size_t size) {
    using element_type = std::remove_extent_t<T>;
    return SharedPtr<T>(new element_type[size], std::default_delete<T>{});
}

// 指针转换函数
template <typename T, typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U>& other) noexcept {
    auto ptr = static_cast<typename SharedPtr<T>::element_type*>(other.get());
    return SharedPtr<T>(other, ptr);
}

template <typename T, typename U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U>& other) noexcept {
    auto ptr = dynamic_cast<typename SharedPtr<T>::element_type*>(other.get());
    if (ptr) {
        return SharedPtr<T>(other, ptr);
    }
    return SharedPtr<T>{};
}

template <typename T, typename U>
SharedPtr<T> const_pointer_cast(const SharedPtr<U>& other) noexcept {
    auto ptr = const_cast<typename SharedPtr<T>::element_type*>(other.get());
    return SharedPtr<T>(other, ptr);
}

template <typename T, typename U>
SharedPtr<T> reinterpret_pointer_cast(const SharedPtr<U>& other) noexcept {
    auto ptr =
        reinterpret_cast<typename SharedPtr<T>::element_type*>(other.get());
    return SharedPtr<T>(other, ptr);
}

// 全局交换函数
template <typename T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

}  // namespace MyStd