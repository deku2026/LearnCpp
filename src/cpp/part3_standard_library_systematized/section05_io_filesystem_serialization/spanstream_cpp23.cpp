// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : spanstream_cpp23
// Topic id : part3/section05/spanstream_cpp23
//
// Covers: std::spanstream C++23 fixed buffer streams

#include "learn/topic_registry.hpp"

#include <array>
#include <span>
#include <string>
#include <version>
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
#include <spanstream>
#endif

namespace {

void demo_basics() {
    std::array<char, 16> buf{};
    buf[0] = 'o';
    buf[1] = 'k';
    std::span<char> sp{buf};
    LEARN_CHECK(sp[0] == 'o');
}

void demo_intermediate() {
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    std::array<char, 32> buf{};
    std::span<char> sp{buf};
    std::ospanstream os{sp};
    os << "hi";
    LEARN_CHECK(std::string(buf.data()) == "hi" || os.span().size() >= 2);
#else
    std::array<char, 8> buf{'h', 'i', '\0'};
    LEARN_CHECK(std::string(buf.data()) == "hi");
#endif
}

void demo_expert() {
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    std::array<char, 16> buf{'4', '2', '\0'};
    std::ispanstream is{std::span<const char>{buf.data(), 2}};
    int v = 0;
    is >> v;
    LEARN_CHECK(v == 42);
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/spanstream_cpp23", run>;

}  // namespace
