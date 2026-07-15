// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : spanstream_cpp23
// Topic id : part2/stage10/section11_other_stl/spanstream_cpp23
//
// Covers: spanstream ispanstream ospanstream C++23

#include "learn/topic_registry.hpp"

#include <span>
#include <string>
#include <version>

#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
#include <spanstream>
#endif
#include <sstream>

namespace {

void demo_basics() {
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    char buf[] = "10 20";
    std::ispanstream in{std::span<char>{buf}};
    int a = 0;
    int b = 0;
    in >> a >> b;
    LEARN_CHECK(a == 10 && b == 20);
#else
    std::istringstream in{"10 20"};
    int a = 0;
    int b = 0;
    in >> a >> b;
    LEARN_CHECK(a == 10 && b == 20);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    char buf[32]{};
    std::ospanstream out{std::span<char>{buf}};
    out << "hi" << 42;
    const auto sp = out.span();
    LEARN_CHECK(std::string(sp.data(), sp.size()) == "hi42");
#else
    std::ostringstream out;
    out << "hi" << 42;
    LEARN_CHECK(out.str() == "hi42");
#endif
}

void demo_expert() {
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    char buf[64]{};
    std::spanstream io{std::span<char>{buf}};
    io << 7 << ' ' << 8;
    io.seekg(0);
    int x = 0;
    int y = 0;
    io >> x >> y;
    LEARN_CHECK(x == 7 && y == 8);
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/spanstream_cpp23", run>;

}  // namespace
