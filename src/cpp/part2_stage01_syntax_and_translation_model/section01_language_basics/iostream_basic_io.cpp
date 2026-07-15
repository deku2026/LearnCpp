// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : iostream_basic_io
// Topic id : part2/stage01/section01/iostream_basic_io
//
// Covers: cout/cerr, endl vs \\n, optional cin skip, std::print when available

#include "learn/topic_registry.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <version>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#endif

namespace {

void demo_basics() {
    std::ostringstream out;
    out << "Hello" << ' ' << 42 << '\n';
    LEARN_CHECK(out.str() == "Hello 42\n");

    std::cerr << "[iostream_basic_io] demo_basics ok\n";
}

void demo_intermediate() {
    // '\\n' inserts a newline; endl inserts newline and flushes.
    std::ostringstream a;
    a << "line\n";
    LEARN_CHECK(a.str() == "line\n");

    std::ostringstream b;
    b << "line" << std::endl;
    LEARN_CHECK(b.str() == "line\n");

    // Prefer '\\n' when flush is not required; use endl / flush when needed.
    std::cout << "[iostream_basic_io] prefer \\n over endl when flush is unnecessary\n";

    // Optional cin: only exercise extraction on a stringstream (no interactive wait).
    std::istringstream in{"7 widgets"};
    int n = 0;
    std::string word;
    in >> n >> word;
    LEARN_CHECK(n == 7);
    LEARN_CHECK(word == "widgets");
    LEARN_CHECK(static_cast<bool>(in));
}

void demo_expert() {
    std::ostringstream oss;
    oss << std::boolalpha << true << ' ' << std::noboolalpha << false;
    LEARN_CHECK(oss.str() == "true 0");

    // Unitbuf / flush: cerr is typically unitbuf; cout is not.
    std::cerr << "[iostream_basic_io] cerr path\n";

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    std::print("[iostream_basic_io] std::print available ({})\n", 1);
    std::println("[iostream_basic_io] std::println ok");
#else
    std::cout << "[iostream_basic_io] std::print not available; using iostream only\n";
#endif

    // Tied streams: cin is tied to cout by default so prompts flush before input.
    LEARN_CHECK(std::cin.tie() == &std::cout);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/iostream_basic_io", run>;

}  // namespace
