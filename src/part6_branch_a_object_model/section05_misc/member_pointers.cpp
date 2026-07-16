// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section05_misc
// Item     : member_pointers
// Topic id : part6/a/section05/member_pointers
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <functional>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part6/a/section05/member_pointers";

struct Record {
    int value{3};
    int scale(int factor) const { return value * factor; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int Record::* data = &Record::value;
    int (Record::*function)(int) const = &Record::scale;
    Record record;
    record.*data = 7;
    LEARN_EXPECT_EQ(checks, record.value, 7);
    LEARN_EXPECT_EQ(checks, (record.*function)(3), 21);
    LEARN_EXPECT_EQ(checks, std::invoke(data, record), 7);
    LEARN_EXPECT_EQ(checks, std::invoke(function, &record, 2), 14);
    static_assert(!std::is_pointer_v<decltype(data)>);  // A member pointer is its own type category.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section05/member_pointers", run>;

}  // namespace
