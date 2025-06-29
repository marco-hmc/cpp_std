#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

class Node {
  public:
    int value;
    Node **forward;
    Node(int level, int value) {
        this->value = value;
        forward = new Node *[level + 1];
        memset(forward, 0, sizeof(Node *) * (level + 1));
    }
    ~Node() { delete[] forward; }
};

class SkipList {
    int MAXLVL;
    float P;
    int level;
    Node *header;

  public:
    SkipList(int MAXLVL, float P) {
        this->MAXLVL = MAXLVL;
        this->P = P;
        level = 0;
        header = new Node(MAXLVL, -1);
    }

    int randomLevel() {
        float r = (float)rand() / RAND_MAX;
        int lvl = 0;
        while (r < P && lvl < MAXLVL) {
            lvl++;
            r = (float)rand() / RAND_MAX;
        }
        return lvl;
    }

    Node *createNode(int value, int level) {
        Node *n = new Node(level, value);
        return n;
    }

    void insertElement(int value) {
        Node *current = header;
        Node *update[MAXLVL + 1];
        memset(update, 0, sizeof(Node *) * (MAXLVL + 1));

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                   current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == NULL || current->value != value) {
            int rlevel = randomLevel();
            if (rlevel > level) {
                for (int i = level + 1; i < rlevel + 1; i++) {
                    update[i] = header;
                }
                level = rlevel;
            }

            Node *n = createNode(value, rlevel);

            for (int i = 0; i <= rlevel; i++) {
                n->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = n;
            }
            std::cout << "Successfully inserted key " << value << "\n";
        }
    }

    void deleteElement(int value) {
        Node *current = header;
        Node *update[MAXLVL + 1];
        memset(update, 0, sizeof(Node *) * (MAXLVL + 1));

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                   current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != NULL && current->value == value) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }

            while (level > 0 && header->forward[level] == 0) {
                level--;
            }

            std::cout << "Successfully deleted key " << value << "\n";
        }
    }

    void searchElement(int value) {
        Node *current = header;

        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < value) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        if (current && current->value == value) {
            std::cout << "Found key: " << value << "\n";
        } else {
            std::cout << "Key not found: " << value << "\n";
        }
    }

    void displayList() {
        std::cout << "\n*****Skip List*****"
                  << "\n";
        for (int i = 0; i <= level; i++) {
            Node *node = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (node != NULL) {
                std::cout << node->value << " ";
                node = node->forward[i];
            }
            std::cout << "\n";
        }
    }
};

int main() {
    srand((unsigned)time(0));

    SkipList lst(3, 0.5);

    lst.insertElement(3);
    lst.insertElement(6);
    lst.insertElement(7);
    lst.insertElement(9);
    lst.insertElement(12);
    lst.insertElement(19);
    lst.insertElement(17);
    lst.insertElement(26);
    lst.insertElement(21);
    lst.insertElement(25);

    lst.displayList();

    lst.searchElement(19);
    lst.deleteElement(19);
    lst.searchElement(19);

    lst.displayList();

    return 0;
}

/*
100个节点
min(logx100) x=e// 2.7
*/