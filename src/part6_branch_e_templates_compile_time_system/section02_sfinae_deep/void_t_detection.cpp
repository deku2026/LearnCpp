// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : void_t_detection
// Topic id : part6/e/section02/void_t_detection

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <type_traits>
#include <vector>

namespace {

template <class, class = void>
struct has_value_type : std::false_type {};

template <class T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

template <class, class = void>
struct has_size_and_data : std::false_type {};

template <class T>
struct has_size_and_data<
    T, std::void_t<decltype(std::declval<const T&>().size()), decltype(std::declval<const T&>().data())>>
    : std::true_type {};

struct MinimalRange {
    using value_type = int;
    [[nodiscard]] constexpr std::size_t size() const noexcept { return 0; }
    [[nodiscard]] constexpr const int* data() const noexcept { return nullptr; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section02/void_t_detection"};

    static_assert(has_value_type<std::vector<int>>::value);
    static_assert(!has_value_type<int>::value);
    static_assert(has_size_and_data<MinimalRange>::value);
    static_assert(!has_size_and_data<int>::value);
    LEARN_EXPECT(checks, has_value_type<MinimalRange>::value);
    LEARN_EXPECT(checks, has_size_and_data<std::vector<double>>::value);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/void_t_detection", run>;

}  // namespace
