// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : std_launder_when_necessary
// Topic id : part6/g/section04/std_launder_when_necessary

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <memory>
#include <new>

namespace {

struct ImmutableRecord {
    const int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/std_launder_when_necessary"};

    alignas(ImmutableRecord) std::byte storage[sizeof(ImmutableRecord)];
    const auto* original = ::new (static_cast<void*>(storage)) const ImmutableRecord{1};
    LEARN_EXPECT_EQ(checks, original->value, 1);
    std::destroy_at(original);

    [[maybe_unused]] const auto* replacement = ::new (static_cast<void*>(storage)) const ImmutableRecord{42};
    const ImmutableRecord* current = std::launder(original);
    LEARN_EXPECT_EQ(checks, current->value, 42);
    std::destroy_at(current);

#if 0
    std::byte raw[sizeof(ImmutableRecord)];
    auto* invalid = std::launder(reinterpret_cast<ImmutableRecord*>(raw));
    // launder cannot create an object; dereferencing invalid remains undefined.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/std_launder_when_necessary", run>;

}  // namespace
