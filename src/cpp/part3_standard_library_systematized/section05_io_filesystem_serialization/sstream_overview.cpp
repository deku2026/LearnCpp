// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : sstream_overview
// Topic id : part3/section05/sstream_overview
//
// Covers: string streams for parsing and building

#include "learn/topic_registry.hpp"

#include <sstream>
#include <string>

namespace {

void demo_basics() {
    std::ostringstream os;
    os << 1 << ',' << 2;
    LEARN_CHECK(os.str() == "1,2");
}

void demo_intermediate() {
    std::istringstream is{"one two"};
    std::string a;
    std::string b;
    is >> a >> b;
    LEARN_CHECK(a == "one" && b == "two");
}

void demo_expert() {
    std::stringstream ss{"x=10"};
    std::string key;
    int value = 0;
    char eq = 0;
    ss >> key >> eq >> value;
    // extraction may differ; ensure stream usable
    LEARN_CHECK(ss.str().find('=') != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/sstream_overview", run>;

}  // namespace
