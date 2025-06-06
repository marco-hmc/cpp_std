####怎么求两个节点的最近公共祖先
    ?
#二叉树

    ##Morris 遍历

            -
            由于在遍历的时候，我们需要记住某种遍历次序的的后驱或者前驱节点，常见的递归和非递归都是采用栈的方式完成这个过程，需要
            O(n) 的空间复杂度
            -
            有没有内部空间来记录这些后驱或者前驱节点，使额外空间复杂度为 O(1)
                呢？有，那就是叶节点的左，右孩子节点，因为叶节点的两个孩子节点都是空指针，如果利用好这些空间，我们就可以在
            O(1) 的空间完成遍历
            -
            利用叶节点的左、右孩子指向遍历的前驱或者后驱结点，这些指针叫做线索，对应的二叉树叫做线索二叉树。

            ## #Morris 先序遍历

            -
            Morris 前序遍历的流程如下：

            1. 当前结点的左孩子是否为空，若** 是则输出当前结点**，** 并更新当前结点为当前结点的右孩子**；否则进入
            2

            2. ** 在当前结点的左子树中寻找中序遍历下的前驱结点**（** 左子树中最右结点**）

            a.** 若前驱结点的右孩子为空，则将前驱结点的右孩子指向当前结点，输出当前结点**（在这里输出，和中序遍历不同的地方），当前结点更新为当前结点的左孩子，进入
            3

            b.** 若前驱结点的右孩子为当前结点**（不为空），将前驱结点的右孩子置
            NULL，当前结点更新为当前结点的右孩子，进入 3

            3. 若当前结点不为空，进入 1；否则程序结束

        < img src = "imgs/algorithm/morris_pre.png" alt = "morris_pre" style =
        "zoom:80%;" / >

        -代码实现

  ```c++ ![morris_in](imgs / algorithm /
                       morris_in.png) void morrisPre(TreeNode * root) {
            TreeNode* cur = root;
            TreeNode* rightMost = nullptr;
            vector<int> vec;
            while (cur != nullptr) {
                // cur的左子节点为空，到达最左边，直接输出，并开始遍历其右子树
                if (cur->left == nullptr) {
                    vec.push_back(cur->val);
                    cur = cur->right;
                } else {  // 找到cur节点在中序遍历中的前驱节点，也就是左子树最右边的节点
                    rightMost = cur->left;
                    while (rightMost->right != nullptr &&
                           rightMost->right != cur) {
                        rightMost = rightMost->right;
                    }
                    // 前驱节点的右子节点为空，表示cur节点被首次遍历，输出cur
                    if (rightMost->right == nullptr) {
                        vec.push_back(cur->val);
                        rightMost->right = cur;
                        cur = cur->left;
                    } else {
                        // 前驱节点的右子节点不为空，cur节点已经被遍历，
                        // 还原前驱节点为nullptr，然后去遍历cur的右子树
                        rightMost->right = nullptr;
                        cur = cur->right;
                    }
                }
            }
        }
  ```

            ## #Morris 中序遍历

            -
            Morris 中序遍历过程如下：

            1. 当前结点的左孩子是否为空，若是则输出当前结点，更当前结点为当前结点的右孩子；否则进入
            2

            2. 在当前结点的左子树中寻找中序遍历下的前驱结点（左子树中最右结点）

            a.若前驱结点的右孩子为空，则将前驱结点的右孩子指向当前结点，当前结点更新为当前结点的左孩子；进入
            3

            b.若前驱结点的右孩子为当前结点（不为空），将前驱结点的右孩子置
            NULL，输出当前结点，当前结点更新为当前结点的右孩子，进入 3

            3. 若当前结点不为空，进入 1；否则程序结束

        < img src = "imgs/algorithm/morris_in.png" alt = "morris_in" style =
            "zoom:80%;" / >

            -代码实现

  ```c++ void morrisIn(TreeNode* root, vector<vector<int> >& ret) {
    TreeNode* cur = root;
    TreeNode* rightMost = nullptr;
    vector<int> vec;
    while (cur != nullptr) {
        if (cur->left == nullptr) {
            vec.push_back(cur->val);
            cur = cur->right;
        } else {
            rightMost = cur->left;
            while (rightMost->right != nullptr && rightMost->right != cur) {
                rightMost = rightMost->right;
            }
            if (rightMost->right == nullptr) {
                rightMost->right = cur;
                cur = cur->left;
            } else {
                // 中序遍历与先序遍历不同点，在返回时输出cur节点
                vec.push_back(cur->val);
                rightMost->right = nullptr;
                cur = cur->right;
            }
        }
    }
}
```

    ## #Morris 后序遍历

    - 后序遍历规则：

    1. 第二次访问到某一节点时，逆序打印其左子树的右边界。最后单独逆序打印整棵树的右边界

    <img src = "imgs/algorithm/morris_post.png" alt = "morris_post" style =
         "zoom:80%;" />

    - 代码实现

  ```c++ void morrisPost(TreeNode* root, vector<vector<int> >& ret) {
    TreeNode* cur = root;
    TreeNode* rightMost = nullptr;
    vector<int> vec;
    while (cur != nullptr) {
        if (cur->left == nullptr) {
            cur = cur->right;
        } else {
            rightMost = cur->left;
            while (rightMost->right != nullptr && rightMost->right != cur) {
                rightMost = rightMost->right;
            }
            // 第一次访问cur节点
            if (rightMost->right == nullptr) {
                rightMost->right = cur;
                cur = cur->left;
            } else {
                // 第二次访问cur节点，逆序打印cur的左子树的右边界
                rightMost->right = nullptr;
                reverseOutput(cur->left, vec);
                cur = cur->right;
            }
        }
    }
    // 最后逆序打印整棵树的右边界
    reverseOutput(root, vec);
}

void reverseOutput(TreeNode* node, vector<int>& vec) {
    stack<int> stk;
    while (node != nullptr) {
        stk.push(node->val);
        node = node->right;
    }
    while (!stk.empty()) {
        vec.push_back(stk.top());
        stk.pop();
    }
}
  ```

## [路径总和 III](https://leetcode-cn.com/problems/path-sum-iii/)

- 代码实现：利用树的前缀和，巧妙

  ```c++
  int pathSum(TreeNode * root, int targetSum) {
    if (root == nullptr) return 0;
    unordered_map<int, int> map;
    // 关键，前缀和-target=0，节点本身为一条路径
    map[0] = 1;
    return calSum(root, map, 0, targetSum);
  }

  int calSum(TreeNode *root, unordered_map<int, int> &map, int curSum, int target) {
    if (root == nullptr) return 0;

    curSum += root->val;
    int ret = 0;
    // 统计前面前缀和为curSum - target有多少条路径
    if (map.count(curSum - target)) ret = map[curSum - target];
    ++map[curSum];  // 当前节点为末端的前缀和
    // 左右子树前缀和
    int left = calSum(root->left, map, curSum, target);
    int right = calSum(root->right, map, curSum, target);
    // 关键！统计完，返回时需要消除当前节点前缀和，否则会重复。因为题目要求是从上往下
    --map[curSum];
    return ret + left + right;
  }
  ```

## [二叉树中的最大路径和](https://leetcode-cn.com/problems/binary-tree-maximum-path-sum/)

- 代码实现

  ```c++
  int maxPathSum(TreeNode* root) {
    if (root == nullptr) return 0;
    int maxVal = INT_MIN;
    calMax(root, maxVal);
    return maxVal;
  }

  int calMax(TreeNode * node, int& maxVal) {
    if (node == nullptr) {
        return 0;
    }
    // 返回左右子树最大路径
    int leftMax = calMax(node->left, maxVal);
    int rightMax = calMax(node->right, maxVal);
    // 记录当前节点最大，可能左+根，右+根，左+右+根，根
    int curMax = node->val + max(leftMax, 0) + max(rightMax, 0);
    // 维护全局最大
    maxVal = max(maxVal, curMax);
    // 返回当前层最大路径，可能左+根，右+根，根。因为路径定义，不存在左+右+根
    return max(node->val, max(leftMax, rightMax) + node->val);
  }
  ```

## [检查平衡性](https://leetcode-cn.com/problems/check-balance-lcci/)(自底向上递归树)

- 代码实现

  ```c++
  bool isBalanced(TreeNode * root) {
    return height(root) >= 0;
  }

  // 小于0表示不平衡
  int height(TreeNode * node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    if (leftHeight < 0 || rightHeight < 0 ||
        abs(leftHeight - rightHeight) > 1) {
        return -1;  // 向上报告不平衡，退出递归
    }
    // 平衡
    return max(leftHeight, rightHeight) + 1;
  }
  ```

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

      -
      广度优先遍历，可以对二叉树进行层次遍历，那么对于每层来说，最右边的结点一定是最后被遍历到的。二叉树的层次遍历可以用广度优先搜索实现

  ```c++ class Solution {
    public:
      vector<int> rightSideView(TreeNode* root) {
          unordered_map<int, int> rightmostValueAtDepth;
          int max_depth = -1;

          queue<TreeNode*> nodeQueue;
          queue<int> depthQueue;
          nodeQueue.push(root);
          depthQueue.push(0);

          while (!nodeQueue.empty()) {
              TreeNode* node = nodeQueue.front();
              nodeQueue.pop();
              int depth = depthQueue.front();
              depthQueue.pop();

              if (node != NULL) {
                  // 维护二叉树的最大深度
                  max_depth = max(max_depth, depth);

                  // 由于每一层最后一个访问到的节点才是我们要的答案，因此不断更新对应深度的信息即可
                  rightmostValueAtDepth[depth] = node->val;

                  nodeQueue.push(node->left);
                  nodeQueue.push(node->right);
                  depthQueue.push(depth + 1);
                  depthQueue.push(depth + 1);
              }
          }

          vector<int> rightView;
          for (int depth = 0; depth <= max_depth; ++depth) {
              rightView.push_back(rightmostValueAtDepth[depth]);
          }

          return rightView;
      }
  };
  ```

      ##判断一棵二叉树是否为二叉搜索树和完全二叉树

      - 代码实现

  ```c++ vector<bool> judgeIt(TreeNode* root) {
      vector<bool> ret(2, false);
      if (isBST(root, INT_MIN, INT_MAX)) ret[0] = true;
      if (isCompleted(root)) ret[1] = true;
      return ret;
  }

  bool isBST(TreeNode* node, int minVal, int maxVal) {
      if (node == nullptr) return true;
      // 当前节点值违反上层最大最小约束，则不平衡
      if (node->val >= maxVal || node->val <= minVal) return false;
      // 左子树所有节点都小于当前根节点(node)，所以最大约束为node->val
      // 同理，右子树所有节点都大于当前根节点(node)，所以最小约束为node->val
      return isBST(node->left, minVal, node->val) &&
             isBST(node->right, node->val, maxVal);
  }

  bool isCompleted(TreeNode* node) {
      if (node == nullptr) return true;
      // 左子树非空，右子树为空，需要递归判断左子树
      if (node->left != nullptr && node->right == nullptr)
          return isCompleted(node->left);
      // 左子树为空，右子树非空，必非完全二叉树
      if (node->left == nullptr && node->right != nullptr) return false;
      // 左右非空，递归判断
      return isCompleted(node->left) && isCompleted(node->right);
  }

  // 完全二叉树其他判断思路：
  // 	  1. 层次遍历，遇到空节点后，后面出现非空节点则为不完全树
  //    2. dfs或者bfs遍历的同时，给节点编号，完全树i节点的子节点为(2*i + 1)和(2*i + 2)，
  //       最后判断最后一个编号是否等于节点总数可判断

  // #1
  class Solution {
    public:
      bool isCompleteTree(TreeNode* root) {
          queue<TreeNode*> q;
          q.push(root);
          bool flag = false;
          while (!q.empty()) {
              auto node = q.front();
              q.pop();
              if (node == nullptr) {
                  flag = true;
                  continue;
              }
              // 非空节点且之前出现空节点
              if (flag) return false;
              q.push(node->left);
              q.push(node->right);
          }
          return true;
      }
  };

  // #2
  class Solution {
      int size = 0;
      int maxCode = 0;
    public
      boolean isCompleteTree(TreeNode root) {
          if (root == null) {
              return true;
          }
          recursive(root, 1);
          return size == maxCode;
      }

    public
      void recursive(TreeNode root, int index) {
          if (root == null) {
              return;
          }
          ++size;
          maxCode = Math.max(maxCode, index);
          recursive(root.left, index * 2);
          recursive(root.right, index * 2 + 1);
      }
  }
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
