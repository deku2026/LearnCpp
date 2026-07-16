// Runnable teaching example
// Doc      : 第6部分-支线B-生命周期与所有权.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : static_initialization_order_problem
// Topic id : part6/b/section01/static_initialization_order_problem
// References: C++23 [basic.stc], [basic.start], [basic.life], [class.temporary], [stmt.ranged]

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/b/section01/static_initialization_order_problem";

bool has_cycle(std::string_view node, const std::map<std::string, std::vector<std::string>>& dependencies,
               std::set<std::string>& active, std::set<std::string>& complete) {
    if (complete.contains(std::string{node})) return false;
    if (!active.insert(std::string{node}).second) return true;
    for (const auto& dependency : dependencies.at(std::string{node})) {
        if (has_cycle(dependency, dependencies, active, complete)) return true;
    }
    active.erase(std::string{node});
    complete.insert(std::string{node});
    return false;
}

int& construct_on_first_use() {
    static int value = 23;
    return value;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::map<std::string, std::vector<std::string>> cyclic{{"tu-a", {"tu-b"}}, {"tu-b", {"tu-a"}}};
    std::set<std::string> active;
    std::set<std::string> complete;
    LEARN_EXPECT(checks, has_cycle("tu-a", cyclic, active, complete));
    int& first = construct_on_first_use();
    int& second = construct_on_first_use();
    LEARN_EXPECT(checks, &first == &second);
    LEARN_EXPECT_EQ(checks, first, 23);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/b/section01/static_initialization_order_problem", run>;

}  // namespace
