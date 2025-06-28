#include <algorithm>
#include <memory>
#include <typeinfo>

namespace Impl1 {

    class Any {
      public:
        template <typename T>
        Any(T value)
            : ptr_(std::unique_ptr<void, void (*)(void *)>(
                  new T(std::move(value)),
                  [](void *ptr) { static_cast<T *>(ptr)->~T(); })),
              type_(&typeid(T)) {}

        ~Any() = default;

        template <typename T>
        T &get() {
            if (typeid(T) != *type_) {
                throw std::bad_cast();
            }
            return *static_cast<T *>(ptr_.get());
        }

      public:
        Any(const Any &) = delete;
        Any &operator=(const Any &) = delete;
        Any(Any &&other) noexcept = default;
        Any &operator=(Any &&other) noexcept = default;

      private:
        std::unique_ptr<void, void (*)(void *)> ptr_;
        const std::type_info *type_;
    };
}  // namespace Impl1

namespace Impl2 {

    class Any {
      public:
        Any() = default;

        template <typename T>
        Any(T value) : ptr_(std::make_unique<Model<T>>(std::move(value))) {}

        template <typename T>
        T &get() {
            if (typeid(T) != ptr_->type()) {
                throw std::bad_cast();
            }
            return dynamic_cast<Model<T> &>(*ptr_).data_;
        }

        const std::type_info &type() const {
            return ptr_ ? ptr_->type() : typeid(void);
        }

      private:
        struct Concept {
            virtual ~Concept() = default;
            virtual const std::type_info &type() const = 0;
        };

        template <typename T>
        struct Model : Concept {
            Model(T data) : data_(std::move(data)) {}
            const std::type_info &type() const override { return typeid(T); }
            T data_;
        };

        std::unique_ptr<Concept> ptr_;
    };

}  // namespace Impl2