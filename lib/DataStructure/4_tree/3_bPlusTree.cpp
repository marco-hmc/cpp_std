#include <algorithm>
#include <iostream>
#include <vector>

class BPlusTreeNode {
  public:
    bool isLeaf;
    std::vector<int> keys;
    std::vector<BPlusTreeNode *> children;

    explicit BPlusTreeNode(bool leaf = false) : isLeaf(leaf) {}
};

class BPlusTree {
  public:
    BPlusTreeNode *root{nullptr};
    BPlusTree() = default;

    void insert(int key) {
        if (root == nullptr) {
            root = new BPlusTreeNode(true);
            root->keys.push_back(key);
        } else {
            BPlusTreeNode *node = root;
            while (!node->isLeaf) {
                int i = 0;
                while (i < node->keys.size() && key > node->keys[i]) {
                    i++;
                }
                node = node->children[i];
            }
            node->keys.push_back(key);
            std::sort(node->keys.begin(), node->keys.end());
        }
    }

    void traverse() { traverseNode(root); }

  private:
    void traverseNode(BPlusTreeNode *node) {
        if (node != nullptr) {
            for (int i = 0; i < node->keys.size(); i++) {
                std::cout << node->keys[i] << " ";
            }
            std::cout << '\n';

            if (!node->isLeaf) {
                for (int i = 0; i < node->children.size(); i++) {
                    traverseNode(node->children[i]);
                }
            }
        }
    }
};

int main() {
    BPlusTree bPlusTree;

    bPlusTree.insert(10);
    bPlusTree.insert(20);
    bPlusTree.insert(5);
    bPlusTree.insert(15);
    bPlusTree.insert(25);

    bPlusTree.traverse();

    return 0;
}