#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    explicit ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

std::vector<int> listToVector(ListNode *head) {
    std::vector<int> ret;
    ListNode *cur = head;
    while (cur) {
        ret.push_back(cur->val);
        cur = cur->next;
    }
    return ret;
}

ListNode *vecToNode(const std::vector<int> &vec) {
    auto *dummyHead = new ListNode(0);
    ListNode *cur = dummyHead;
    for (int i : vec) {
        auto *tmp = new ListNode(i);
        cur->next = tmp;
        cur = cur->next;
    }
    return dummyHead->next;
}

namespace ReverseList {
    ListNode *ReverseList(ListNode *head) {
        ListNode *cur = head;
        ListNode *pre = nullptr;
        while (cur != nullptr) {
            ListNode *nxt = cur->next;
            cur->next = pre;
            pre = cur;
            cur = nxt;
        }
        return pre;
    }

    void task() {
        auto *node = vecToNode({1, 2, 3, 4, 5});
        std::vector<int> result = listToVector(ReverseList(node));
        std::vector<int> expect = {5, 4, 3, 2, 1};
        const bool isSame = result == expect;
        assert(isSame);
    }
}  // namespace ReverseList

namespace MergeTwoLists {
    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2) {
        auto *dummyHead = new ListNode(0);
        ListNode *cur = dummyHead;
        while (list1 || list2) {
            int val1 = list1 ? list1->val : INT_MAX;
            int val2 = list2 ? list2->val : INT_MAX;
            int val = std::min(val1, val2);
            if (val == val1) {
                list1 = list1->next;
            } else {
                list2 = list2->next;
            }
            auto *tmp = new ListNode(val);
            cur->next = tmp;
            cur = cur->next;
        }
        return dummyHead->next;
    }
}  // namespace MergeTwoLists

namespace HasCycle {
    bool hasCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) {
            return false;
        }
        ListNode *slow = head;
        ListNode *fast = head->next;
        while (slow != fast) {
            if (fast == nullptr || fast->next == nullptr) {
                return false;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        return true;
    }
}  // namespace HasCycle

namespace removeElements {
    ListNode *removeElements(ListNode *head, int val) {
        auto *preHead = new ListNode(0);
        ListNode *pre = preHead;
        pre->next = head;
        ListNode *cur = head;
        while (cur != nullptr) {
            if (cur->val == val) {
                ListNode *next = cur->next;
                pre->next = next;
                cur = cur->next;
            } else {
                cur = cur->next;
                pre = pre->next;
            }
        }
        return preHead->next;
    }

    ListNode *removeElements_wrong(ListNode *head, int val) {
        auto *pre = new ListNode(0);
        pre->next = head;
        ListNode *cur = head;
        while (cur != nullptr) {
            if (cur->val == val) {
                ListNode *next = cur->next;
                pre->next = next;
                cur = cur->next;
            } else {
                cur = cur->next;
                pre = pre->next;
            }
        }
        return head;
    }

    void test() {
        // Create a linked list
        auto *head = new ListNode(1);
        auto *node1 = new ListNode(2);
        auto *node2 = new ListNode(3);
        auto *node3 = new ListNode(4);
        auto *node4 = new ListNode(5);
        head->next = node1;
        node1->next = node2;
        node2->next = node3;
        node3->next = node4;

        // Test method1
        ListNode *result1 = removeElements(head, 3);
        std::cout << "Method1 Result: ";
        while (result1 != nullptr) {
            std::cout << result1->val << " ";
            result1 = result1->next;
        }
        std::cout << '\n';

        ListNode *result2 = removeElements_wrong(head, 3);
        std::cout << "Method2 Result: ";
        while (result2 != nullptr) {
            std::cout << result2->val << " ";
            result2 = result2->next;
        }
        std::cout << '\n';
    }
}  // namespace removeElements

int main() {
    ReverseList::task();
    return 0;
}
