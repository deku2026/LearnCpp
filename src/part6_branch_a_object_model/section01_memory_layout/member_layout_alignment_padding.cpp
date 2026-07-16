// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section01_memory_layout
// Item     : member_layout_alignment_padding
// Topic id : part6/a/section01/member_layout_alignment_padding
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/a/section01/member_layout_alignment_padding";

struct Mixed {
    char tag;
    std::uint32_t count;
    char state;
};

struct Reordered {
    std::uint32_t count;
    char tag;
    char state;
};

static_assert(std::is_standard_layout_v<Mixed>);
static_assert(offsetof(Mixed, tag) == 0);
static_assert(offsetof(Mixed, count) >= sizeof(char));
static_assert(offsetof(Mixed, state) > offsetof(Mixed, count));
static_assert(sizeof(Mixed) >= sizeof(char) * 2 + sizeof(std::uint32_t));

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT(checks, alignof(Mixed) >= alignof(std::uint32_t));
    LEARN_EXPECT(checks, sizeof(Reordered) >= sizeof(std::uint32_t) + 2U);
    LEARN_EXPECT(checks, sizeof(Mixed) % alignof(Mixed) == 0U);  // Arrays require every next object to be aligned.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section01/member_layout_alignment_padding", run>;

}  // namespace
