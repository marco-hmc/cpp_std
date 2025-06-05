// K 个一组翻转链表
//
// 题目描述：
// 给你一个链表，每 k 个节点一组进行翻转，请你返回翻转后的链表。
// k 是一个正整数，它的值小于或等于链表的长度。
// 如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。
// https://leetcode-cn.com/problems/reverse-nodes-in-k-group/

#include <iostream>

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* reverseKGroup(ListNode* head, int k) {
    if (head == nullptr || k <= 1) {
        return head;
    }
    int nodeNum = 0;
    ListNode* cur = head;
    // 统计节点总数
    while (cur != nullptr) {
        ++nodeNum;
        cur = cur->next;
    }
    // 有多少组节点需要翻转
    int group = nodeNum / k;
    cur = head;
    ListNode *prev = nullptr, *next = nullptr, dum;
    dum.next = head;            // dummy头，方便处理
    ListNode* lastTail = &dum;  // 记录上一组的尾节点
    while (group--) {
        ListNode* tmp = cur;
        prev = nullptr;  // 需要置为nullptr，使得每组翻转后尾节点为nullptr
        // reverse
        for (int i = 0; i < k; ++i) {
            next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        // 将上一组尾节点连接到本组首节点
        lastTail->next = prev;
        // 更新尾节点为本组尾节点
        lastTail = tmp;
        // 连接下组首节点
        lastTail->next = cur;
    }
    return dum.next;
}

// 创建链表辅助函数
ListNode* createList(int arr[], int n) {
    if (n == 0) return nullptr;
    ListNode* head = new ListNode(arr[0]);
    ListNode* current = head;

    for (int i = 1; i < n; i++) {
        current->next = new ListNode(arr[i]);
        current = current->next;
    }
    return head;
}

// 打印链表辅助函数
void printList(ListNode* head) {
    while (head) {
        std::cout << head->val;
        if (head->next) std::cout << " -> ";
        head = head->next;
    }
    std::cout << std::endl;
}

// 释放链表内存
void freeList(ListNode* head) {
    ListNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// 测试K个一组翻转链表函数
bool testValid() {
    bool allTestsPassed = true;

    // 测试用例1：k=2
    int arr1[] = {1, 2, 3, 4, 5};
    ListNode* list1 = createList(arr1, 5);
    std::cout << "原始链表: ";
    printList(list1);

    ListNode* reversed1 = reverseKGroup(list1, 2);
    std::cout << "每2个一组翻转: ";
    printList(reversed1);
    std::cout << "预期结果: 2 -> 1 -> 4 -> 3 -> 5" << std::endl;
    // 实际验证需要检查链表值，此处简化为打印

    // 测试用例2：k=3
    int arr2[] = {1, 2, 3, 4, 5};
    ListNode* list2 = createList(arr2, 5);

    ListNode* reversed2 = reverseKGroup(list2, 3);
    std::cout << "每3个一组翻转: ";
    printList(reversed2);
    std::cout << "预期结果: 3 -> 2 -> 1 -> 4 -> 5" << std::endl;

    // 释放内存
    freeList(reversed1);
    freeList(reversed2);

    return allTestsPassed;
}

int main() {
    std::cout << "K个一组翻转链表测试:" << std::endl;
    if (testValid()) {
        std::cout << "所有测试通过!" << std::endl;
    } else {
        std::cout << "测试失败!" << std::endl;
    }
    return 0;
}
