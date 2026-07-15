// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_overview
// Topic id : part3/section05/iostream_overview
//
// Covers: iostream basic extraction/insertion

#include "learn/topic_registry.hpp"

#include <sstream>
#include <string>

namespace {

void demo_basics() {
    std::ostringstream os;
    os << "x=" << 1;
    LEARN_CHECK(os.str() == "x=1");
}

void demo_intermediate() {
    std::istringstream is{"10 20"};
    int a = 0;
    int b = 0;
    is >> a >> b;
    LEARN_CHECK(a == 10 && b == 20);
}

void demo_expert() {
    std::stringstream ss;
    ss << 3.5;
    double d = 0;
    ss >> d;
    LEARN_CHECK(d > 3.4 && d < 3.6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_overview", run>;

}  // namespace
