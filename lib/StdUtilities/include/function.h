#pragma once

#include <functional>
#include <memory>

namespace MyStd {

    namespace FunctionImpl1 {
        template <typename>
        class Function;

        template <typename R, typename... Args>
        class Function<R(Args...)> {
          public:
            Function() = default;

            template <typename T>
            Function(T func)
                : ptr_(std::make_unique<Model<T>>(std::move(func))) {}

            R operator()(Args... args) const {
                if (!ptr_) {
                    throw std::bad_function_call();
                }
                return ptr_->invoke(std::forward<Args>(args)...);
            }

            explicit operator bool() const noexcept { return ptr_ != nullptr; }

          private:
            struct Concept {
                virtual ~Concept() = default;
                virtual R invoke(Args... args) const = 0;
            };

            template <typename T>
            struct Model : Concept {
                Model(T func) : func_(std::move(func)) {}
                R invoke(Args... args) const override {
                    return func_(std::forward<Args>(args)...);
                }
                T func_;
            };

            std::unique_ptr<Concept> ptr_;
        };
    }  // namespace FunctionImpl1

    namespace FunctionImpl2 {
        template <typename>
        struct Function {
            static_assert(false, "not a valid function signature");
        };

        template <typename Ret, typename... Args>
        struct Function<Ret(Args...)> {
          private:
            struct FuncBase {
                virtual Ret M_call(Args... __args) = 0;
                virtual std::unique_ptr<FuncBase> M_clone() const = 0;
                virtual std::type_info const &M_type() const = 0;
                virtual ~FuncBase() = default;
            };

            template <class Fn>
            struct FuncImpl : FuncBase {
                Fn M_f;

                template <class... CArgs>
                explicit FuncImpl(std::in_place_t, CArgs &&..._args)
                    : M_f(std::forward<CArgs>(_args)...) {}

                Ret M_call(Args... _args) override {
                    return std::invoke(M_f, std::forward<Args>(_args)...);
                }

                std::unique_ptr<FuncBase> M_clone() const override {
                    return std::make_unique<FuncImpl>(std::in_place, M_f);
                }

                std::type_info const &M_type() const override {
                    return typeid(Fn);
                }
            };

            std::unique_ptr<FuncBase> _M_base;

          public:
            Function() = default;
            Function(std::nullptr_t) noexcept : Function() {}

            template <
                class Fn,
                class = std::enable_if_t<
                    std::is_invocable_r_v<Ret, std::decay_t<Fn>, Args...> &&
                    std::is_copy_constructible_v<Fn> &&
                    !std::is_same_v<std::decay_t<Fn>, Function<Ret(Args...)>>>>
            Function(Fn &&__f)
                : _M_base(std::make_unique<FuncImpl<std::decay_t<Fn>>>(
                      std::in_place, std::forward<Fn>(__f))) {}

            Function(Function &&) = default;
            Function &operator=(Function &&) = default;

            Function(Function const &that)
                : _M_base(that._M_base ? that._M_base->M_clone() : nullptr) {}

            Function &operator=(Function const &that) {
                if (that._M_base)
                    _M_base = that._M_base->M_clone();
                else
                    _M_base = nullptr;
            }

            explicit operator bool() const noexcept {
                return _M_base != nullptr;
            }

            bool operator==(std::nullptr_t) const noexcept {
                return _M_base == nullptr;
            }

            bool operator!=(std::nullptr_t) const noexcept {
                return _M_base != nullptr;
            }

            Ret operator()(Args... _args) const {
                if (!_M_base) [[unlikely]]
                    throw std::bad_function_call();

                return _M_base->M_call(std::forward<Args>(_args)...);
            }

            std::type_info const &target_type() const noexcept {
                return _M_base ? _M_base->M_type() : typeid(void);
            }

            template <class Fn>
            Fn *target() const noexcept {
                return _M_base && typeid(Fn) == _M_base->M_type()
                           ? std::addressof(
                                 static_cast<FuncImpl<Fn> *>(_M_base.get())
                                     ->M_f)
                           : nullptr;
            }

            void swap(Function &that) const noexcept {
                _M_base.swap(that._M_base);
            }
        };

    }  // namespace FunctionImpl2
}  // namespace MyStd
