// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : type_punning_with_bit_cast_or_memcpy
// Topic id : part6/g/section04/type_punning_with_bit_cast_or_memcpy

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>
#include <version>

namespace {

template <class To, class From>
[[nodiscard]] To memcpy_cast(const From& source) noexcept {
    static_assert(sizeof(To) == sizeof(From));
    static_assert(std::is_trivially_copyable_v<To>);
    static_assert(std::is_trivially_copyable_v<From>);
    To result{};
    std::memcpy(&result, &source, sizeof(result));
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/type_punning_with_bit_cast_or_memcpy"};

    const float source = 1.0F;
#if defined(__cpp_lib_bit_cast) && __cpp_lib_bit_cast >= 201806L
    const auto bits = std::bit_cast<std::uint32_t>(source);
    LEARN_EXPECT_EQ(checks, std::bit_cast<float>(bits), source);
#else
    const auto bits = memcpy_cast<std::uint32_t>(source);
#endif
    LEARN_EXPECT_EQ(checks, memcpy_cast<float>(bits), source);
    if constexpr (std::numeric_limits<float>::is_iec559 && sizeof(float) == 4) {
        LEARN_EXPECT_EQ(checks, bits, 0x3F800000U);
    }

#if 0
    const auto invalid = *reinterpret_cast<const std::uint32_t*>(&source);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/type_punning_with_bit_cast_or_memcpy", run>;

}  // namespace
