// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section05_compile_time_containers_and_algorithms
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part2/stage07/section05/constexpr_unique_ptr_cpp23
// References: N4950 [unique.ptr], [expr.const]; P2273R3.

#include "learn/example_support.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace {

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L && \
    defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L

constexpr int smart_pointer_pipeline() {
    auto values = std::make_unique<std::vector<int>>();
    for (int value = 1; value <= 5; ++value) {
        values->push_back(value);
    }

    auto multiplier = std::make_unique<int>(2);
    auto owner = std::move(multiplier);
    if (multiplier || !owner) {
        return -1;
    }

    int sum{};
    for (const int value : *values) {
        sum += value;
    }
    return sum * *owner;
}

static_assert(smart_pointer_pipeline() == 30);

#if 0
// Dynamic storage owned by this object would escape constant evaluation.
constexpr auto persistent_owner = std::make_unique<int>(42);
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L && \
    defined(__cpp_lib_constexpr_vector) && __cpp_lib_constexpr_vector >= 201907L
    ::learn::ExampleChecks checks{"part2/stage07/section05/constexpr_unique_ptr_cpp23"};

    LEARN_EXPECT_EQ(checks, smart_pointer_pipeline(), 30);

    auto runtime_owner = std::make_unique<int>(42);
    auto moved_owner = std::move(runtime_owner);
    LEARN_EXPECT(checks, runtime_owner == nullptr);
    LEARN_EXPECT_EQ(checks, *moved_owner, 42);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section05/constexpr_unique_ptr_cpp23",
                                               "__cpp_lib_constexpr_memory >= 202202L and constexpr vector");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section05/constexpr_unique_ptr_cpp23", run>;

}  // namespace
