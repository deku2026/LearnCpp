// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part3/section06/constexpr_unique_ptr_cpp23
// References: C++23 [unique.ptr], P2273R3

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section06/constexpr_unique_ptr_cpp23";

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
constexpr int observe_local_through_unique_ptr() {
    int value = 42;
    std::unique_ptr<int> observer{&value};
    const int result = *observer;
    // release is required: the local object is not dynamically allocated.
    static_cast<void>(observer.release());
    return result;
}

static_assert(observe_local_through_unique_ptr() == 42);

constexpr int compile_time_dynamic_sum() {
    std::unique_ptr<int[]> values{new int[4]{}};
    values[0] = 2;
    values[1] = 3;
    values[2] = 5;
    values[3] = 7;
    int sum{};
    for (int index = 0; index < 4; ++index) {
        sum += values[index];
    }
    // Constant evaluation requires the allocation to be released before the
    // expression ends; unique_ptr's constexpr destructor does that.
    return sum;
}

static_assert(compile_time_dynamic_sum() == 17);
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, observe_local_through_unique_ptr(), 42);

    auto first = std::make_unique<int>(42);
    auto second = std::move(first);
    LEARN_EXPECT(checks, first == nullptr);
    LEARN_EXPECT(checks, second != nullptr);
    LEARN_EXPECT_EQ(checks, *second, 42);
    LEARN_EXPECT_EQ(checks, compile_time_dynamic_sum(), 17);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "constexpr std::unique_ptr");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/constexpr_unique_ptr_cpp23", run>;

}  // namespace
