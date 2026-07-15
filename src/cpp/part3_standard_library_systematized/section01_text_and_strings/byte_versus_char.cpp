// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : byte_versus_char
// Topic id : part3/section01/byte_versus_char
//
// Covers: std::byte vs char: binary vs character data

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace {

void demo_basics() {
    char c = 'A';
    assert(static_cast<int>(c) == 65);
    std::byte b{0x41};
    assert(std::to_integer<int>(b) == 0x41);
}

void demo_intermediate() {
    std::vector<std::byte> blob{std::byte{0x00}, std::byte{0xFF}};
    assert(blob.size() == 2);
    assert(std::to_integer<unsigned>(blob[1]) == 255u);
    char text[] = "hi";
    assert(text[0] == 'h');
}

void demo_expert() {
    std::byte x{0b0000'1111};
    x <<= 1;
    assert(std::to_integer<int>(x) == 0b0001'1110);
    // char may be signed; std::byte is not a character type
    static_assert(!std::is_same_v<std::byte, char>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/byte_versus_char", run>;

}  // namespace
