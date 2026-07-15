// Topic     : 嵌套类
// Doc       : 第2部分-阶段4 · 步骤 8.4
// cppreference: https://en.cppreference.com/cpp/language/nested_types
//
// 要点: 嵌套类是外层作用域内的类型；可访问外层 private（作为成员语义）；用于实现细节/迭代器。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <utility>

namespace {

class LinkedList {
    struct Node {  // 私有嵌套：实现细节
        int value;
        Node* next;
        explicit Node(int v, Node* n = nullptr) : value(v), next(n) {}
    };

    Node* head_ = nullptr;

public:
    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    ~LinkedList() {
        while (head_) {
            Node* n = head_->next;
            delete head_;
            head_ = n;
        }
    }

    void push_front(int v) { head_ = new Node(v, head_); }

    class Iterator {
        Node* cur_ = nullptr;

    public:
        explicit Iterator(Node* n) : cur_(n) {}
        int operator*() const { return cur_->value; }
        Iterator& operator++() {
            cur_ = cur_->next;
            return *this;
        }
        bool operator==(const Iterator& o) const { return cur_ == o.cur_; }
    };

    Iterator begin() { return Iterator{head_}; }
    Iterator end() { return Iterator{nullptr}; }

    int front() const {
        assert(head_);
        return head_->value;
    }
};

// 公开嵌套类型可在外部命名
struct Outer {
    struct Inner {
        int x = 0;
    };
    Inner make() const { return Inner{1}; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [nested_class] ===\n";

    // -------------------------------------------------------------------------
    // §入门：公开嵌套类型名
    // -------------------------------------------------------------------------
    Outer::Inner in{5};
    assert(in.x == 5);
    Outer o;
    assert(o.make().x == 1);
    std::cout << "[intro] Outer::Inner x=" << in.x << '\n';

    // -------------------------------------------------------------------------
    // §进阶：链表 + 迭代器嵌套类
    // -------------------------------------------------------------------------
    LinkedList list;
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);
    assert(list.front() == 1);

    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    assert(sum == 6);
    std::cout << "[advanced] list sum=" << sum << '\n';

    // -------------------------------------------------------------------------
    // §专家：访问与封装
    // -------------------------------------------------------------------------
    // Node 私有：外部不能写 LinkedList::Node
    // Iterator 公有：外部可用 LinkedList::Iterator
    LinkedList::Iterator it = list.begin();
    assert(*it == 1);

    std::cout << "[expert] nested Iterator first=" << *it << '\n';
    std::cout << "=== nested_class: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/nested_class", run>;

}  // namespace
