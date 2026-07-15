// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section02_raii
// Item     : raii_resource_handle
// Topic id : part2/stage04/section02/raii_resource_handle
//
// Covers: RAII acquire in ctor / release in dtor, no copy, movable handle

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

int g_open = 0;
int g_close = 0;

class Handle {
    int id_ = 0;

public:
    explicit Handle(int id) : id_(id) { ++g_open; }

    ~Handle() {
        if (id_ != 0) {
            ++g_close;
            id_ = 0;
        }
    }

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    Handle(Handle&& other) noexcept : id_(std::exchange(other.id_, 0)) {}
    Handle& operator=(Handle&& other) noexcept {
        if (this != &other) {
            if (id_ != 0) {
                ++g_close;
            }
            id_ = std::exchange(other.id_, 0);
        }
        return *this;
    }

    int id() const { return id_; }
    bool empty() const { return id_ == 0; }
};

void demo_basics() {
    g_open = g_close = 0;
    {
        Handle h{1};
        LEARN_CHECK(h.id() == 1);
        LEARN_CHECK(g_open == 1);
    }
    LEARN_CHECK(g_close == 1);
}

void demo_intermediate() {
    g_open = g_close = 0;
    Handle a{7};
    Handle b = std::move(a);
    LEARN_CHECK(a.empty());
    LEARN_CHECK(b.id() == 7);
    LEARN_CHECK(g_open == 1);
    LEARN_CHECK(g_close == 0);
}

void demo_expert() {
    g_open = g_close = 0;
    Handle a{1};
    Handle b{2};
    a = std::move(b);
    LEARN_CHECK(a.id() == 2);
    LEARN_CHECK(b.empty());
    LEARN_CHECK(g_close == 1);
    LEARN_CHECK(g_open == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section02/raii_resource_handle", run>;

}  // namespace
