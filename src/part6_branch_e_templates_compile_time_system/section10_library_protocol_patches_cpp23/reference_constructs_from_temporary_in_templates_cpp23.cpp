// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : reference_constructs_from_temporary_in_templates_cpp23
// Topic id : part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <memory>
#include <type_traits>

namespace {

template <class T>
class SafeReference {
public:
    template <class U>
        requires std::same_as<std::remove_cvref_t<U>, T> && std::is_lvalue_reference_v<U&&>
    explicit constexpr SafeReference(U&& value) noexcept : pointer_(std::addressof(value)) {}

    [[nodiscard]] constexpr T& get() const noexcept { return *pointer_; }

private:
    T* pointer_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23"};

    int value = 42;
    SafeReference<int> safe{value};
    LEARN_EXPECT_EQ(checks, safe.get(), 42);
    static_assert(!std::constructible_from<SafeReference<int>, int&&>);

#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L && \
    (!defined(LEARNCPP_HAS_REFERENCE_FROM_TEMPORARY_TRAITS) || LEARNCPP_HAS_REFERENCE_FROM_TEMPORARY_TRAITS)
    static_assert(std::reference_constructs_from_temporary_v<const int&, int>);
    static_assert(std::reference_converts_from_temporary_v<const int&, int>);
    static_assert(!std::reference_constructs_from_temporary_v<int&, int&>);
    LEARN_EXPECT(checks, (std::reference_constructs_from_temporary_v<const int&, int>));
#else
    // Requiring an lvalue is a useful conservative fallback, though the C++23
    // traits can diagnose subtler conversion-created temporaries.
    LEARN_EXPECT(checks, std::is_lvalue_reference_v<decltype((value))>);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section10/reference_constructs_from_temporary_in_templates_cpp23", run>;

}  // namespace
