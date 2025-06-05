#include <iostream>
#include <vector>

class MaxHeap {
  private:
    std::vector<int> heap;

    void heapifyUp(int index) {
        int parent = (index - 1) / 2;
        while (index > 0 && heap[index] > heap[parent]) {
            std::swap(heap[index], heap[parent]);
            index = parent;
            parent = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int largest = index;

        if (leftChild < heap.size() && heap[leftChild] > heap[largest]) {
            largest = leftChild;
        }

        if (rightChild < heap.size() && heap[rightChild] > heap[largest]) {
            largest = rightChild;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

  public:
    void insert(int value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    int extractMax() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }

        int max = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);

        return max;
    }

    bool isEmpty() { return heap.empty(); }
};

int main() {
    MaxHeap maxHeap;
    maxHeap.insert(5);
    maxHeap.insert(10);
    maxHeap.insert(3);
    maxHeap.insert(8);

    while (!maxHeap.isEmpty()) {
        std::cout << maxHeap.extractMax() << " ";
    }

    return 0;
}