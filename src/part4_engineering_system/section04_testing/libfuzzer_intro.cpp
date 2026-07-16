// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : libfuzzer_intro
// Topic id : part4/section04/libfuzzer_intro

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace {

struct Packet {
    std::uint8_t version{};
    std::span<const std::uint8_t> payload;
};

[[nodiscard]] std::optional<Packet> parse_packet(std::span<const std::uint8_t> bytes) noexcept {
    if (bytes.size() < 2) {
        return std::nullopt;
    }
    const auto payload_size = static_cast<std::size_t>(bytes[1]);
    if (payload_size != bytes.size() - 2) {
        return std::nullopt;
    }
    return Packet{bytes[0], bytes.subspan(2)};
}

// A real libFuzzer adapter exposes this logic through LLVMFuzzerTestOneInput.
[[nodiscard]] bool fuzz_one_input(const std::uint8_t* data, std::size_t size) noexcept {
    constexpr std::size_t input_limit = 256;
    const auto bounded_size = std::min(size, input_limit);
    const std::span bytes{data, bounded_size};
    const auto packet = parse_packet(bytes);
    return !packet || packet->payload.size() == bytes.size() - 2;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/libfuzzer_intro"};

    const std::array corpus{
        std::vector<std::uint8_t>{},
        std::vector<std::uint8_t>{1},
        std::vector<std::uint8_t>{1, 0},
        std::vector<std::uint8_t>{2, 3, 'c', 'p', 'p'},
        std::vector<std::uint8_t>{2, 4, 'c', 'p', 'p'},
    };
    for (const auto& seed : corpus) {
        LEARN_EXPECT(checks, fuzz_one_input(seed.data(), seed.size()));
        for (std::size_t index = 0; index < seed.size(); ++index) {
            auto mutation = seed;
            mutation[index] ^= 0x80U;
            LEARN_EXPECT(checks, fuzz_one_input(mutation.data(), mutation.size()));
        }
    }

    constexpr std::string_view build = "clang++ -fsanitize=fuzzer,address,undefined fuzz_target.cpp";
    LEARN_EXPECT(checks, build.contains("fuzzer"));
    LEARN_EXPECT(checks, build.contains("address"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/libfuzzer_intro", run>;

}  // namespace
