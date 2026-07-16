// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : compile_time_sort
// Topic id : part2/stage07/section05/compile_time_sort
// References: N4950 [alg.sort], [algorithms]; P0202R3.

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <functional>

namespace {

#if defined(__cpp_lib_constexpr_algorithms) && __cpp_lib_constexpr_algorithms >= 201806L

constexpr auto sorted_numbers() {
    std::array values{9, -1, 4, 4, 2, 7};
    std::sort(values.begin(), values.end());
    return values;
}

struct Task {
    int id;
    int priority;
    friend constexpr bool operator==(const Task&, const Task&) = default;
};

constexpr auto sorted_tasks_by_priority() {
    std::array tasks{Task{1, 30}, Task{2, 10}, Task{3, 20}};
    std::ranges::sort(tasks, std::ranges::less{}, &Task::priority);
    return tasks;
}

inline constexpr auto numbers = sorted_numbers();
inline constexpr auto tasks = sorted_tasks_by_priority();

static_assert(numbers == std::array{-1, 2, 4, 4, 7, 9});
static_assert(tasks[0] == Task{2, 10});
static_assert(tasks[2] == Task{1, 30});

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_constexpr_algorithms) && __cpp_lib_constexpr_algorithms >= 201806L
    ::learn::ExampleChecks checks{"part2/stage07/section05/compile_time_sort"};

    LEARN_EXPECT(checks, std::is_sorted(numbers.begin(), numbers.end()));
    LEARN_EXPECT_EQ(checks, numbers.front(), -1);
    LEARN_EXPECT_EQ(checks, numbers.back(), 9);
    LEARN_EXPECT_EQ(checks, tasks[0].id, 2);
    LEARN_EXPECT_EQ(checks, tasks[2].priority, 30);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section05/compile_time_sort",
                                               "__cpp_lib_constexpr_algorithms >= 201806L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/compile_time_sort", run>;

}  // namespace
