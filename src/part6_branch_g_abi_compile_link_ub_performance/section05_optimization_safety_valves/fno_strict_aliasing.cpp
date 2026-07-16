// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section05_optimization_safety_valves
// Item     : fno_strict_aliasing
// Topic id : part6/g/section05/fno_strict_aliasing

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <bit>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <type_traits>

namespace {

template <class To, class From>
[[nodiscard]] To portable_pun(const From& source) noexcept {
    static_assert(sizeof(To) == sizeof(From));
    static_assert(std::is_trivially_copyable_v<To> && std::is_trivially_copyable_v<From>);
    To result{};
    std::memcpy(&result, &source, sizeof(result));
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section05/fno_strict_aliasing"};

    const float value = 1.0F;
    const auto bits = portable_pun<std::uint32_t>(value);
    LEARN_EXPECT_EQ(checks, portable_pun<float>(bits), value);

    constexpr std::string_view flag{"-fno-strict-aliasing"};
    constexpr std::string_view rule =
        "a build flag can reduce alias-based optimization but cannot make the source portable";
    LEARN_EXPECT(checks, flag.contains("no-strict-aliasing"));
    LEARN_EXPECT(checks, rule.contains("cannot make the source portable"));

#if 0
    // Some toolchains assign extension semantics under the flag; the C++
    // abstract machine still does not bless an incompatible lvalue access.
    auto invalid = *reinterpret_cast<const std::uint32_t*>(&value);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section05/fno_strict_aliasing", run>;

}  // namespace
