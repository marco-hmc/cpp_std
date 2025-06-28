#include <iostream>

// 红黑树是一种特殊的二叉查找树,它在每一个节点上都使用红色或黑色进行标记,通过一些性质确保它是始终平衡的.
// 它的性质是这样的:
// 1. 每个节点不是红色就是黑色.
// 2. 根节点是黑色的.
// 3. 叶节点的空节点是黑色的.
// 4. 如果一个节点是红色的,那么它的两个子节点是黑色的.
// 5. 对于任意节点,从它到叶节点的每条路径上都有相同数目的黑色节点.

// 红黑树的插入,查询,删除在一般情况和最坏情况下的时间复杂度都是O(log(n))
// 应用场景主要是STL中map,set的实现,优点在于支持频繁的修改,因为查询删除插入时间复杂度都是logN

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color{};
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

    explicit Node(int data) : data(data) {}
};

class RedBlackTree {
  private:
    Node *root{};

    void rotateLeft(Node *node) {
        Node *rightChild = node->right;
        node->right = rightChild->left;

        if (rightChild->left != nullptr) {
            rightChild->left->parent = node;
        }

        rightChild->parent = node->parent;

        if (node->parent == nullptr) {
            root = rightChild;
        } else if (node == node->parent->left) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node *node) {
        Node *leftChild = node->left;
        node->left = leftChild->right;

        if (leftChild->right != nullptr) {
            leftChild->right->parent = node;
        }

        leftChild->parent = node->parent;

        if (node->parent == nullptr) {
            root = leftChild;
        } else if (node == node->parent->left) {
            node->parent->left = leftChild;
        } else {
            node->parent->right = leftChild;
        }

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixInsert(Node *node) {
        while (node != root && node->parent->color == RED) {
            if (node->parent == node->parent->parent->left) {
                Node *uncle = node->parent->parent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            } else {
                Node *uncle = node->parent->parent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }

        root->color = BLACK;
    }

  public:
    RedBlackTree() = default;

    void insert(int data) {
        Node *newNode = new Node(data);
        Node *current = root;
        Node *parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (parent == nullptr) {
            root = newNode;
        } else if (data < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        fixInsert(newNode);
    }

    void inorderTraversal(Node *node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            std::cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    void printInorderTraversal() {
        inorderTraversal(root);
        std::cout << '\n';
    }
};

void test() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);

    std::cout << "Inorder Traversal: ";
    tree.printInorderTraversal();
}

int main() {
    test();
    return 0;
}