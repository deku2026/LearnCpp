// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : move_construction
// Topic id : part2/stage05/section01/move_construction
//
// Covers: move ctor steals resources, source nulled, O(1) ownership transfer

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
            data_[i] = static_cast<int>(i + 1);
        }
    }

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    int at(std::size_t i) const { return data_[i]; }
    int* data() const { return data_; }
};

void demo_basics() {
    std::string a = "payload";
    std::string b = std::move(a);  // move construction
    LEARN_CHECK(b == "payload");
    // a is valid but unspecified; reassignment is always safe
    a = "refilled";
    LEARN_CHECK(a == "refilled");
}

void demo_intermediate() {
    Buffer a(8);
    const int* old = a.data();
    LEARN_CHECK(a.size() == 8);
    LEARN_CHECK(a.at(0) == 1);

    Buffer b = std::move(a);  // steal pointer, null source
    LEARN_CHECK(b.size() == 8);
    LEARN_CHECK(b.data() == old);
    LEARN_CHECK(b.at(7) == 8);
    LEARN_CHECK(a.empty());
    LEARN_CHECK(a.data() == nullptr);
}

void demo_expert() {
    std::vector<std::string> src{"x", "y", "z"};
    const auto* first_ptr = src[0].data();
    std::vector<std::string> dst = std::move(src);
    LEARN_CHECK(dst.size() == 3);
    LEARN_CHECK(dst[0] == "x");
    // After move, src is valid; clear/assign are safe
    src.clear();
    LEARN_CHECK(src.empty());
    src.push_back("new");
    LEARN_CHECK(src.size() == 1);
    (void)first_ptr;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/move_construction", run>;

}  // namespace
