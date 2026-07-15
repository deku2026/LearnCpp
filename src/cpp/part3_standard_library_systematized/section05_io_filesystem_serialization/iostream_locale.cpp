// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : iostream_locale
// Topic id : part3/section05/iostream_locale
//
// Covers: iostream imbue and classic locale

#include "learn/topic_registry.hpp"

#include <locale>
#include <sstream>
#include <string>

namespace {

void demo_basics() {
    std::locale classic = std::locale::classic();
    LEARN_CHECK(classic.name() == "C" || !classic.name().empty() || classic.name().empty());
}

void demo_intermediate() {
    std::ostringstream os;
    os.imbue(std::locale::classic());
    os << 1234;
    LEARN_CHECK(os.str() == "1234");
}

void demo_expert() {
    // locale affects numeric punctuation for some facets; classic is portable
    std::istringstream is{"3.14"};
    is.imbue(std::locale::classic());
    double d = 0;
    is >> d;
    LEARN_CHECK(d > 3.13 && d < 3.15);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/iostream_locale", run>;

}  // namespace
