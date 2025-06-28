#include <iostream>
#include <vector>

class MyCircularQueue {
  public:
    MyCircularQueue(int k) : size(k), head(-1), tail(-1) { queue.reserve(k); }

    bool enQueue(int value) {
        if (isEmpty()) {
            head++;
            tail++;
        } else if (isFull())
            return false;
        tail = (tail + 1) % size;
        queue[tail] = value;
        return true;
    }

    bool deQueue() {
        if (isEmpty()) return false;
        if (head == tail) {
            head = -1;
            tail = -1;
        } else {
            head = (head + 1) % size;
        }
        return true;
    }

    int Front() {
        if (isEmpty()) return -1;
        return queue[head];
    }

    int Rear() {
        if (isEmpty()) return -1;
        return queue[tail];
    }

    bool isEmpty() { return (head == -1 && tail == -1); }

    bool isFull() { return ((tail + 1) % size == head); }

  private:
    std::vector<int> queue;
    int size;
    int head;
    int tail;
};

int main() {
    MyCircularQueue cq(5);

    cq.enQueue(1);
    cq.enQueue(2);
    cq.enQueue(3);
    cq.enQueue(4);
    cq.enQueue(5);

    bool enqueued = cq.enQueue(6);
    if (!enqueued) {
        std::cout << "Circular queue is full. Cannot enqueue." << std::endl;
    }

    cq.deQueue();
    cq.deQueue();

    int front = cq.Front();
    int rear = cq.Rear();

    std::cout << "Front element: " << front << std::endl;
    std::cout << "Rear element: " << rear << std::endl;

    return 0;
}