// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_state_flags
// Topic id : part3/section05/iostream_state_flags
//
// Covers: rdstate good/fail/eof/bad and clear

#include "learn/topic_registry.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

void demo_basics() {
    std::istringstream is{"42"};
    int x = 0;
    is >> x;
    // Extraction may set eofbit at end of stream; good() is false then, fail() is not.
    assert(!is.fail());
    assert(x == 42);
}

void demo_intermediate() {
    std::istringstream is{"x"};
    int x = 0;
    is >> x;
    assert(is.fail());
    is.clear();
    assert(!is.fail());
}

void demo_expert() {
    std::istringstream is{""};
    char c = 0;
    is >> c;
    assert(is.eof() || is.fail());
    assert((is.rdstate() & std::ios_base::failbit) != 0 || is.eof());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_state_flags", run>;

}  // namespace
