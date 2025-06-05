#include <iostream>

class BTreeNode {
    int *keys;
    int t;
    BTreeNode **children;
    int numKeys;
    bool leaf;

  public:
    BTreeNode(int t, bool leaf);

    void insertNonFull(int k);
    void splitChild(int i, BTreeNode *y);
    void traverse();

    friend class BTree;
};

class BTree {
    BTreeNode *root;
    int t;

  public:
    BTree(int t);

    void insert(int k);
    void traverse();
};

BTreeNode::BTreeNode(int t, bool leaf)
    : keys(new int[2 * t - 1]),
      t(t),
      children(new BTreeNode *[2 * t]),
      numKeys(0),
      leaf(leaf) {}

void BTreeNode::insertNonFull(int k) {
    int i = numKeys - 1;

    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        numKeys++;
    } else {
        while (i >= 0 && keys[i] > k) {
            i--;
        }

        if (children[i + 1]->numKeys == 2 * t - 1) {
            splitChild(i + 1, children[i + 1]);

            if (keys[i + 1] < k) {
                i++;
            }
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
    auto *z = new BTreeNode(y->t, y->leaf);
    z->numKeys = t - 1;

    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
    }

    if (!y->leaf) {
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }

    y->numKeys = t - 1;

    for (int j = numKeys; j >= i + 1; j--) {
        children[j + 1] = children[j];
    }

    children[i + 1] = z;

    for (int j = numKeys - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
    }

    keys[i] = y->keys[t - 1];
    numKeys++;
}

void BTreeNode::traverse() {
    int i = 0;
    for (i = 0; i < numKeys; i++) {
        if (!leaf) {
            children[i]->traverse();
        }
        std::cout << " " << keys[i];
    }

    if (!leaf) {
        children[i]->traverse();
    }
}

BTree::BTree(int t) : root(nullptr), t(t) {}

void BTree::insert(int k) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->numKeys = 1;
    } else {
        if (root->numKeys == 2 * t - 1) {
            auto *s = new BTreeNode(t, false);
            s->children[0] = root;
            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k) {
                i++;
            }
            s->children[i]->insertNonFull(k);

            root = s;
        } else {
            root->insertNonFull(k);
        }
    }
}

void BTree::traverse() {
    if (root != nullptr) {
        root->traverse();
    }
}

void test() {
    BTree bTree(3);

  bTree.insert(10);
  bTree.insert(20);
  bTree.insert(5);
  bTree.insert(6);
  bTree.insert(12);
  bTree.insert(30);
  bTree.insert(7);
  bTree.insert(17);

  std::cout << "B树遍历结果:";
  bTree.traverse();
}

int main() {
    test();
    return 0;
}