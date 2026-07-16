// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : destruction_order
// Topic id : part6/b/section01/destruction_order
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/b/section01/destruction_order";

struct Tracer {
    Tracer(std::vector<std::string>& destination, std::string object_name)
        : events(destination), name(std::move(object_name)) {
        events.push_back(this->name + "+");
    }
    ~Tracer() { events.push_back(name + "-"); }
    std::vector<std::string>& events;
    std::string name;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::vector<std::string> events;
    {
        Tracer first{events, "first"};
        Tracer second{events, "second"};
        (void)first;
        (void)second;
    }
    LEARN_EXPECT_EQ(checks, events, std::vector<std::string>({"first+", "second+", "second-", "first-"}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section01/destruction_order", run>;

}  // namespace
