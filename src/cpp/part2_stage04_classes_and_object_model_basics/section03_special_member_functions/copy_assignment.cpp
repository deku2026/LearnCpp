// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : copy_assignment
// Topic id : part2/stage04/section03/copy_assignment
//
// Covers: copy assignment, self-assignment, strong-ish resource replace

#include "learn/topic_registry.hpp"

#include <cstring>
#include <utility>

namespace {

class Buffer {
    int* data_ = nullptr;
    int size_ = 0;

public:
    explicit Buffer(int n) : data_(new int[static_cast<std::size_t>(n)]{}), size_(n) {}
    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_(new int[static_cast<std::size_t>(other.size_)]), size_(other.size_) {
        std::memcpy(data_, other.data_, static_cast<std::size_t>(size_) * sizeof(int));
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;
        }
        int* fresh = new int[static_cast<std::size_t>(other.size_)];
        std::memcpy(fresh, other.data_, static_cast<std::size_t>(other.size_) * sizeof(int));
        delete[] data_;
        data_ = fresh;
        size_ = other.size_;
        return *this;
    }

    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    int size() const { return size_; }
    int& at(int i) { return data_[i]; }
    int at(int i) const { return data_[i]; }
};

void demo_basics() {
    Buffer a{2};
    a.at(0) = 1;
    a.at(1) = 2;
    Buffer b{1};
    b = a;
    LEARN_CHECK(b.size() == 2);
    LEARN_CHECK(b.at(1) == 2);
}

void demo_intermediate() {
    Buffer a{1};
    a.at(0) = 7;
    // Indirection so -Wself-assign-overloaded cannot see a = a.
    Buffer* p = &a;
    a = *p;
    LEARN_CHECK(a.at(0) == 7);
}

void demo_expert() {
    Buffer a{3};
    a.at(0) = 9;
    Buffer b{1};
    b = a;
    b.at(0) = 0;
    LEARN_CHECK(a.at(0) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/copy_assignment", run>;

}  // namespace
