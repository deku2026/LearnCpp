// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : std_allocator
// Topic id : part3/section06/std_allocator
// References: C++23 [default.allocator], [allocator.traits], [allocator.requirements]

#include "learn/example_support.hpp"

#include <concepts>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section06/std_allocator";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    using Allocator = std::allocator<std::string>;
    using Traits = std::allocator_traits<Allocator>;
    static_assert(std::same_as<Traits::value_type, std::string>);
    static_assert(std::same_as<Traits::rebind_alloc<int>, std::allocator<int>>);

    Allocator allocator;
    constexpr std::size_t count = 3;
    std::string* storage = Traits::allocate(allocator, count);
    std::size_t constructed{};
    try {
        Traits::construct(allocator, storage + constructed, "alpha");
        ++constructed;
        Traits::construct(allocator, storage + constructed, 3, 'x');
        ++constructed;
        Traits::construct(allocator, storage + constructed, "omega");
        ++constructed;

        LEARN_EXPECT_EQ(checks, storage[0], std::string{"alpha"});
        LEARN_EXPECT_EQ(checks, storage[1], std::string{"xxx"});
        LEARN_EXPECT_EQ(checks, storage[2], std::string{"omega"});
    } catch (...) {
        while (constructed != 0) {
            Traits::destroy(allocator, storage + --constructed);
        }
        Traits::deallocate(allocator, storage, count);
        throw;
    }
    while (constructed != 0) {
        Traits::destroy(allocator, storage + --constructed);
    }
    Traits::deallocate(allocator, storage, count);

    // Containers normally hide this protocol and talk through allocator_traits.
    std::vector<int, std::allocator<int>> values{2, 3, 5, 7};
    LEARN_EXPECT_EQ(checks, values.get_allocator(), std::allocator<int>{});
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{4});
    LEARN_EXPECT(checks, Traits::max_size(allocator) >= count);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/std_allocator", run>;

}  // namespace
