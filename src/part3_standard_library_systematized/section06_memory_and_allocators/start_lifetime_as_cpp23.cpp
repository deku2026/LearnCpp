// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : start_lifetime_as_cpp23
// Topic id : part3/section06/start_lifetime_as_cpp23
// References: C++23 [obj.lifetime], [mem.start.lifetime], P2590R2, P2679R2

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string_view>
#include <type_traits>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section06/start_lifetime_as_cpp23";

struct PacketHeader {
    std::uint32_t kind{};
    std::uint32_t payload_size{};
};

static_assert(std::is_trivially_copyable_v<PacketHeader>);
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
static_assert(std::is_implicit_lifetime_v<PacketHeader>);
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    ::learn::ExampleChecks checks{kTopic};

    const PacketHeader source{7, 4096};
    alignas(PacketHeader) std::array<std::byte, sizeof(PacketHeader)> storage{};
    std::memcpy(storage.data(), &source, sizeof(source));
    // The bytes already contain an implicit-lifetime type's representation;
    // start_lifetime_as begins the object lifetime without running a constructor.
    const PacketHeader* decoded = std::start_lifetime_as<PacketHeader>(storage.data());
    LEARN_EXPECT_EQ(checks, decoded->kind, source.kind);
    LEARN_EXPECT_EQ(checks, decoded->payload_size, source.payload_size);
    LEARN_EXPECT(checks, reinterpret_cast<const void*>(decoded) == storage.data());

    const std::array<std::uint32_t, 3> source_values{2, 3, 5};
    alignas(std::uint32_t) std::array<std::byte, sizeof(source_values)> array_storage{};
    std::memcpy(array_storage.data(), source_values.data(), sizeof(source_values));
    const std::uint32_t* values =
        std::start_lifetime_as_array<std::uint32_t>(array_storage.data(), source_values.size());
    LEARN_EXPECT_EQ(checks, values[0] + values[1] + values[2], 10U);

#if 0
    // Inactive negative form: a cast alone does not generally begin T's lifetime.
    const auto* not_a_started_object = reinterpret_cast<const PacketHeader*>(storage.data());
    static_cast<void>(not_a_started_object->kind);
#endif
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::start_lifetime_as");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/start_lifetime_as_cpp23", run>;

}  // namespace
