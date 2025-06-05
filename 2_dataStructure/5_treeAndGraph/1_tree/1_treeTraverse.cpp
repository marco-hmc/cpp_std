#include <iostream>
#include <queue>
#include <stack>

struct TreeNode {
    int val;
    TreeNode *left{};
    TreeNode *right{};
    explicit TreeNode(int x) : val(x) {}
};

TreeNode *getTree() {
    auto *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);
    return root;
}

namespace PreorderTraversal {
    void preorderTraversal(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        std::cout << root->val << " ";
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }

    std::vector<int> preorderTraversal2(TreeNode *root) {
        std::vector<int> result;
        std::stack<TreeNode *> stk;
        if (root != nullptr) stk.push(root);

        while (!stk.empty()) {
            TreeNode *node = stk.top();
            stk.pop();

            if (node != nullptr) {
                result.push_back(node->val);
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
            }
        }

        return result;
    }

    void task() {
        std::cout << "Preorder traversal: ";
        TreeNode *root = getTree();
        preorderTraversal(root);
        std::cout << std::endl;

        std::cout << "Preorder traversal2: ";
        std::vector<int> result = preorderTraversal2(root);
        for (int i : result) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

}  // namespace PreorderTraversal

namespace InorderTraversal {
    void inorderTraversal(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        inorderTraversal(root->left);
        std::cout << root->val << " ";
        inorderTraversal(root->right);
    }

    std::vector<int> inorderTraversal2(TreeNode *root) {
        std::vector<int> result;
        std::stack<TreeNode *> stk;
        TreeNode *curr = root;

        while (!curr || !stk.empty()) {
            while (!curr) {
                stk.push(curr);
                curr = curr->left;
            }

            curr = stk.top();
            stk.pop();
            result.push_back(curr->val);
            curr = curr->right;
        }

        return result;
    }

    void task() {
        std::cout << "Inorder traversal: ";
        TreeNode *root = getTree();
        inorderTraversal(root);
        std::cout << std::endl;

        std::cout << "Inorder traversal2: ";
        std::vector<int> result = inorderTraversal2(root);
        for (int i : result) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}  // namespace InorderTraversal

namespace Postorder {
    void postorderTraversal(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        std::cout << root->val << " ";
    }

    std::vector<int> postorderTraversal2(TreeNode *root) {
        std::vector<int> result;
        std::stack<TreeNode *> stk;
        if (root != nullptr) stk.push(root);

        while (!stk.empty()) {
            TreeNode *node = stk.top();
            stk.pop();

            if (node != nullptr) {
                result.insert(result.begin(), node->val);
                if (node->left) stk.push(node->left);
                if (node->right) stk.push(node->right);
            }
        }
        return result;
    }

    void task() {
        std::cout << "Postorder traversal: ";
        TreeNode *root = getTree();
        postorderTraversal(root);
        std::cout << std::endl;

        std::cout << "Postorder traversal2: ";
        std::vector<int> result = postorderTraversal2(root);
        for (int i : result) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}  // namespace Postorder

namespace levelOrderTraversal {

    void levelOrderTraversal(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        std::queue<TreeNode *> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *node = q.front();
            q.pop();
            std::cout << node->val << " ";
            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
    }
    void task() {
        std::cout << "Level order traversal: ";
        TreeNode *root = getTree();
        levelOrderTraversal(root);
        std::cout << std::endl;
    }

}  // namespace levelOrderTraversal

int main() {
    PreorderTraversal::task();
    InorderTraversal::task();
    Postorder::task();
    levelOrderTraversal::task();

    return 0;
}