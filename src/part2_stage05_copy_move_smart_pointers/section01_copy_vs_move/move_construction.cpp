// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : move_construction
// Topic id : part2/stage05/section01/move_construction
// References: [class.copy.ctor]

#include "learn/example_support.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section01/move_construction";

struct Packet {
    explicit Packet(int value) : payload(std::make_unique<int>(value)) {}
    Packet(Packet&&) noexcept = default;
    Packet& operator=(Packet&&) noexcept = default;
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;
    std::unique_ptr<int> payload;
};

static_assert(std::is_nothrow_move_constructible_v<Packet>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Packet source{44};
    const int* address = source.payload.get();
    Packet destination{std::move(source)};

    LEARN_EXPECT_EQ(checks, source.payload.get(), nullptr);
    LEARN_EXPECT_EQ(checks, destination.payload.get(), address);
    LEARN_EXPECT_EQ(checks, *destination.payload, 44);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section01/move_construction", run>;

}  // namespace
