#include <iostream>
#include <stdexcept>

template <typename T>
class Stack {
  private:
    T* data;
    size_t capacity;
    size_t stackSize;

    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < stackSize; ++i) {
            newData[i] = data[i];
        }

        delete[] data;

        data = newData;
        capacity = newCapacity;
    }

  public:
    Stack() : capacity(10), stackSize(0) { data = new T[capacity]; }

    Stack(const Stack& other)
        : capacity(other.capacity), stackSize(other.stackSize) {
        data = new T[capacity];
        for (size_t i = 0; i < stackSize; ++i) {
            data[i] = other.data[i];
        }
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            stackSize = other.stackSize;
            data = new T[capacity];
            for (size_t i = 0; i < stackSize; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~Stack() { delete[] data; }

    void push(const T& value) {
        if (stackSize >= capacity) {
            resize(capacity * 2);
        }
        data[stackSize++] = value;
    }

    void pop() {
        if (empty()) {
            return;
        }
        --stackSize;

        if (stackSize > 0 && stackSize < capacity / 4) {
            resize(capacity / 2);
        }
    }

    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data[stackSize - 1];
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data[stackSize - 1];
    }

    bool empty() const { return stackSize == 0; }

    size_t size() const { return stackSize; }
};

int main() {
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);

    std::cout << "Stack size: " << stack.size() << '\n';
    std::cout << "Top element: " << stack.top() << '\n';

    stack.pop();
    std::cout << "Stack size after pop: " << stack.size() << '\n';
    std::cout << "Top element after pop: " << stack.top() << '\n';

    Stack<int> stack2 = stack;
    std::cout << "Copied stack size: " << stack2.size() << '\n';
    std::cout << "Copied stack top: " << stack2.top() << '\n';

    return 0;
}