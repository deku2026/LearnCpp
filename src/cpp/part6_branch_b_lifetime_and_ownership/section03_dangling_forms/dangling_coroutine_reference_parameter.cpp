// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_coroutine_reference_parameter
// Topic id : part6/b/section03/dangling_coroutine_reference_parameter
//
// Covers: coroutine frame may outlive caller's locals; pass by value or own

#include "learn/topic_registry.hpp"

#include <coroutine>
#include <optional>

namespace {

struct Task {
    struct promise_type {
        int value = 0;
        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(int v) { value = v; }
        void unhandled_exception() {}
    };
    std::coroutine_handle<promise_type> h;
    explicit Task(std::coroutine_handle<promise_type> handle) : h(handle) {}
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&& o) noexcept : h(o.h) { o.h = {}; }
    ~Task() {
        if (h) h.destroy();
    }
    int get() const { return h.promise().value; }
};

Task add_by_value(int a, int b) {
    co_return a + b;
}

void demo_basics() {
    Task t = add_by_value(2, 3);
    LEARN_CHECK(t.get() == 5);
}

void demo_intermediate() {
    // SAFE: copy parameters into coroutine by value so frame owns them.
    int x = 10;
    int y = 20;
    Task t = add_by_value(x, y);
    LEARN_CHECK(t.get() == 30);
}

void demo_expert() {
    // Teaching (not executed): co_return ref_param where ref_param binds to temporary
    // that dies at the call full-expression can dangle when the coroutine resumes later.
    Task t = add_by_value(1, 1);
    LEARN_CHECK(t.get() == 2);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_coroutine_reference_parameter", run>;

}  // namespace
