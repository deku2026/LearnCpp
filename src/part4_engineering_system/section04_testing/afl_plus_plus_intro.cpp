// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : afl_plus_plus_intro
// Topic id : part4/section04/afl_plus_plus_intro

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace {

enum class ImageKind { unknown, png, gif };

[[nodiscard]] constexpr ImageKind detect_image(std::span<const std::uint8_t> bytes) noexcept {
    if (bytes.size() >= 4 && bytes[0] == 0x89U && bytes[1] == 'P' && bytes[2] == 'N' && bytes[3] == 'G') {
        return ImageKind::png;
    }
    if (bytes.size() >= 3 && bytes[0] == 'G' && bytes[1] == 'I' && bytes[2] == 'F') {
        return ImageKind::gif;
    }
    return ImageKind::unknown;
}

[[nodiscard]] bool afl_harness(std::span<const std::uint8_t> input) noexcept {
    constexpr std::size_t maximum_input = 4'096;
    const auto bounded = input.first(std::min(input.size(), maximum_input));
    const auto kind = detect_image(bounded);
    return kind == ImageKind::unknown || !bounded.empty();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/afl_plus_plus_intro"};

    std::array corpus{
        std::vector<std::uint8_t>{'G', 'I', 'F'},
        std::vector<std::uint8_t>{0x89U, 'P', 'N', 'G'},
        std::vector<std::uint8_t>{0x00U},
    };
    LEARN_EXPECT_EQ(checks, detect_image(corpus[0]), ImageKind::gif);
    LEARN_EXPECT_EQ(checks, detect_image(corpus[1]), ImageKind::png);

    // This bounded loop models AFL++ corpus mutation without invoking an external fuzzer.
    for (const auto& seed : corpus) {
        LEARN_EXPECT(checks, afl_harness(seed));
        for (std::size_t index = 0; index < seed.size(); ++index) {
            auto mutation = seed;
            mutation[index] ^= 1U;
            LEARN_EXPECT(checks, afl_harness(mutation));
        }
    }

    constexpr std::string_view compile = "afl-clang-fast++ harness.cpp -o harness";
    constexpr std::string_view execute = "afl-fuzz -i seeds -o findings -- ./harness @@";
    LEARN_EXPECT(checks, compile.starts_with("afl-clang-fast"));
    LEARN_EXPECT(checks, execute.contains("-i seeds"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/afl_plus_plus_intro", run>;

}  // namespace
