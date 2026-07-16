// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : copy_and_swap_idiom
// Topic id : part2/stage15/section01/copy_and_swap_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section01/copy_and_swap_idiom";

class Document {
public:
    explicit Document(std::string text = {}) : text_(std::move(text)) {}
    Document(const Document&) = default;

    Document& operator=(Document replacement) noexcept {
        swap(*this, replacement);
        return *this;
    }

    friend void swap(Document& left, Document& right) noexcept {
        using std::swap;
        swap(left.text_, right.text_);
    }
    const std::string& text() const noexcept { return text_; }

private:
    std::string text_;
};

void assign_from(Document& target, const Document& source) {
    target = source;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Document first{"alpha"};
    Document second{"beta"};
    first = second;
    LEARN_EXPECT_EQ(checks, first.text(), std::string{"beta"});
    assign_from(second, second);  // The by-value parameter makes self-assignment naturally safe.
    LEARN_EXPECT_EQ(checks, second.text(), std::string{"beta"});
    first = Document{"moved"};
    LEARN_EXPECT_EQ(checks, first.text(), std::string{"moved"});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section01/copy_and_swap_idiom", run>;

}  // namespace
