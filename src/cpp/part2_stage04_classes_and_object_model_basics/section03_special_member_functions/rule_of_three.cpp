// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : rule_of_three
// Topic id : part2/stage04/section03/rule_of_three
//
// Covers: Rule of 3 — dtor + copy ctor + copy assign for raw resource

#include "learn/topic_registry.hpp"

#include <cstring>

namespace {

class IntArray {
    int* data_ = nullptr;
    int n_ = 0;

public:
    explicit IntArray(int n) : data_(new int[static_cast<std::size_t>(n)]{}), n_(n) {}

    ~IntArray() { delete[] data_; }

    IntArray(const IntArray& o) : data_(new int[static_cast<std::size_t>(o.n_)]), n_(o.n_) {
        std::memcpy(data_, o.data_, static_cast<std::size_t>(n_) * sizeof(int));
    }

    IntArray& operator=(const IntArray& o) {
        if (this == &o) {
            return *this;
        }
        int* fresh = new int[static_cast<std::size_t>(o.n_)];
        std::memcpy(fresh, o.data_, static_cast<std::size_t>(o.n_) * sizeof(int));
        delete[] data_;
        data_ = fresh;
        n_ = o.n_;
        return *this;
    }

    int size() const { return n_; }
    int& operator[](int i) { return data_[i]; }
    int operator[](int i) const { return data_[i]; }
};

void demo_basics() {
    IntArray a{2};
    a[0] = 1;
    a[1] = 2;
    IntArray b = a;
    LEARN_CHECK(b[1] == 2);
}

void demo_intermediate() {
    IntArray a{1};
    a[0] = 9;
    IntArray b{3};
    b = a;
    LEARN_CHECK(b.size() == 1);
    LEARN_CHECK(b[0] == 9);
}

void demo_expert() {
    IntArray a{2};
    a[0] = 4;
    IntArray b = a;
    b[0] = 0;
    LEARN_CHECK(a[0] == 4);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/rule_of_three", run>;

}  // namespace
