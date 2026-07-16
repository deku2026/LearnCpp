// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : custom_comparator_hash_projection
// Topic id : part3/section02/custom_comparator_hash_projection
// Reference: C++23 working draft [alg.sorting], [unord.req], [range.utility].

#include "learn/example_support.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

namespace {

struct Employee {
    int id;
    std::string name;
    int salary;
};

struct BySalaryThenId {
    bool operator()(const Employee& left, const Employee& right) const noexcept {
        if (left.salary != right.salary) {
            return left.salary < right.salary;
        }
        return left.id < right.id;
    }
};

struct EmployeeIdHash {
    std::size_t operator()(const Employee& value) const noexcept { return std::hash<int>{}(value.id); }
};

struct SameEmployeeId {
    bool operator()(const Employee& left, const Employee& right) const noexcept { return left.id == right.id; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/custom_comparator_hash_projection";
    learn::ExampleChecks checks{topic};

    const Employee ada{7, "Ada", 120};
    const Employee linus{9, "Linus", 100};
    const Employee grace{3, "Grace", 120};

    // An ordered-container comparator must impose a strict weak ordering. The tie-breaker keeps
    // employees with equal salaries distinct instead of making the set treat them as equivalent.
    std::set<Employee, BySalaryThenId> payroll{ada, linus, grace};
    LEARN_EXPECT_EQ(checks, payroll.begin()->name, std::string{"Linus"});
    LEARN_EXPECT_EQ(checks, payroll.size(), std::size_t{3});

    // Equal keys must always hash alike. Here identity intentionally means employee id only.
    std::unordered_set<Employee, EmployeeIdHash, SameEmployeeId> identities;
    identities.insert(ada);
    const auto [position, inserted] = identities.insert(Employee{7, "renamed", 999});
    LEARN_EXPECT(checks, !inserted);
    LEARN_EXPECT_EQ(checks, position->name, std::string{"Ada"});

    // A ranges projection separates "which field" from the algorithm's ordering operation.
    std::vector employees{ada, linus, grace};
    std::ranges::sort(employees, std::ranges::less{}, &Employee::salary);
    LEARN_EXPECT_EQ(checks, employees.front().salary, 100);
    const auto named = std::ranges::find(employees, std::string{"Ada"}, &Employee::name);
    LEARN_EXPECT(checks, named != employees.end());
    LEARN_EXPECT_EQ(checks, named->id, 7);

#if 0
    struct BadComparator {
        bool operator()(int left, int right) const { return left <= right; }
    };
    std::set<int, BadComparator> broken;  // <= is not irreflexive: container behavior is undefined.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/custom_comparator_hash_projection", run>;

}  // namespace
