#pragma once

#include <cstddef>
#include <memory>
#include <utility>

namespace MyStd {

    template <class T>
    struct ListNode {
        ListNode* next;
        ListNode* prev;
        T value;

        ListNode(const T& val) : next(nullptr), prev(nullptr), value(val) {}
        ListNode(T&& val)
            : next(nullptr), prev(nullptr), value(std::move(val)) {}
    };

    template <class T>
    class List {
        using Node = ListNode<T>;
        Node dummy;  // dummy node for simplicity
        std::size_t sz;

      public:
        List() : dummy(T{}), sz(0) { dummy.next = dummy.prev = &dummy; }

        ~List() { clear(); }

        void push_back(const T& val) { insert(end(), val); }

        void push_front(const T& val) { insert(begin(), val); }

        void pop_back() {
            if (!empty()) erase(--end());
        }

        void pop_front() {
            if (!empty()) erase(begin());
        }

        bool empty() const { return sz == 0; }
        std::size_t size() const { return sz; }

        void clear() {
            Node* curr = dummy.next;
            while (curr != &dummy) {
                Node* next = curr->next;
                delete curr;
                curr = next;
            }
            dummy.next = dummy.prev = &dummy;
            sz = 0;
        }

        // 基本正向迭代器
        struct iterator {
            Node* ptr;
            iterator(Node* p = nullptr) : ptr(p) {}
            T& operator*() const { return ptr->value; }
            iterator& operator++() {
                ptr = ptr->next;
                return *this;
            }
            bool operator!=(const iterator& rhs) const {
                return ptr != rhs.ptr;
            }
        };

        iterator begin() { return iterator(dummy.next); }
        iterator end() { return iterator(&dummy); }

        iterator insert(iterator pos, const T& val) {
            Node* node = new Node(val);
            Node* next = pos.ptr;
            Node* prev = next->prev;
            node->next = next;
            node->prev = prev;
            prev->next = node;
            next->prev = node;
            ++sz;
            return iterator(node);
        }

        iterator erase(iterator pos) {
            Node* node = pos.ptr;
            if (node == &dummy) return end();
            Node* next = node->next;
            Node* prev = node->prev;
            prev->next = next;
            next->prev = prev;
            delete node;
            --sz;
            return iterator(next);
        }
    };

}  // namespace MyStd
