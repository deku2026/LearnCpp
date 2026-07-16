// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : lifetime_violation_deep
// Topic id : part6/g/section03/lifetime_violation_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <memory>
#include <new>
#include <optional>

namespace {

struct Widget {
    int value;
    inline static int live_count{};
    explicit Widget(int number) noexcept : value(number) { ++live_count; }
    ~Widget() { --live_count; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/lifetime_violation_deep"};

    alignas(Widget) std::byte storage[sizeof(Widget)];
    Widget* object = std::construct_at(reinterpret_cast<Widget*>(storage), 42);
    LEARN_EXPECT_EQ(checks, Widget::live_count, 1);
    LEARN_EXPECT_EQ(checks, object->value, 42);
    std::destroy_at(object);
    object = nullptr;
    LEARN_EXPECT_EQ(checks, Widget::live_count, 0);
    LEARN_EXPECT(checks, object == nullptr);

    std::optional<Widget> managed;
    managed.emplace(7);
    LEARN_EXPECT_EQ(checks, Widget::live_count, 1);
    managed.reset();
    LEARN_EXPECT_EQ(checks, Widget::live_count, 0);

#if 0
    alignas(Widget) std::byte other_storage[sizeof(Widget)];
    Widget* expired = std::construct_at(reinterpret_cast<Widget*>(other_storage), 9);
    std::destroy_at(expired);
    int invalid = expired->value;  // The pointer remains, but its Widget lifetime ended.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/lifetime_violation_deep", run>;

}  // namespace
