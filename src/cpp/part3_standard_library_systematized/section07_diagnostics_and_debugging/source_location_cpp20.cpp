// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : source_location_cpp20
// Topic id : part3/section07/source_location_cpp20
//
// Covers: std::source_location current file/line/function

#include "learn/topic_registry.hpp"

#include <source_location>
#include <string_view>

namespace {

void demo_basics() {
    const auto loc = std::source_location::current();
    LEARN_CHECK(loc.line() > 0);
    LEARN_CHECK(std::string_view{loc.file_name()}.size() > 0);
}

void demo_intermediate() {
    auto where = [](const std::source_location loc = std::source_location::current()) { return loc.function_name(); };
    LEARN_CHECK(std::string_view{where()}.size() > 0);
}

void demo_expert() {
    const auto loc = std::source_location::current();
    LEARN_CHECK(loc.column() >= 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/source_location_cpp20", run>;

}  // namespace
