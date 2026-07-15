// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : copy_construction
// Topic id : part2/stage05/section01/copy_construction
//
// Covers: deep copy construction, independent ownership, source unchanged

#include "learn/topic_registry.hpp"

#include <algorithm>
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
            data_[i] = static_cast<int>(i);
        }
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    std::size_t size() const { return size_; }
    int at(std::size_t i) const { return data_[i]; }
    void set(std::size_t i, int v) { data_[i] = v; }
};

void demo_basics() {
    std::string a = "hello";
    std::string b = a;  // copy construction
    LEARN_CHECK(a == "hello");
    LEARN_CHECK(b == "hello");
    b[0] = 'H';
    LEARN_CHECK(a == "hello");
    LEARN_CHECK(b == "Hello");
}

void demo_intermediate() {
    Buffer a(4);
    Buffer b = a;  // deep copy: new allocation + element copy
    LEARN_CHECK(a.size() == 4);
    LEARN_CHECK(b.size() == 4);
    LEARN_CHECK(a.at(2) == 2);
    LEARN_CHECK(b.at(2) == 2);

    b.set(2, 99);
    LEARN_CHECK(a.at(2) == 2);   // source unchanged
    LEARN_CHECK(b.at(2) == 99);  // independent storage
}

void demo_expert() {
    std::vector<std::string> v1{"a", "bb", "ccc"};
    std::vector<std::string> v2 = v1;
    LEARN_CHECK(v1.size() == v2.size());
    v2.push_back("dddd");
    LEARN_CHECK(v1.size() == 3);
    LEARN_CHECK(v2.size() == 4);
    LEARN_CHECK(v1[0] == "a");
    LEARN_CHECK(v2[0] == "a");
    v2[0][0] = 'A';
    LEARN_CHECK(v1[0] == "a");  // independent after copy
    LEARN_CHECK(v2[0] == "A");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/copy_construction", run>;

}  // namespace
