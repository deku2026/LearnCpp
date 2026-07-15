// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : diagnostics_headers
// Topic id : part3/section08/diagnostics_headers
//
// Covers: headers map: <cassert> <stdexcept> <system_error> <source_location> <stacktrace>

#include "learn/topic_registry.hpp"

#include <source_location>
#include <stdexcept>
#include <system_error>
#include <version>

namespace {

void demo_basics() {
    LEARN_CHECK(true);
    try {
        throw std::runtime_error("x");
    } catch (const std::exception&) {
    }
}

void demo_intermediate() {
    std::error_code ec = std::make_error_code(std::errc::timed_out);
    LEARN_CHECK(ec);
}

void demo_expert() {
    const auto loc = std::source_location::current();
    LEARN_CHECK(loc.line() > 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/diagnostics_headers", run>;

}  // namespace
