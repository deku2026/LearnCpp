// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : trap_representation_note
// Topic id : part6/g/section04/trap_representation_note

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <type_traits>

namespace {

enum class Status : std::uint8_t { idle = 0, ready = 1, failed = 2 };

[[nodiscard]] constexpr std::optional<Status> decode_status(std::uint8_t bits) noexcept {
    switch (bits) {
        case 0:
            return Status::idle;
        case 1:
            return Status::ready;
        case 2:
            return Status::failed;
        default:
            return std::nullopt;
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/trap_representation_note"};

    static_assert(std::has_unique_object_representations_v<std::uint8_t>);
    LEARN_EXPECT_EQ(checks, decode_status(1), std::optional<Status>{Status::ready});
    LEARN_EXPECT(checks, !decode_status(0xFFU).has_value());

#if 0
    std::array<unsigned char, sizeof(bool)> invalid_bits{};
    invalid_bits.fill(0xFFU);
    bool potentially_invalid;
    std::memcpy(&potentially_invalid, invalid_bits.data(), sizeof(bool));
    return potentially_invalid;  // Do not manufacture and read invalid value representations.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/trap_representation_note", run>;

}  // namespace
