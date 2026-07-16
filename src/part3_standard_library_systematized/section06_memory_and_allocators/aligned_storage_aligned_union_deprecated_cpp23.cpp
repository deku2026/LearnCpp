// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : aligned_storage_aligned_union_deprecated_cpp23
// Topic id : part3/section06/aligned_storage_aligned_union_deprecated_cpp23
// References: C++23 [meta.trans.other], P1413R3

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <variant>

namespace {

constexpr std::string_view kTopic = "part3/section06/aligned_storage_aligned_union_deprecated_cpp23";

struct alignas(32) WideValue {
    std::array<int, 8> lanes{};
};

constexpr std::size_t kStorageSize = std::max(sizeof(std::string), sizeof(WideValue));
constexpr std::size_t kStorageAlignment = std::max(alignof(std::string), alignof(WideValue));

struct RawSlot {
    alignas(kStorageAlignment) std::array<std::byte, kStorageSize> bytes{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    static_assert(sizeof(RawSlot) >= kStorageSize);
    static_assert(alignof(RawSlot) >= alignof(WideValue));

    // C++23 deprecates aligned_storage/aligned_union. An explicit alignas byte
    // buffer is the raw-storage replacement when manual lifetime is essential.
    RawSlot slot;
    auto* text = std::construct_at(reinterpret_cast<std::string*>(slot.bytes.data()), "manual lifetime");
    LEARN_EXPECT_EQ(checks, *text, std::string{"manual lifetime"});
    std::destroy_at(text);

    auto* wide =
        std::construct_at(reinterpret_cast<WideValue*>(slot.bytes.data()), WideValue{{2, 3, 5, 7, 11, 13, 17, 19}});
    LEARN_EXPECT_EQ(checks, wide->lanes[0] + wide->lanes[3], 9);
    const auto address = reinterpret_cast<std::uintptr_t>(wide);
    LEARN_EXPECT_EQ(checks, address % alignof(WideValue), std::uintptr_t{0});
    std::destroy_at(wide);

    // If the alternatives are known, variant is safer: it tracks the active
    // object and performs construction/destruction automatically.
    std::variant<std::string, WideValue> value{std::in_place_type<std::string>, "managed"};
    LEARN_EXPECT_EQ(checks, std::get<std::string>(value), std::string{"managed"});
    value.emplace<WideValue>(WideValue{{11, 13, 17, 19, 23, 29, 31, 37}});
    LEARN_EXPECT_EQ(checks, std::get<WideValue>(value).lanes[2], 17);
    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part3/section06/aligned_storage_aligned_union_deprecated_cpp23", run>;

}  // namespace
