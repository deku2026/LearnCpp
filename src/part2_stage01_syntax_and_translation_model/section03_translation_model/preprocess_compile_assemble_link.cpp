// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : preprocess_compile_assemble_link
// Topic id : part2/stage01/section03/preprocess_compile_assemble_link
// References: N4950 [lex.phases]. Named intermediate files are a common
//             toolchain model; an implementation may combine or omit them.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <type_traits>

namespace {

#define LEARNCPP_STAGE01_PREPROCESSED_VALUE 21
inline constexpr int value_after_preprocessing = LEARNCPP_STAGE01_PREPROCESSED_VALUE * 2;
#undef LEARNCPP_STAGE01_PREPROCESSED_VALUE

constexpr int compiled_function(int value) noexcept {
    return value + 1;
}

enum class BuildStep { preprocess, compile, assemble, link };
enum class Artifact { source_text, preprocessed_tokens, assembly, object_file, program_image };

struct PipelineStage {
    BuildStep step;
    Artifact input;
    Artifact output;
};

inline constexpr std::array pipeline{
    PipelineStage{BuildStep::preprocess, Artifact::source_text, Artifact::preprocessed_tokens},
    PipelineStage{BuildStep::compile, Artifact::preprocessed_tokens, Artifact::assembly},
    PipelineStage{BuildStep::assemble, Artifact::assembly, Artifact::object_file},
    PipelineStage{BuildStep::link, Artifact::object_file, Artifact::program_image},
};

constexpr bool pipeline_is_connected() noexcept {
    for (std::size_t index = 1; index < pipeline.size(); ++index) {
        if (pipeline[index - 1].output != pipeline[index].input) {
            return false;
        }
    }
    return true;
}

static_assert(value_after_preprocessing == 42);
static_assert(std::is_same_v<decltype(&compiled_function), int (*)(int) noexcept>);
static_assert(pipeline_is_connected());

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section03/preprocess_compile_assemble_link"};

    LEARN_EXPECT_EQ(checks, value_after_preprocessing, 42);
    LEARN_EXPECT_EQ(checks, compiled_function(41), 42);
    LEARN_EXPECT_EQ(checks, pipeline.front().input, Artifact::source_text);
    LEARN_EXPECT_EQ(checks, pipeline.back().output, Artifact::program_image);
    LEARN_EXPECT(checks, pipeline_is_connected());

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/preprocess_compile_assemble_link", run>;

}  // namespace
