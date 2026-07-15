// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : copy_constructor
// Topic id : part2/stage04/section03/copy_constructor
//
// Covers: copy ctor deep copy of owned resource, memberwise default

#include "learn/topic_registry.hpp"

#include <cstring>
#include <string>
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

    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    int size() const { return size_; }
    int& at(int i) { return data_[i]; }
    int at(int i) const { return data_[i]; }
};

struct Pair {
    std::string a;
    std::string b;
};

void demo_basics() {
    Pair p{"hi", "there"};
    Pair q = p;
    LEARN_CHECK(q.a == "hi" && q.b == "there");
    q.a = "x";
    LEARN_CHECK(p.a == "hi");
}

void demo_intermediate() {
    Buffer a{3};
    a.at(0) = 10;
    a.at(1) = 20;
    Buffer b = a;
    LEARN_CHECK(b.size() == 3);
    LEARN_CHECK(b.at(0) == 10);
    b.at(0) = 99;
    LEARN_CHECK(a.at(0) == 10);
}

void demo_expert() {
    // Copy constructs a distinct resource; lifetime is independent.
    Buffer a{2};
    a.at(0) = 1;
    {
        Buffer b = a;
        LEARN_CHECK(b.at(0) == 1);
    }
    LEARN_CHECK(a.at(0) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/copy_constructor", run>;

}  // namespace
