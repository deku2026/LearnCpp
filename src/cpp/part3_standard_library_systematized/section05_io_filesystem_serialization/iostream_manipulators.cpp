// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_manipulators
// Topic id : part3/section05/iostream_manipulators
//
// Covers: iostream manipulators boolalpha hex setw setprecision

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>

namespace {

void demo_basics() {
    std::ostringstream os;
    os << std::boolalpha << true;
    assert(os.str() == "true");
}

void demo_intermediate() {
    std::ostringstream os;
    os << std::hex << 255;
    assert(os.str() == "ff");
}

void demo_expert() {
    std::ostringstream os;
    os << std::setw(5) << std::setfill('0') << 42;
    assert(os.str() == "00042");
    std::ostringstream os2;
    os2 << std::fixed << std::setprecision(2) << 3.14159;
    assert(os2.str() == "3.14");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_manipulators", run>;

}  // namespace
