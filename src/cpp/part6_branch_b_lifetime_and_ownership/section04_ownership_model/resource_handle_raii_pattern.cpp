// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : resource_handle_raii_pattern
// Topic id : part6/b/section04/resource_handle_raii_pattern
//
// Covers: RAII handle owns resource; destructor releases

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

struct Handle {
    int* data = nullptr;
    explicit Handle(int v) : data(new int(v)) {}
    ~Handle() { delete data; }
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    Handle(Handle&& o) noexcept : data(o.data) { o.data = nullptr; }
    Handle& operator=(Handle&& o) noexcept {
        if (this != &o) {
            delete data;
            data = o.data;
            o.data = nullptr;
        }
        return *this;
    }
    int get() const { return data ? *data : -1; }
};

void demo_basics() {
    Handle h{5};
    assert(h.get() == 5);
}

void demo_intermediate() {
    Handle a{1};
    Handle b{std::move(a)};
    assert(b.get() == 1);
    assert(a.get() == -1);
}

void demo_expert() {
    {
        Handle h{9};
        assert(h.get() == 9);
    }
    // resource released at scope end
    assert(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/resource_handle_raii_pattern", run>;

}  // namespace
