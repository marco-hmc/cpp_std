#include <iostream>

class Queue {
  private:
    static const int MAX_SIZE = 100;
    int arr[MAX_SIZE]{};
    int front{-1};
    int rear{-1};

  public:
    Queue() {}

    bool isEmpty() const { return front == -1 && rear == -1; }
    bool isFull() const { return rear == MAX_SIZE - 1; }

    void enqueue(int value) {
        if (isFull()) {
            std::cout << "Queue is full. Cannot enqueue." << '\n';
            return;
        }

        if (isEmpty()) {
            front = 0;
            rear = 0;
        } else {
            rear++;
        }

        arr[rear] = value;
    }

    void dequeue() {
        if (isEmpty()) {
            std::cout << "Queue is empty. Cannot dequeue." << '\n';
            return;
        }

        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front++;
        }
    }

    int getFront() {
        if (isEmpty()) {
            std::cout << "Queue is empty." << '\n';
            return -1;
        }

        return arr[front];
    }

    int getRear() {
        if (isEmpty()) {
            std::cout << "Queue is empty." << '\n';
            return -1;
        }

        return arr[rear];
    }
};

int main() {
    Queue queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    std::cout << "Front: " << queue.getFront() << '\n';
    std::cout << "Rear: " << queue.getRear() << '\n';

    queue.dequeue();
    std::cout << "Front: " << queue.getFront() << '\n';
    std::cout << "Rear: " << queue.getRear() << '\n';

    return 0;
}