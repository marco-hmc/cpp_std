#pragma once

#include <memory>
#include <typeinfo>

/*
1. std::any的实现需要支持多种类型的存储和访问，因此需要使用模板来实现。但取回来的时候，需要明确知道类型。
*/

namespace MyStd {

    namespace AnyImpl1 {
        class Any {
          public:
            template <typename T>
            Any(T value) : ptr_(new T(std::move(value))), type_(&typeid(T)) {}

            ~Any() { delete ptr_; }

            template <typename T>
            T& get() {
                if (typeid(T) != *type_) {
                    throw std::bad_cast();
                }
                return *static_cast<T*>(ptr_);
            }

            Any(Any&& other) noexcept : ptr_(other.ptr_), type_(other.type_) {
                other.ptr_ = nullptr;
                other.type_ = nullptr;
            }

            Any& operator=(Any&& other) noexcept {
                if (this != &other) {
                    delete ptr_;
                    ptr_ = other.ptr_;
                    type_ = other.type_;
                    other.ptr_ = nullptr;
                    other.type_ = nullptr;
                }
                return *this;
            }

          private:
            void* ptr_;
            const std::type_info* type_;
        };
    }  // namespace AnyImpl1

    namespace AnyImpl2 {
        class Any {
          public:
            Any() = default;

            template <typename T>
            Any(T value) : ptr_(std::make_unique<Model<T>>(std::move(value))) {}

            Any(const Any& other)
                : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}

            Any& operator=(const Any& other) {
                if (this != &other) {
                    ptr_ = other.ptr_ ? other.ptr_->clone() : nullptr;
                }
                return *this;
            }

            Any(Any&& other) noexcept = default;
            Any& operator=(Any&& other) noexcept = default;

            template <typename T>
            T& get() {
                if (typeid(T) != ptr_->type()) {
                    throw std::bad_cast();
                }
                return dynamic_cast<Model<T>&>(*ptr_).data_;
            }

            const std::type_info& type() const {
                return ptr_ ? ptr_->type() : typeid(void);
            }

          private:
            struct Concept {
                virtual ~Concept() = default;
                virtual const std::type_info& type() const = 0;
                virtual std::unique_ptr<Concept> clone() const = 0;
            };

            template <typename T>
            struct Model : Concept {
                Model(T data) : data_(std::move(data)) {}
                const std::type_info& type() const override {
                    return typeid(T);
                }
                std::unique_ptr<Concept> clone() const override {
                    return std::make_unique<Model<T>>(data_);
                }
                T data_;
            };

            std::unique_ptr<Concept> ptr_;
        };
    }  // namespace AnyImpl2

}  // namespace MyStd
