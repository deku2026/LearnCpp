// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : move_assignment
// Topic id : part2/stage05/section01/move_assignment
//
// Covers: move assignment transfers ownership, releases old resource

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace {

class Buffer {
    int* data_ = nullptr;
    std::size_t size_ = 0;

public:
    explicit Buffer(std::size_t n) : data_(new int[n]{}), size_(n) {
        for (std::size_t i = 0; i < n; ++i) {
            data_[i] = static_cast<int>(10 + i);
        }
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    int at(std::size_t i) const { return data_[i]; }
    int* data() const { return data_; }
};

void demo_basics() {
    std::string a = "source";
    std::string b = "target";
    b = std::move(a);
    LEARN_CHECK(b == "source");
    a = "ok";
    LEARN_CHECK(a == "ok");
}

void demo_intermediate() {
    Buffer a(4);
    Buffer b(1);
    const int* stolen = a.data();
    b = std::move(a);
    LEARN_CHECK(b.size() == 4);
    LEARN_CHECK(b.data() == stolen);
    LEARN_CHECK(b.at(0) == 10);
    LEARN_CHECK(a.empty());
    LEARN_CHECK(a.data() == nullptr);
}

void demo_expert() {
    Buffer a(3);
    Buffer* p = &a;
    a = std::move(*p);  // self-move: remains valid
    LEARN_CHECK(a.size() == 3 || a.empty());

    std::vector<int> v1{1, 2, 3, 4};
    std::vector<int> v2{9};
    v2 = std::move(v1);
    LEARN_CHECK(v2.size() == 4);
    LEARN_CHECK(v2[3] == 4);
    v1 = std::vector<int>{5};
    LEARN_CHECK(v1.size() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/move_assignment", run>;

}  // namespace
