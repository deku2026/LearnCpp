// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_pointer
// Topic id : part6/b/section03/dangling_pointer
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <memory>
#include <ranges>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section03/dangling_pointer";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    auto owner = std::make_shared<int>(23);
    std::weak_ptr<int> observer = owner;
    LEARN_EXPECT_EQ(checks, *observer.lock(), 23);
    owner.reset();
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT(checks, observer.lock() == nullptr);
#if 0
    int* pointer = new int{23};
    delete pointer;
    const int undefined = *pointer;
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section03/dangling_pointer", run>;

}  // namespace
