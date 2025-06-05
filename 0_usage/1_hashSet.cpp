#include <iostream>
#include <vector>

class MyHashSet {
  private:
    struct Node {
        int val;
        Node *next;
        Node(int val) : val(val), next(nullptr) {}
    };

  public:
    MyHashSet() { bucket = std::vector<Node *>(keyRange); }

    void add(int key) {
        int index = hash(key);
        if (bucket[index] == nullptr) {
            bucket[index] = new Node(key);
            return;
        }

        auto *temp = bucket[index];
        if (temp->val == key) {
            return;
        }

        while (temp->next) {
            if (temp->next->val == key) {
                return;
            }
            temp = temp->next;
        }
        temp->next = new Node(key);
    }

    void remove(int key) {
        int index = hash(key);
        if (bucket[index] == nullptr) {
            return;
        }
        auto *temp = bucket[index];
        if (temp->val == key) {
            bucket[index] = temp->next;
            delete temp;
        } else {
            auto *next = temp->next;
            while (next) {
                if (next->val == key) {
                    temp->next = next->next;
                    delete next;
                    break;
                }
                temp = next;
                next = next->next;
            }
        }
    }

    /** Returns true if this set contains the specified element */
    bool contains(int key) {
        bool result = false;
        int index = hash(key);
        auto *node = bucket[index];
        while (node) {
            if (node->val == key) {
                result = true;
                break;
            }
            node = node->next;
        }
        return result;
    }

  private:
    int hash(int key) const { return (key % keyRange); }
    int keyRange = 31;
    std::vector<Node *> bucket;
};

int main() {
    MyHashSet mySet;

    mySet.add(5);
    mySet.add(10);
    mySet.add(15);

    bool containsElement = mySet.contains(10);
    if (containsElement) {
        std::cout << "The set contains the element 10." << '\n';
    } else {
        std::cout << "The set does not contain the element 10." << '\n';
    }

    mySet.remove(10);

    containsElement = mySet.contains(10);
    if (containsElement) {
        std::cout << "The set still contains the element 10." << '\n';
    } else {
        std::cout << "The set does not contain the element 10 anymore." << '\n';
    }

    return 0;
}