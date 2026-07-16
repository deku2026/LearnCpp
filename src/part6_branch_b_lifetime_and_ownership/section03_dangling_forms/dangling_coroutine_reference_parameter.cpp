// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_coroutine_reference_parameter
// Topic id : part6/b/section03/dangling_coroutine_reference_parameter
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <coroutine>
#include <exception>
#include <ranges>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_coroutine_reference_parameter";

class LazySize {
public:
    struct promise_type {
        std::size_t value{};
        LazySize get_return_object() noexcept {
            return LazySize{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_value(std::size_t result) noexcept { value = result; }
        void unhandled_exception() const noexcept { std::terminate(); }
    };
    explicit LazySize(std::coroutine_handle<promise_type> handle) : handle_(handle) {}
    ~LazySize() {
        if (handle_) handle_.destroy();
    }
    std::size_t finish() {
        handle_.resume();
        return handle_.promise().value;
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

LazySize borrowed_size(const std::string& text) {
    co_return text.size();
}
LazySize owned_size(std::string text) {
    co_return text.size();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::string owner = "alive";
    auto borrowed = borrowed_size(owner);
    auto owned = owned_size(std::string{"frame-owned"});
    LEARN_EXPECT_EQ(checks, borrowed.finish(), 5U);
    LEARN_EXPECT_EQ(checks, owned.finish(), 11U);
#if 0
    auto dangling = borrowed_size(std::string{"temporary"});
    dangling.finish();
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part6/b/section03/dangling_coroutine_reference_parameter", run>;

}  // namespace
