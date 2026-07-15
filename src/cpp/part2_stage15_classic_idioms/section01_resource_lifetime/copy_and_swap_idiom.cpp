// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §1.4
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : copy_and_swap_idiom
// Topic id : part2/stage15/section01/copy_and_swap_idiom
//
// Copy-and-swap assignment for strong exception safety.
// Motive : operator= either fully succeeds or leaves *this untouched; self-assign safe.
// Modern : often replaced by defaulted members under Rule of 0; still canonical when
//          you manage raw resources or need a single unified assignment.
// Pitfall: pass-by-value always builds a temporary — can be slower than hand-tuned assign.

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>
#include <utility>

namespace {

class Buffer {
    std::size_t size_ = 0;
    int* data_ = nullptr;

public:
    Buffer() = default;

    explicit Buffer(std::size_t n) : size_(n), data_(n ? new int[n]{} : nullptr) {}

    ~Buffer() { delete[] data_; }

    Buffer(const Buffer& o) : size_(o.size_), data_(o.size_ ? new int[o.size_] : nullptr) {
        if (data_) {
            std::copy(o.data_, o.data_ + size_, data_);
        }
    }

    Buffer(Buffer&& o) noexcept : Buffer() { swap(*this, o); }

    friend void swap(Buffer& a, Buffer& b) noexcept {
        using std::swap;
        swap(a.size_, b.size_);
        swap(a.data_, b.data_);
    }

    // Unified copy + move assignment: argument is already a copy (or moved-in).
    Buffer& operator=(Buffer o) noexcept {
        swap(*this, o);
        return *this;
    }

    std::size_t size() const noexcept { return size_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }
};

// Contrasting weak assignment (not strong-exception-safe if mid-copy throws).
class FragileBuffer {
    std::size_t size_ = 0;
    int* data_ = nullptr;

public:
    FragileBuffer() = default;
    explicit FragileBuffer(std::size_t n) : size_(n), data_(n ? new int[n]{} : nullptr) {}
    ~FragileBuffer() { delete[] data_; }
    FragileBuffer(const FragileBuffer&) = delete;
    FragileBuffer& operator=(const FragileBuffer&) = delete;

    // Teaching sketch only: free first, then allocate — if new throws, *this is empty.
    void assign_weak(const int* src, std::size_t n) {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        data_ = new int[n];  // if this throws, we already lost old data
        size_ = n;
        std::copy(src, src + n, data_);
    }

    std::size_t size() const noexcept { return size_; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== copy-and-swap strong guarantee ==\n";
    Buffer a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    Buffer b(2);
    b[0] = 1;
    b[1] = 2;

    b = a;  // copy-construct temporary, then swap
    assert(b.size() == 3);
    assert(b[0] == 10 && b[2] == 30);
    std::cout << "  copy-assign ok, b={10,20,30}\n";

    // Self-assignment: copy-and-swap builds an independent temporary first.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#endif
    b = b;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    assert(b.size() == 3);
    std::cout << "  self-assignment safe\n";

    Buffer c(1);
    c[0] = 99;
    c = std::move(a);  // move-construct temporary, then swap
    assert(c.size() == 3);
    assert(c[1] == 20);
    std::cout << "  move-assign via same operator=\n";

    std::cout << "pitfall: extra allocation on every assign; hand-tune if hot path\n";
    std::cout << "contrast: weak assign frees first → not strongly exception-safe\n";
    (void)sizeof(FragileBuffer);
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/copy_and_swap_idiom", run>;

}  // namespace
