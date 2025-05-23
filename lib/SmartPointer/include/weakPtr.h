#pragma once

#include <memory>

namespace MyStd {
    namespace WeakPtrImpl1 {
        template <typename T>
        class weakPtr {
          private:
            T *ptr;

          public:
            weakPtr() : ptr(nullptr) {}
            weakPtr(T *p) : ptr(p) {}

            bool expired() const { return ptr == nullptr; }

            std::shared_ptr<T> lock() const {
                if (expired()) {
                    return nullptr;
                }
                return std::shared_ptr<T>(ptr);
            }
        };

    }  // namespace WeakPtrImpl1

    namespace WeakPtrImpl2 {}
}  // namespace MyStd