#include <cstdlib>
#include <cstring>
#include <iostream>

const int MAX_LEVEL = 6;

class Node {
  public:
    int value;
    Node **forward;

    Node(const Node &) = default;
    Node(Node &&) = delete;
    Node &operator=(const Node &) = default;
    Node &operator=(Node &&) = delete;

    Node(int level, int value) : value(value), forward(new Node *[level + 1]) {
        memset(forward, 0, sizeof(Node *) * (level + 1));
    }

    ~Node() { delete[] forward; }
};

class SkipList {
  private:
    Node *header;
    int level{0};

  public:
    SkipList() : header(new Node(MAX_LEVEL, 0)) {}

    SkipList(const SkipList &) = default;
    SkipList(SkipList &&) = delete;
    SkipList &operator=(const SkipList &) = default;
    SkipList &operator=(SkipList &&) = delete;
    ~SkipList() { delete header; }

    static int randomLevel() {
        int level = 0;
        while (rand() < RAND_MAX / 2 && level < MAX_LEVEL) {
            level++;
        }
        return level;
    }

    void insert(int value) {
        Node *current = header;
        Node *update[MAX_LEVEL + 1];
        memset(update, 0, sizeof(Node *) * (MAX_LEVEL + 1));

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->value != value) {
            int newLevel = randomLevel();

            if (newLevel > level) {
                for (int i = level + 1; i <= newLevel; i++) {
                    update[i] = header;
                }
                level = newLevel;
            }

            Node *newNode = new Node(newLevel, value);

            for (int i = 0; i <= newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    bool search(int value) {
        Node *current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];
        return current != nullptr && current->value == value;
    }

    void print() {
        std::cout << "Skip List:" << '\n';
        for (int i = 0; i <= level; i++) {
            Node *current = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (current != nullptr) {
                std::cout << current->value << " ";
                current = current->forward[i];
            }
            std::cout << '\n';
        }
    }
};

int main() {
    SkipList skipList;

    skipList.insert(3);
    skipList.insert(6);
    skipList.insert(2);
    skipList.insert(9);
    skipList.insert(1);

    skipList.print();

    int value = 6;
    if (skipList.search(value)) {
        std::cout << "Value " << value << " found in the skip list." << '\n';
    } else {
        std::cout << "Value " << value << " not found in the skip list."
                  << '\n';
    }

    return 0;
}
