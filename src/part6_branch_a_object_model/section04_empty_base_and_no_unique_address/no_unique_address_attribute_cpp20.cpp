// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section04_empty_base_and_no_unique_address
// Item     : no_unique_address_attribute_cpp20
// Topic id : part6/a/section04/no_unique_address_attribute_cpp20
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/a/section04/no_unique_address_attribute_cpp20";

struct Empty {};
struct Ordinary {
    Empty empty;
    int value;
};
struct Compressed {
#if __has_cpp_attribute(no_unique_address)
    [[no_unique_address]] Empty empty;
#elif __has_cpp_attribute(msvc::no_unique_address)
    [[msvc::no_unique_address]] Empty empty;
#else
    Empty empty;
#endif
    int value;
};

static_assert(sizeof(Compressed) <= sizeof(Ordinary));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Compressed compressed{{}, 23};
    LEARN_EXPECT_EQ(checks, compressed.value, 23);
    LEARN_EXPECT(checks, sizeof(Compressed) >= sizeof(int));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section04/no_unique_address_attribute_cpp20", run>;

}  // namespace
