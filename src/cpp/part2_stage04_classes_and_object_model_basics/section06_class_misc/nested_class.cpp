// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : nested_class
// Topic id : part2/stage04/section06/nested_class
//
// Covers: nested types for implementation detail and public nested API

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

class LinkedList {
    struct Node {
        int value = 0;
        Node* next = nullptr;
    };

    Node* head_ = nullptr;

public:
    class Iterator {
        Node* p_ = nullptr;

    public:
        explicit Iterator(Node* p) : p_(p) {}
        int operator*() const { return p_->value; }
        Iterator& operator++() {
            p_ = p_->next;
            return *this;
        }
        bool operator==(const Iterator& o) const { return p_ == o.p_; }
    };

    ~LinkedList() {
        while (head_) {
            Node* n = head_->next;
            delete head_;
            head_ = n;
        }
    }

    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void push_front(int v) {
        Node* n = new Node{v, head_};
        head_ = n;
    }

    Iterator begin() { return Iterator{head_}; }
    Iterator end() { return Iterator{nullptr}; }
};

void demo_basics() {
    LinkedList list;
    list.push_front(2);
    list.push_front(1);
    auto it = list.begin();
    LEARN_CHECK(*it == 1);
    ++it;
    LEARN_CHECK(*it == 2);
}

void demo_intermediate() {
    LinkedList list;
    list.push_front(5);
    std::vector<int> out;
    for (auto it = list.begin(); it != list.end(); ++it) {
        out.push_back(*it);
    }
    LEARN_CHECK(out.size() == 1);
    LEARN_CHECK(out[0] == 5);
}

void demo_expert() {
    // Nested type name is LinkedList::Iterator
    LinkedList list;
    list.push_front(9);
    LinkedList::Iterator it = list.begin();
    LEARN_CHECK(*it == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/nested_class", run>;

}  // namespace
