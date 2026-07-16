// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part2/stage05/section04/constexpr_unique_ptr_cpp23
// References: [unique.ptr], P2273R3, SD-FeatureTest __cpp_lib_constexpr_memory

#include "learn/example_support.hpp"

#include <memory>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/constexpr_unique_ptr_cpp23";

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
constexpr int compile_time_ownership() {
    auto value = std::make_unique<int>(40);
    *value += 2;
    return *value;  // The allocation is released during constant evaluation.
}
static_assert(compile_time_ownership() == 42);
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto value = std::make_unique<int>(9);
    LEARN_EXPECT_EQ(checks, *value, 9);
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    LEARN_EXPECT_EQ(checks, compile_time_ownership(), 42);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/constexpr_unique_ptr_cpp23", run>;

}  // namespace
