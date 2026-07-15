// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : copy_assignment
// Topic id : part2/stage05/section01/copy_assignment
//
// Covers: copy assignment, self-assignment safety via copy-and-swap

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>

namespace {

class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    explicit Buffer(std::size_t n) : data_(new int[n]{}), size_(n) {
        for (std::size_t i = 0; i < n; ++i) {
            data_[i] = static_cast<int>(i);
        }
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // By-value + swap: handles copy-assign and self-assign safely
    Buffer& operator=(Buffer other) noexcept {
        swap(other);
        return *this;
    }

    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    void swap(Buffer& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    std::size_t size() const { return size_; }
    int at(std::size_t i) const { return data_[i]; }
    void set(std::size_t i, int v) { data_[i] = v; }
};

void demo_basics() {
    std::string a = "alpha";
    std::string b = "beta";
    b = a;  // copy assignment
    LEARN_CHECK(a == "alpha");
    LEARN_CHECK(b == "alpha");
    a[0] = 'A';
    LEARN_CHECK(b == "alpha");
}

void demo_intermediate() {
    Buffer a(3);
    Buffer b(1);
    b = a;
    LEARN_CHECK(b.size() == 3);
    LEARN_CHECK(b.at(2) == 2);
    b.set(0, 42);
    LEARN_CHECK(a.at(0) == 0);
    LEARN_CHECK(b.at(0) == 42);
}

void demo_expert() {
    Buffer a(5);
    Buffer& alias = a;
    a = alias;  // self-assignment via alias must be safe
    LEARN_CHECK(a.size() == 5);
    LEARN_CHECK(a.at(4) == 4);

    Buffer left(2);
    Buffer right(4);
    left = right;
    LEARN_CHECK(left.size() == 4);
    LEARN_CHECK(right.size() == 4);
    left.set(1, 7);
    LEARN_CHECK(right.at(1) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/copy_assignment", run>;

}  // namespace
