// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §1.2
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : rule_of_zero_three_five_idiom
// Topic id : part2/stage15/section01/rule_of_zero_three_five_idiom
//
// Rule of 0 / 3 / 5 for special members.
// Motive : keep copy/move/destructor consistent so resources are not double-freed
//          or leaked when the compiler generates the rest.
// Modern : Rule of 0 first — compose RAII members (string, vector, unique_ptr).
// Pitfall: customizing one of dtor/copy/move without the others (Rule of 5).

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

// ---------- Rule of 0: let members own everything ----------
struct Person {
    std::string name;
    std::vector<int> scores;
    // no special members written → compiler-generated copy/move/dtor are correct
};

// ---------- Rule of 5: raw owning pointer (teaching only; prefer unique_ptr) ----------
class Buffer5 {
    std::size_t n_ = 0;
    char* data_ = nullptr;

public:
    Buffer5() = default;
    explicit Buffer5(std::size_t n) : n_(n), data_(n ? new char[n]{} : nullptr) {}

    ~Buffer5() { delete[] data_; }

    Buffer5(const Buffer5& o) : n_(o.n_), data_(o.n_ ? new char[o.n_] : nullptr) {
        if (data_) {
            std::memcpy(data_, o.data_, n_);
        }
    }

    Buffer5& operator=(const Buffer5& o) {
        if (this == &o) {
            return *this;
        }
        Buffer5 tmp(o);  // reuse copy ctor
        swap(*this, tmp);
        return *this;
    }

    Buffer5(Buffer5&& o) noexcept : n_(std::exchange(o.n_, 0)), data_(std::exchange(o.data_, nullptr)) {}

    Buffer5& operator=(Buffer5&& o) noexcept {
        if (this != &o) {
            delete[] data_;
            n_ = std::exchange(o.n_, 0);
            data_ = std::exchange(o.data_, nullptr);
        }
        return *this;
    }

    friend void swap(Buffer5& a, Buffer5& b) noexcept {
        using std::swap;
        swap(a.n_, b.n_);
        swap(a.data_, b.data_);
    }

    std::size_t size() const noexcept { return n_; }
    char* data() noexcept { return data_; }
    const char* data() const noexcept { return data_; }
};

// ---------- Rule of 0 rewrite of Buffer5 ----------
struct Buffer0 {
    std::unique_ptr<char[]> data;
    std::size_t n = 0;

    Buffer0() = default;
    explicit Buffer0(std::size_t size) : data(size ? std::make_unique<char[]>(size) : nullptr), n(size) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== Rule of 0 ==\n";
    Person a{.name = "Ada", .scores = {1, 2, 3}};
    Person b = a;  // deep copy via member copy
    b.name = "Bob";
    assert(a.name == "Ada");
    assert(b.scores.size() == 3);
    std::cout << "  Person copy works without any special members\n";

    Person c = std::move(a);
    assert(c.name == "Ada");
    std::cout << "  Person move also compiler-generated\n";

    std::cout << "== Rule of 5 (raw owner) ==\n";
    Buffer5 x(4);
    std::memcpy(x.data(), "xyz", 3);
    Buffer5 y = x;
    assert(y.size() == 4);
    assert(y.data()[0] == 'x');
    Buffer5 z = std::move(y);
    assert(z.size() == 4);
    assert(y.size() == 0);
    std::cout << "  Buffer5 copy/move/dtor all hand-written and consistent\n";

    std::cout << "== Rule of 0 rewrite ==\n";
    Buffer0 u(8);
    Buffer0 v = std::move(u);  // unique_ptr moves
    assert(v.n == 8);
    std::cout << "  unique_ptr member ⇒ Rule of 0 preferred over Rule of 5\n";

    std::cout << "pitfall: custom dtor without copy/move → shallow-copy double free\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/rule_of_zero_three_five_idiom", run>;

}  // namespace
