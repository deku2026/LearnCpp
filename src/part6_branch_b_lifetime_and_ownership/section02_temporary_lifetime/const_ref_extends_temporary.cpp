// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section02_temporary_lifetime
// Item     : const_ref_extends_temporary
// Topic id : part6/b/section02/const_ref_extends_temporary
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/b/section02/const_ref_extends_temporary";

struct Record {
    std::string name;
    int score;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const Record& record = Record{"Ada", 10};
    LEARN_EXPECT_EQ(checks, record.name, std::string{"Ada"});
    LEARN_EXPECT_EQ(checks, record.score, 10);
    const std::string& member = Record{"Lin", 9}.name;
    LEARN_EXPECT_EQ(checks, member, std::string{"Lin"});
    // Binding the reference directly performs lifetime extension; returning a reference does not relay it.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section02/const_ref_extends_temporary", run>;

}  // namespace
