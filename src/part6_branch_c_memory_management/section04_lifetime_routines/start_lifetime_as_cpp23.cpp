// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_cpp23
// Topic id : part6/c/section04/start_lifetime_as_cpp23
// Reference: WG21 P2590R2 (__cpp_lib_start_lifetime_as).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <type_traits>

namespace {

struct Packet {
    int id;
    unsigned flags;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section04/start_lifetime_as_cpp23";
    learn::ExampleChecks checks{topic};
    static_assert(std::is_trivially_copyable_v<Packet>);
    const Packet source{42, 0xA5U};

#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    alignas(Packet) std::array<std::byte, sizeof(Packet)> representation{};
    std::memcpy(representation.data(), &source, sizeof(source));

    // The routine starts an implicit-lifetime Packet in suitably aligned storage while preserving
    // its object representation. It performs no constructor call and is not for arbitrary classes.
    Packet* packet = std::start_lifetime_as<Packet>(representation.data());
    LEARN_EXPECT_EQ(checks, packet->id, 42);
    LEARN_EXPECT_EQ(checks, packet->flags, 0xA5U);
    LEARN_EXPECT_EQ(checks, static_cast<void*>(packet), static_cast<void*>(representation.data()));
#else
    Packet packet{};  // Existing object lifetime makes memcpy into this destination well-defined.
    std::memcpy(&packet, &source, sizeof(source));
    LEARN_EXPECT_EQ(checks, packet.id, 42);
    LEARN_EXPECT_EQ(checks, packet.flags, 0xA5U);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_start_lifetime_as >= 202207L");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_cpp23", run>;

}  // namespace
