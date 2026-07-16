// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : string_view_dangling_pitfall
// Topic id : part2/stage10/section06/string_view_dangling_pitfall

#ifdef __has_include
#if __has_include(<string_view>)
#include <string_view>
#define LEARN_HAS_STRING_VIEW_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <string>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/string_view_dangling_pitfall";

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L

// The returned view borrows from input. The contract therefore belongs in the type/API docs.
[[nodiscard]] std::string_view first_word(std::string_view input) noexcept {
    return input.substr(0, input.find(' '));
}

// Use an owning return when the result must outlive the caller's source buffer.
[[nodiscard]] std::string owning_first_word(std::string_view input) {
    return std::string{first_word(input)};
}

class StableText {
public:
    explicit StableText(std::string text) : text_(std::move(text)) {}

    // Keep the owner alive and avoid mutating text_ while this view is in use.
    [[nodiscard]] std::string_view view() const noexcept { return text_; }

private:
    std::string text_;
};

#if 0
// Intentionally non-compiling/unsafe teaching forms: both return views to dead storage.
std::string_view bad_local() {
    std::string local = "gone";
    return local;
}
std::string_view bad_temporary() { return std::string{"gone"}; }
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(LEARN_HAS_STRING_VIEW_HEADER) && defined(__cpp_lib_string_view) && __cpp_lib_string_view >= 201603L
    learn::ExampleChecks checks{topic_id};

    const std::string owner = "borrowed data";
    const auto borrowed = first_word(owner);  // owner is alive for every use below.
    LEARN_EXPECT_EQ(checks, borrowed, "borrowed");
    LEARN_EXPECT_EQ(checks, borrowed.data(), owner.data());

    const auto escaped = owning_first_word(std::string{"temporary input"});
    LEARN_EXPECT_EQ(checks, escaped, "temporary");

    const StableText stable{"member-backed view"};
    const auto member_view = stable.view();
    LEARN_EXPECT_EQ(checks, member_view.substr(0, 6), "member");

    // The type system accepts both lvalue and temporary strings; it cannot encode lifetime.
    static_assert(std::is_constructible_v<std::string_view, const std::string&>);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable(topic_id, "C++17 std::string_view");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_dangling_pitfall", run>;

}  // namespace
