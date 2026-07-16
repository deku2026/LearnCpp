// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : byte_char_observation
// Topic id : part6/g/section04/byte_char_observation

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

namespace {

[[nodiscard]] std::uint32_t byte_checksum(std::span<const std::byte> bytes) noexcept {
    std::uint32_t result = 0;
    for (const std::byte value : bytes) {
        result += std::to_integer<std::uint32_t>(value);
    }
    return result;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/byte_char_observation"};

    const std::uint32_t value = 0x01020304U;
    const auto object_view = std::span{&value, 1};
    const auto bytes = std::as_bytes(object_view);
    LEARN_EXPECT_EQ(checks, bytes.size(), sizeof(value));
    LEARN_EXPECT_EQ(checks, byte_checksum(bytes), 10U);

    std::uint32_t reconstructed{};
    std::memcpy(&reconstructed, bytes.data(), bytes.size());
    LEARN_EXPECT_EQ(checks, reconstructed, value);

#if 0
    // The aliasing exemption is one-way: byte observation is allowed, but a
    // byte buffer cannot simply be dereferenced as an unrelated object type.
    auto invalid = *reinterpret_cast<const std::uint32_t*>(bytes.data());
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/byte_char_observation", run>;

}  // namespace
