// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : temporary_object_basics
// Topic id : part6/b/section02/temporary_object_basics
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section02/temporary_object_basics";

struct Tracked {
    explicit Tracked(int& live_count) : live(&live_count) { ++*live; }
    Tracked(const Tracked& other) : live(other.live) { ++*live; }
    ~Tracked() { --*live; }
    int* live;
};

void consume(const Tracked&, int& observed) {
    observed = 1;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int live{};
    int observed{};
    consume(Tracked{live}, observed);
    LEARN_EXPECT_EQ(checks, observed, 1);
    LEARN_EXPECT_EQ(checks, live, 0);  // The full-expression ended after the call.
    {
        const Tracked& extended = Tracked{live};
        LEARN_EXPECT_EQ(checks, live, 1);
        (void)extended;
    }
    LEARN_EXPECT_EQ(checks, live, 0);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section02/temporary_object_basics", run>;

}  // namespace
