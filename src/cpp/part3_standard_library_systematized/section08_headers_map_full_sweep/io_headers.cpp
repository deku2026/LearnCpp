// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : io_headers
// Topic id : part3/section08/io_headers
//
// Covers: headers map: iostream fstream sstream filesystem iomanip print

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void demo_basics() {
    std::ostringstream os;
    os << std::setw(2) << 7;
    LEARN_CHECK(!os.str().empty());
}

void demo_intermediate() {
    auto p = std::filesystem::path{"a/b"};
    LEARN_CHECK(p.filename() == "b");
}

void demo_expert() {
    std::stringstream ss{"1"};
    int x = 0;
    ss >> x;
    LEARN_CHECK(x == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/io_headers", run>;

}  // namespace
