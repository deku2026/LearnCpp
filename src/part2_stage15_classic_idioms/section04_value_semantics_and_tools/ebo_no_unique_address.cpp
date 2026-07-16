// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : ebo_no_unique_address
// Topic id : part2/stage15/section04/ebo_no_unique_address
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <cstddef>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section04/ebo_no_unique_address";

struct EmptyPolicy {};

struct MemberStorage {
    EmptyPolicy policy;
    int value;
};

struct BaseStorage : EmptyPolicy {
    int value;
};

struct AttributeStorage {
#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]] EmptyPolicy policy;
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]] EmptyPolicy policy;
#else
    EmptyPolicy policy;
#endif
    int value;
};

static_assert(std::is_empty_v<EmptyPolicy>);
static_assert(sizeof(BaseStorage) <= sizeof(MemberStorage));
static_assert(sizeof(AttributeStorage) <= sizeof(MemberStorage));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    BaseStorage base{{}, 7};
    AttributeStorage attributed{{}, 9};
    LEARN_EXPECT_EQ(checks, base.value, 7);
    LEARN_EXPECT_EQ(checks, attributed.value, 9);
    // Exact sizes are ABI choices; only the permitted overlap and relative observation are taught.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section04/ebo_no_unique_address", run>;

}  // namespace
