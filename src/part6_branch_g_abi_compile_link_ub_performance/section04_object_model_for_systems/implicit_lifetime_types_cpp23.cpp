// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : implicit_lifetime_types_cpp23
// Topic id : part6/g/section04/implicit_lifetime_types_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <string_view>
#include <type_traits>
#include <version>

namespace {

struct PacketHeader {
    unsigned int kind;
    unsigned int size;
};

struct ManagedResource {
    ManagedResource() : value(0) {}
    ~ManagedResource() {}
    int value;
};

template <class T>
inline constexpr bool implicit_lifetime_candidate =
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    std::is_implicit_lifetime_v<T>;
#else
    std::is_trivially_copyable_v<T>;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/implicit_lifetime_types_cpp23"};

    static_assert(implicit_lifetime_candidate<int>);
    static_assert(implicit_lifetime_candidate<PacketHeader>);
    static_assert(!std::is_trivially_copyable_v<ManagedResource>);
    LEARN_EXPECT(checks, implicit_lifetime_candidate<PacketHeader>);

#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    constexpr std::string_view status{"standard trait available"};
#else
    constexpr std::string_view status{"stricter trivially-copyable fallback"};
#endif
    LEARN_EXPECT(checks, !status.empty());

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/implicit_lifetime_types_cpp23", run>;

}  // namespace
