#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace MyStd {

    template <class T>
    class Vector {
      public:
        using value_type = T;
        using size_type = std::size_t;
        using iterator = T*;

        Vector() : data_(nullptr), size_(0), cap_(0) {}

        Vector(size_type n) : data_(new T[n]()), size_(n), cap_(n) {}

        Vector(std::initializer_list<T> ilist)
            : data_(new T[ilist.size()]),
              size_(ilist.size()),
              cap_(ilist.size()) {
            size_type i = 0;
            for (const auto& val : ilist) data_[i++] = val;
        }

        Vector(const Vector& other)
            : data_(new T[other.size_]), size_(other.size_), cap_(other.size_) {
            for (size_type i = 0; i < size_; ++i) data_[i] = other.data_[i];
        }

        Vector& operator=(const Vector& other) {
            if (this == &other) return *this;
            delete[] data_;
            size_ = cap_ = other.size_;
            data_ = new T[size_];
            for (size_type i = 0; i < size_; ++i) data_[i] = other.data_[i];
            return *this;
        }

        ~Vector() { delete[] data_; }

        void push_back(const T& val) {
            if (size_ == cap_) reserve(cap_ == 0 ? 1 : cap_ * 2);
            data_[size_++] = val;
        }

        void pop_back() {
            if (size_) --size_;
        }

        void clear() { size_ = 0; }

        void resize(size_type n) {
            if (n > cap_) reserve(n);
            if (n > size_)
                for (size_type i = size_; i < n; ++i) data_[i] = T();
            size_ = n;
        }

        void reserve(size_type n) {
            if (n <= cap_) return;
            T* new_data = new T[n];
            for (size_type i = 0; i < size_; ++i) new_data[i] = data_[i];
            delete[] data_;
            data_ = new_data;
            cap_ = n;
        }

        size_type size() const { return size_; }
        size_type capacity() const { return cap_; }
        bool empty() const { return size_ == 0; }

        T& operator[](size_type i) { return data_[i]; }
        const T& operator[](size_type i) const { return data_[i]; }

        T& at(size_type i) {
            if (i >= size_) throw std::out_of_range("vector::at");
            return data_[i];
        }

        iterator begin() { return data_; }
        iterator end() { return data_ + size_; }

      private:
        T* data_;
        size_type size_;
        size_type cap_;
    };

}  // namespace MyStd
