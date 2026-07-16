// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : effective_type
// Topic id : part6/g/section04/effective_type

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace {

template <class Access, class Object>
[[nodiscard]] consteval bool corresponding_signedness_types() {
    using A = std::remove_cv_t<Access>;
    using O = std::remove_cv_t<Object>;
    if constexpr (std::is_integral_v<A> && std::is_integral_v<O> && !std::is_same_v<A, bool> &&
                  !std::is_same_v<O, bool>) {
        return std::is_same_v<std::make_unsigned_t<A>, std::make_unsigned_t<O>> &&
               std::is_signed_v<A> != std::is_signed_v<O>;
    }
    return false;
}

template <class Access, class Object>
inline constexpr bool modeled_type_accessible =
    std::is_same_v<std::remove_cv_t<Access>, std::remove_cv_t<Object>> ||
    corresponding_signedness_types<Access, Object>() || std::is_same_v<std::remove_cv_t<Access>, char> ||
    std::is_same_v<std::remove_cv_t<Access>, unsigned char> || std::is_same_v<std::remove_cv_t<Access>, std::byte>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/effective_type"};

    static_assert(modeled_type_accessible<int, int>);
    static_assert(modeled_type_accessible<unsigned int, int>);
    static_assert(!modeled_type_accessible<unsigned long, int>);
    static_assert(modeled_type_accessible<std::byte, double>);
    static_assert(!modeled_type_accessible<float, std::uint32_t>);

    constexpr float value = 1.5F;
    constexpr auto bits = std::bit_cast<std::uint32_t>(value);
    LEARN_EXPECT_EQ(checks, std::bit_cast<float>(bits), value);
    LEARN_EXPECT(checks, (modeled_type_accessible<unsigned char, float>));

#if 0
    std::uint32_t storage = 0;
    float invalid = *reinterpret_cast<float*>(&storage);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/effective_type", run>;

}  // namespace
