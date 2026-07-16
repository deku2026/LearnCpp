// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : comments
// Topic id : part2/stage01/section01/comments
// Reference: N4950 [lex.comment] and [lex.phases].

#include "learn/example_support.hpp"

#include <array>
#include <string_view>

namespace {

enum class CommentForm { line, block };

struct CommentProperty {
    CommentForm form;
    bool may_span_logical_lines;
    bool may_nest;
};

constexpr std::array properties{
    CommentProperty{CommentForm::line, false, false},
    CommentProperty{CommentForm::block, true, false},
};

static_assert(20 /* comments become whitespace in translation phase 3 */ + 22 == 42);

#if 0
This branch is removed by conditional inclusion before C++ syntax analysis.
It is safer than wrapping code that already contains block comments in another block comment.
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section01/comments"};

    int /* the comment separates two preprocessing tokens */ separated_name = 7;
    LEARN_EXPECT_EQ(checks, separated_name, 7);
    LEARN_EXPECT_EQ(checks, properties[0].form, CommentForm::line);
    LEARN_EXPECT(checks, !properties[0].may_span_logical_lines);
    LEARN_EXPECT(checks, properties[1].may_span_logical_lines);
    LEARN_EXPECT(checks, !properties[1].may_nest);

    // Phase-2 backslash/newline splicing can still make // cover multiple
    // physical source lines; its terminating rule is about a logical new-line.

    // Comment-looking characters inside a literal are ordinary characters.
    constexpr std::string_view markers = R"(// text, /* more text */)";
    LEARN_EXPECT(checks, markers.starts_with("//"));
    LEARN_EXPECT(checks, markers.ends_with("*/"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/comments", run>;

}  // namespace
