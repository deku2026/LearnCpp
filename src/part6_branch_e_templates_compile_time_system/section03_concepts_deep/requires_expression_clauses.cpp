// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section03_concepts_deep
// Item     : requires_expression_clauses
// Topic id : part6/e/section03/requires_expression_clauses

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <concepts>
#include <cstddef>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

template <class T>
concept ReadableBuffer = requires(const T& buffer, std::size_t index) {
    typename T::value_type;                                          // type requirement
    buffer.size();                                                   // simple requirement
    { buffer.size() } noexcept -> std::convertible_to<std::size_t>;  // compound
    { buffer[index] } -> std::convertible_to<const typename T::value_type&>;
    requires std::is_trivially_copyable_v<typename T::value_type>;  // nested
};

template <class T>
    requires ReadableBuffer<T>
[[nodiscard]] constexpr std::size_t byte_count(const T& buffer) noexcept {
    return buffer.size() * sizeof(typename T::value_type);
}

struct NonTrivial {
    NonTrivial() = default;
    NonTrivial(const NonTrivial&) {}
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section03/requires_expression_clauses"};

    static_assert(ReadableBuffer<std::vector<int>>);
    static_assert(!ReadableBuffer<std::vector<NonTrivial>>);
    static_assert(!ReadableBuffer<int>);
    const std::vector<int> values{1, 2, 3};
    LEARN_EXPECT_EQ(checks, byte_count(values), values.size() * sizeof(int));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section03/requires_expression_clauses", run>;

}  // namespace
