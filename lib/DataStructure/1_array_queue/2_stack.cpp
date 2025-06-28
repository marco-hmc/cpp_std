算法有家问了个记录栈的最大值没答上来，有家问的两个栈组成一个队列

#include <iostream>
#include <vector>

template <typename T>
class Stack {
  private:
    std::vector<T> data;

  public:
    void push(const T &value) { data.push_back(value); }

    void pop() {
        if (!data.empty()) {
            data.pop_back();
        }
    }

    T &top() { return data.back(); }
    bool empty() const { return data.empty(); }
    int size() const { return data.size(); }
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

    return 0;
}