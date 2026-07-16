// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : strict_aliasing_violation_deep
// Topic id : part6/g/section03/strict_aliasing_violation_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace {

template <class To, class From>
[[nodiscard]] To copy_representation(const From& source) noexcept {
    static_assert(sizeof(To) == sizeof(From));
    static_assert(std::is_trivially_copyable_v<To>);
    static_assert(std::is_trivially_copyable_v<From>);
    To destination{};
    std::memcpy(&destination, &source, sizeof(destination));
    return destination;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/strict_aliasing_violation_deep"};

    constexpr float value = 1.0F;
    constexpr auto bits = std::bit_cast<std::uint32_t>(value);
    static_assert(std::bit_cast<float>(bits) == value);
    LEARN_EXPECT_EQ(checks, copy_representation<std::uint32_t>(value), bits);
    LEARN_EXPECT_EQ(checks, copy_representation<float>(bits), value);

#if 0
    // A pointer cast does not make float type-accessible through an int object.
    int object = 0;
    float invalid = *reinterpret_cast<float*>(&object);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/strict_aliasing_violation_deep", run>;

}  // namespace
