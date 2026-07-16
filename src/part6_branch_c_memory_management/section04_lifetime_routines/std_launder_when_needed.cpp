// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : std_launder_when_needed
// Topic id : part6/c/section04/std_launder_when_needed
// Reference: C++23 working draft [ptr.launder], [basic.life].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <new>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section04/std_launder_when_needed";
    learn::ExampleChecks checks{topic};
    alignas(int) std::byte storage[sizeof(int)];

    // A complete const object is not transparently replaceable. After reusing its storage, the old
    // pointer must not be dereferenced; launder obtains a pointer to the new object at that address.
    const int* old_pointer = ::new (static_cast<void*>(storage)) const int{11};
    LEARN_EXPECT_EQ(checks, *old_pointer, 11);
    std::destroy_at(old_pointer);

    const int* replacement = ::new (static_cast<void*>(storage)) const int{29};
    const int* refreshed = std::launder(old_pointer);
    LEARN_EXPECT_EQ(checks, refreshed, replacement);
    LEARN_EXPECT_EQ(checks, *refreshed, 29);
    std::destroy_at(replacement);

#if 0
    const int stale = *old_pointer;  // Undefined behavior after the first object's lifetime ended.
#endif
    // launder does not create an object, change alignment, or make arbitrary type-punning valid;
    // it only refreshes provenance under its same-type and reachability preconditions.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/std_launder_when_needed", run>;

}  // namespace
