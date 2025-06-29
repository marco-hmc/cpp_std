## [二叉树的右视图](https://leetcode-cn.com/problems/binary-tree-right-side-view/)

- 深度优先遍历，先遍历右子树，每个深度遍历的第一个节点即是这一深度右视图能看到的节点，可以存储在每个深度访问的第一个结点，一旦我们知道了树的层数，就可以得到最终的结果数组

  ```c++
  // 迭代版本
  class Solution {
  public:
      vector<int> rightSideView(TreeNode* root) {
          unordered_map<int, int> rightmostValueAtDepth;
          int max_depth = -1;

          stack<TreeNode*> nodeStack;
          stack<int> depthStack;
          nodeStack.push(root);
          depthStack.push(0);

          while (!nodeStack.empty()) {
    TreeNode* node = nodeStack.top();
    nodeStack.pop();
    int depth = depthStack.top();
    depthStack.pop();

    if (node != NULL) {
        // 维护二叉树的最大深度
        max_depth = max(max_depth, depth);

        // 如果不存在对应深度的节点我们才插入
        if (rightmostValueAtDepth.find(depth) == rightmostValueAtDepth.end()) {
            rightmostValueAtDepth[depth] = node->val;
        }

        nodeStack.push(node->left);
        nodeStack.push(node->right);
        depthStack.push(depth + 1);
        depthStack.push(depth + 1);
    }
          }

          vector<int> rightView;
          for (int depth = 0; depth <= max_depth; ++depth) {
    rightView.push_back(rightmostValueAtDepth[depth]);
          }

          return rightView;
  }
  }
  ;

  // 递归版本
  class Solution {
    public:
      vector<int> rightSideView(TreeNode* root) {
          if (root == nullptr) {
              return {};
          }
          vector<int> rightView;
          dfs(root, 0, rightView);
          return rightView;
      }
      // 根-右-左
      void dfs(TreeNode* node, int depth, vector<int>& rightView) {
          if (node == nullptr) {
              return;
          }
          // 该深度第一个节点
          if (depth == rightView.size()) {
              rightView.push_back(node->val);
          }
          ++depth;
          dfs(node->right, depth, rightView);
          dfs(node->left, depth, rightView);
      }
  };
  ```

##二叉搜索树与双向链表转换

      - 代码实现

  ```c++ class BSTToSortedList {
    public:
      // 递归版本
      TreeNode* Convert(TreeNode* pRootOfTree) {
          if (pRootOfTree == nullptr) return nullptr;

          Convert(pRootOfTree->left);

          if (first == nullptr) {
              first = pRootOfTree;
          }

          if (last != nullptr) {
              last->right = pRootOfTree;
              pRootOfTree->left = last;
          }
          last = pRootOfTree;
          Convert(pRootOfTree->right);
          return first;
      }
      // 迭代版本
      TreeNode* Convert(TreeNode* pRootOfTree) {
          if (pRootOfTree == nullptr) return nullptr;
          stack<TreeNode*> stk;
          TreeNode *last = nullptr, *first = nullptr, *cur = pRootOfTree;
          while (cur ! = nullptr || !stk.empty()) {
              while (cur != nullptr) {
                  stk.push(cur);
                  cur = cur->left;
              }

              if (last != nullptr) {
                  stk.top()->left = last;
                  last->right = stk.top();
              }

              if (first == nullptr) {
                  first = stk.top();
              }

              last = stk.top();
              cur = stk.top()->right;
              stk.pop();
          }
          return first;
      }

    private:
      TreeNode *first = nullptr, *last = nullptr;
  };
  ```

  ```c++ class sortedListToBST {
    public:
      TreeNode* convert(ListNode* head) { return BST(head, nullptr); }

      TreeNode* BST(ListNode* head, ListNode* tail) {
          if (head == tail) return NULL;
          ListNode* slow = head;
          ListNode* fast = head;
          // 找到中点
          while (fast != tail && fast->next != tail) {
              slow = slow->next;
              fast = fast->next->next;
          }
          TreeNode* root = new TreeNode(slow->val);
          root->left = BST(head, slow);
          root->right = BST(slow->next, tail);

          return root;
      }
  };
  ```

      ##二叉树的下一个结点

      - 题目描述

      -
      给定一个二叉树其中的一个结点，请找出中序遍历顺序的下一个结点并且返回。注意，树中的结点不仅包含左右子结点，同时包含**
              指向父结点的 next 指针**

          -解题思路

          <img src = "imgs/algorithm/solution9.png" alt = "solution9" style =
               "zoom:67%;" />

              结合图，我们可发现分成两大类：

          1. *
          *有右子树的，那么下个结点就是右子树最左边的点**；（eg：D，B，E，A，C，G） 2. *
          *没有右子树的，也可以分成两类* *
          1. 是父节点左孩子（eg：N，I，L） ，那么父节点就是下一个节点 2. 是父节点的右孩子（eg：H，J，K，M）找他的父节点的父节点的父节点...** 直到当前结点是其父节点的左孩子位置，则返回其父节点**。如果没有
              eg：M，那么他就是尾节点

      - 代码实现

  ```c++ TreeLinkNode* GetNext(TreeLinkNode* pNode) {
      if (pNode == nullptr) return nullptr;
      TreeLinkNode* ret = nullptr;
      // 第一类，有右孩子
      if (pNode->right != nullptr) {
          // 找右子树最左边的叶子节点
          ret = pNode->right;
          while (ret->left != nullptr) ret = ret->left;
          return ret;
      }
      // 第二类，没有右孩子，往上找父节点判定
      //
      while (pNode->next != nullptr) {
          TreeLinkNode* root = pNode->next;
          // 当前节点是父节点的左子节点，返回
          if (root->left == pNode) {
              return root;
          }
          // 继续向上找父节点的父节点...
          pNode = pNode->next;
      }
      // 尾节点
      return nullptr;
  }
  ```

## [ 二叉树中所有距离为 K 的结点](https://leetcode-cn.com/problems/all-nodes-distance-k-in-binary-tree/)

- 题目描述

  <img src="imgs/algorithm/problem863.png" alt="problem863" style="zoom: 80%;" />

- 解题思路

  <img src="imgs/algorithm/solution863.png" alt="solution863" style="zoom:80%;" />
  * 记录父节点的过程，相当于记录建图，然后以target节点为起始节点，遍历全图，找到距离target为k的节点
  * 遍历的时候，记录来源节点，防止遍历父节点时，回到子节点，造成结果重复

- 代码实现

  ```c++
  using TreeNodePtr = TreeNode*;
  vector<int> distanceK(TreeNodePtr root, TreeNodePtr target, int k) {
    int no = -1;
    unordered_map<TreeNodePtr, TreeNodePtr> parent;
    vector<int> ret;
    findParent(root, parent);
    findAns(target, nullptr, 0, ret, parent, k);
    return ret;
  }

  void findParent(TreeNodePtr node, unordered_map<TreeNodePtr, TreeNodePtr> &parent) {
    if (node->left != nullptr) {
        parent[node->left] = node;
        findParent(node->left, parent);
    }
    if (node->right != nullptr) {
        parent[node->right] = node;
        findParent(node->right, parent);
    }
  }
  // from记录从哪个节点来，防止回头，因为parent节点的左右节点是没有变化的，在遍历左右节点处，可能会造成重复
  void findAns(TreeNodePtr node, TreeNodePtr from, int no, vector<int> &ret,
               unordered_map<TreeNodePtr, TreeNodePtr> &parent, int k) {
    if (node == nullptr) {
        return;
    }
    if (no == k) {
        ret.push_back(node->val);
        return;
    }
    if (node->left != from) {
        findAns(node->left, node, no + 1, ret, parent, k);
    }
    if (node->right != from) {
        findAns(node->right, node, no + 1, ret, parent, k);
    }
    if (parent[node] != from) {
        findAns(parent[node], node, no + 1, ret, parent, k);
    }
  }

  // 纯dfs递归实现
  class Solution {
  private:
    vector<int> ans;

  public:
    vector<int> distanceK(TreeNode * root, TreeNode * target, int k) {
        dfs(root, target, k);
        return ans;
    }

    int dfs(TreeNode * root, TreeNode * target, int k) {
        if (!root) return -1;
        if (root == target) {
            find(root, k, true, true);  // target为根向下搜索距离为k
            return 0;
        }
        int leftDis = dfs(root->left, target, k);
        int rightDis = dfs(root->right, target, k);
        if (leftDis != -1) {
            // 回溯后target在左子树，向当前根和其右子树搜索，距离为k - leftDis - 1
            find(root, k - leftDis - 1, false, true);
            return leftDis + 1;
        }
        if (rightDis != -1) {
            // 回溯后target在右子树，向当前根和其左子树搜索，距离为k - rightDis - 1
            find(root, k - rightDis - 1, true, false);
            return rightDis + 1;
        }
        return -1;
    }

    void find(TreeNode * root, int dis, bool searchLeft, bool searchRight) {
        if (!root || dis < 0) return;
        if (dis == 0) {
            ans.push_back(root->val);
            return;
        }
        if (searchLeft) find(root->left, dis - 1, true, true);
        if (searchRight) find(root->right, dis - 1, true, true);
    }
  };
  ```

## [不同的二叉搜索树 II](https://leetcode-cn.com/problems/unique-binary-search-trees-ii/)

- 题目描述

  <img src="imgs/algorithm/problem95.png" alt="problem95" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution95.png" alt="solution95" style="zoom:80%;" />

- 解题代码

  ```c++
  vector<TreeNode*> generateTrees(int n) {
    if (n < 1) {
        return {nullptr};
    }
    function<vector<TreeNode*>(int, int)> buildTree =
        [&](int start, int end) -> vector<TreeNode*> {
        if (start > end) {
            return {nullptr};
        }
        vector<TreeNode*> tmpRet;
        for (int i = start; i <= end; ++i) {
            vector<TreeNode*> leftTrees = buildTree(start, i - 1);
            vector<TreeNode*> rightTrees = buildTree(i + 1, end);
            // 从左右子树中，取左右子树与i组合成子树，返回上层
            for (auto leftTree : leftTrees) {
                for (auto rightTree : rightTrees) {
                    TreeNode* root = new TreeNode(i);
                    root->left = leftTree;
                    root->right = rightTree;
                    tmpRet.push_back(root);
                }
            }
        }
        return tmpRet;
    };

    return buildTree(1, n);
  }
  ```

## [恢复二叉搜索树](https://leetcode-cn.com/problems/recover-binary-search-tree/)

- 题目描述

  <img src="imgs/algorithm/problem99.png" alt="problem99" style="zoom:80%;" />

- 解题思路

  <img src="imgs/algorithm/solution99_1.png" alt="solution99_1" style="zoom:80%;" />

  <img src="imgs/algorithm/solution99_2.png" alt="solution99_2" style="zoom:80%;" />

  <img src="imgs/algorithm/solution99_3.png" alt="solution99_3" style="zoom:80%;" />

- 解题代码

  ```c++
  // 空间复杂度O(n)解法
  void recoverTree(TreeNode* root) {
    TreeNode *x = nullptr, *y = nullptr, *prev = nullptr;
    stack<TreeNode*> stk;
    while (!stk.empty() || root != nullptr) {
        while (root != nullptr) {
            stk.push(root);
            root = root->left;
        }
        root = stk.top();
        stk.pop();
        if (prev != nullptr && prev->val > root->val) {
            y = root;
            if (x == nullptr) {
                x = prev;
            }
        }
        prev = root;
        root = root->right;
    }
    swap(x->val, y->val);
  }

  // 空间复杂度O(1)解法：morris中序遍历
  void recoverTree(TreeNode* root) {
    TreeNode *x = nullptr, *y = nullptr, *prev = nullptr;
    TreeNode* cur = root;
    while (cur != nullptr) {
        // 没有左子节点，直接访问
        if (cur->left == nullptr) {
            if (prev != nullptr && prev->val > cur->val) {
                y = cur;
                if (x == nullptr) {
                    x = prev;
                }
            }
            prev = cur;
            cur = cur->right;
        } else {
            TreeNode* rightMost = cur->left;
            while (rightMost->right != nullptr && rightMost->right != cur) {
                rightMost = rightMost->right;
            }
            // 利用叶子节点空子节点
            if (rightMost->right == nullptr) {
                rightMost->right = cur;
                cur = cur->left;
            }
            // rightMost->right = cur，第二次访问到cur节点，左子树已经访问完
            else {
                if (prev != nullptr && prev->val > cur->val) {
                    y = cur;
                    if (x == nullptr) {
                        x = prev;
                    }
                }
                rightMost->right = nullptr;  // 复原树结构
                prev = cur;
                cur = cur->right;
            }
        }
    }
    swap(x->val, y->val);
  }
  ```

## 最近公共祖先

- 解题代码

  ```c++
  TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr) return nullptr;
    // 提前终止搜索，如果都在同一边，返回root
    if (root == p || root == q) return root;

    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    if (left != nullptr && right != nullptr) return root;

    return left == nullptr ? right : left;
  }

  // bst的lca
  TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    TreeNode* ancestor = root;
    while (true) {
        if (p->val < ancestor->val && q->val < ancestor->val) {
            ancestor = ancestor->left;
        } else if (p->val > ancestor->val && q->val > ancestor->val) {
            ancestor = ancestor->right;
        } else {
            break;
        }
    }
    return ancestor;
  }
  ```

## [填充每个节点的下一个右侧节点指针 I&II](https://leetcode-cn.com/problems/populating-next-right-pointers-in-each-node/)

```c++
// I: 完全二叉树
Node* connect(Node* root) {
    if (root == nullptr) {
        return root;
    }
    Node* leftMost = root;
    while (leftMost->left != nullptr) {
        Node* head = leftMost;
        while (head != nullptr) {
            if (head->right != nullptr) {
                head->left->next = head->right;
            }
            if (head->next != nullptr) {
                head->right->next = head->next->left;
            }
            head = head->next;
        }
        leftMost = leftMost->left;
    }
    return root;
}
// I递归解法
void connect(Node*left, Node*right) {
    if (left == nullptr) {
        return;
    }
    left->next = right;
    connect(left->left, left->right);
    if (right != nullptr) {
        connect(right->left, right->right);
        connect(left->right, right->left);
    }
}

// II：非完全二叉树，思路1：层次遍历
// 思路2：很妙
Node* connect(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    Node* cur = root;
    while (cur != nullptr) {
        Node dum;
        Node* prev = &dum;
        Node* node = cur;
        while (node != nullptr) {
            if (node->left != nullptr) {
                prev->next = node->left;
                prev = prev->next;
            }
            if (node->right != nullptr) {
                prev->next = node->right;
                prev = prev->next;
            }
            node = node->next;
        }
        cur = dum.next;
    }
    return root;
}
```
