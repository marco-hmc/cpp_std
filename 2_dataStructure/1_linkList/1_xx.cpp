// 拷贝有随机指针的链表
//
// 题目描述：
// 给你一个长度为 n 的链表，每个节点包含一个额外增加的随机指针 random ，该指针可以指向链表中的任何节点或空节点。
// 要求深度复制该链表并返回复制链表的头节点。

#include <iostream>
#include <unordered_map>

// 带有随机指针的链表节点定义
struct RandomListNode {
    int label;
    RandomListNode* next;
    RandomListNode* random;
    RandomListNode(int x) : label(x), next(nullptr), random(nullptr) {}
};

// 克隆节点：在原链表每个节点后面插入其复制节点
void cloneNodes(RandomListNode* head) {
    while (head != nullptr) {
        RandomListNode* tmp = new RandomListNode(head->label);
        tmp->next = head->next;
        head->next = tmp;
        head = tmp->next;
    }
}

// 处理随机指针：将克隆节点的random指针指向原节点random指针的下一个节点
void reconnectRandomNodes(RandomListNode* head) {
    while (head != nullptr) {
        if (head->random != nullptr)
            head->next->random = head->random->next;
        else
            head->next->random = nullptr;
        head = head->next->next;
    }
}

// 拆分链表：将交错的链表分成原链表和克隆链表
RandomListNode* splitList(RandomListNode* head) {
    RandomListNode* newHead = head->next;
    RandomListNode* cur = newHead->next;
    head->next = cur;
    RandomListNode* newCur = newHead;
    while (cur != nullptr) {
        newCur->next = cur->next;
        newCur = newCur->next;
        cur->next = newCur->next;
        cur = cur->next;
    }
    return newHead;
}

// 复制带随机指针的链表
RandomListNode* copyRandomList(RandomListNode* head) {
    if (head == nullptr) return nullptr;
    cloneNodes(head);
    reconnectRandomNodes(head);
    return splitList(head);
}

// 创建一个测试链表
RandomListNode* createTestList() {
    // 创建节点
    RandomListNode* node1 = new RandomListNode(1);
    RandomListNode* node2 = new RandomListNode(2);
    RandomListNode* node3 = new RandomListNode(3);

    // 连接next指针
    node1->next = node2;
    node2->next = node3;

    // 设置random指针
    node1->random = node3;
    node2->random = node1;
    node3->random = node2;

    return node1;
}

// 打印链表（包括next和random指针）
void printList(RandomListNode* head) {
    std::unordered_map<RandomListNode*, int> nodeMap;

    // 首先建立节点到索引的映射
    RandomListNode* current = head;
    int index = 0;
    while (current) {
        nodeMap[current] = index++;
        current = current->next;
    }

    // 打印链表信息
    current = head;
    index = 0;
    while (current) {
        std::cout << "节点 " << index << " (值=" << current->label << "): ";
        std::cout << "next -> ";
        if (current->next)
            std::cout << nodeMap[current->next];
        else
            std::cout << "nullptr";

        std::cout << ", random -> ";
        if (current->random)
            std::cout << nodeMap[current->random];
        else
            std::cout << "nullptr";

        std::cout << std::endl;

        current = current->next;
        index++;
    }
}

// 释放链表内存
void freeList(RandomListNode* head) {
    RandomListNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// 测试复制链表函数
bool testValid() {
    // 创建测试链表
    RandomListNode* original = createTestList();
    std::cout << "原始链表:" << std::endl;
    printList(original);

    // 复制链表
    RandomListNode* copied = copyRandomList(original);
    std::cout << "\n复制链表:" << std::endl;
    printList(copied);

    // 验证复制是否成功
    // 简单验证：检查原链表和复制链表地址不同但结构相同
    bool isCorrect = true;
    RandomListNode* origCur = original;
    RandomListNode* copyCur = copied;

    while (origCur && copyCur) {
        if (origCur == copyCur) {
            isCorrect = false;  // 地址相同，非深拷贝
            break;
        }
        if (origCur->label != copyCur->label) {
            isCorrect = false;  // 值不同
            break;
        }
        // 检查random指针关系（本例简化处理）
        origCur = origCur->next;
        copyCur = copyCur->next;
    }

    // 释放内存
    freeList(original);
    freeList(copied);

    return isCorrect;
}

int main() {
    if (testValid()) {
        std::cout << "\n所有测试通过!" << std::endl;
    } else {
        std::cout << "\n测试失败!" << std::endl;
    }
    return 0;
}
