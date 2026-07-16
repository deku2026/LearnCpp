// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : std_launder
// Topic id : part3/section06/std_launder
// References: C++23 [ptr.launder], [basic.life]

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <new>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section06/std_launder";

struct Version {
    int number{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    alignas(Version) std::array<std::byte, sizeof(Version)> storage{};
    const Version* first = ::new (static_cast<void*>(storage.data())) const Version{1};
    LEARN_EXPECT_EQ(checks, first->number, 1);
    std::destroy_at(first);

    const Version* second = ::new (static_cast<void*>(storage.data())) const Version{2};
    // A complete const object is not transparently replaceable. The stale
    // pointer value may be used as launder's input, but must not be dereferenced.
    const Version* refreshed = std::launder(first);
    LEARN_EXPECT(checks, refreshed == second);
    LEARN_EXPECT_EQ(checks, refreshed->number, 2);
    std::destroy_at(second);

    // In ordinary code, retaining the pointer returned by construction avoids
    // the need for launder. It is a narrow low-level tool, not a cast substitute.
    Version* ordinary = std::construct_at(reinterpret_cast<Version*>(storage.data()), Version{3});
    LEARN_EXPECT_EQ(checks, ordinary->number, 3);
    std::destroy_at(ordinary);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/std_launder", run>;

}  // namespace
