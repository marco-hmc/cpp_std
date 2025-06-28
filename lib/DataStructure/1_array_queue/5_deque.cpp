#include <cassert>
#include <iostream>

template <typename T>
class Deque {
  private:
    T *m_arr;
    int m_capacity;
    int m_front{-1};
    int m_rear{0};
    int m_size{0};

  public:
    Deque(const Deque &) = delete;
    Deque(Deque &&) = delete;
    Deque &operator=(const Deque &) = delete;
    Deque &operator=(Deque &&) = delete;

    explicit Deque(int capacity)
        : m_arr(new T[capacity]), m_capacity(capacity) {}

    ~Deque() { delete[] m_arr; }

    void push_front(T data) {
        if (isFull()) {
            std::cout << "Deque is full. Cannot push front." << '\n';
            return;
        }

        if (m_front == -1) {
            m_front = 0;
            m_rear = 0;
        } else if (m_front == 0) {
            m_front = m_capacity - 1;
        } else {
            m_front--;
        }

        m_arr[m_front] = data;
        m_size++;
    }

    void push_back(T data) {
        if (isFull()) {
            std::cout << "Deque is full. Cannot push back." << '\n';
            return;
        }

        if (m_front == -1) {
            m_front = 0;
            m_rear = 0;
        } else if (m_rear == m_capacity - 1) {
            m_rear = 0;
        } else {
            m_rear++;
        }

        m_arr[m_rear] = data;
        m_size++;
    }

    void pop_front() {
        if (isEmpty()) {
            std::cout << "Deque is empty. Cannot pop front." << '\n';
            return;
        }

        if (m_front == m_rear) {
            m_front = -1;
            m_rear = -1;
        } else if (m_front == m_capacity - 1) {
            m_front = 0;
        } else {
            m_front++;
        }

        m_size--;
    }

    void pop_back() {
        if (isEmpty()) {
            std::cout << "Deque is empty. Cannot pop back." << '\n';
            return;
        }

        if (m_front == m_rear) {
            m_front = -1;
            m_rear = -1;
        } else if (m_rear == 0) {
            m_rear = m_capacity - 1;
        } else {
            m_rear--;
        }

        m_size--;
    }

    T getFront() {
        if (isEmpty()) {
            std::cout << "Deque is empty. Cannot get front." << '\n';
            return T();
        }

        return m_arr[m_front];
    }

    T getBack() {
        if (isEmpty()) {
            std::cout << "Deque is empty. Cannot get back." << '\n';
            return T();
        }

        return m_arr[m_rear];
    }

    bool isEmpty() { return m_size == 0; }

    bool isFull() { return m_size == m_capacity; }
};

int main() {
    Deque<int> deque(5);

    deque.push_front(1);
    deque.push_front(2);
    deque.push_back(3);
    deque.push_back(4);

    assert(deque.getFront() == 2);
    assert(deque.getBack() == 4);

    deque.pop_front();
    deque.pop_back();

    assert(deque.getFront() == 1);
    assert(deque.getBack() == 3);

    std::cout << "All assertions passed!" << '\n';

    return 0;
}