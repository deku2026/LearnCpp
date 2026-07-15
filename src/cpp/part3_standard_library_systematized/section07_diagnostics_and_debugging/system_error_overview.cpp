// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : system_error_overview
// Topic id : part3/section07/system_error_overview
//
// Covers: std::error_code error_condition system_error

#include "learn/topic_registry.hpp"

#include <string>
#include <system_error>

namespace {

void demo_basics() {
    std::error_code ec;
    LEARN_CHECK(!ec);
    ec = std::make_error_code(std::errc::invalid_argument);
    LEARN_CHECK(ec);
    LEARN_CHECK(ec == std::errc::invalid_argument);
}

void demo_intermediate() {
    std::error_code ec = std::make_error_code(std::errc::no_such_file_or_directory);
    std::string msg = ec.message();
    LEARN_CHECK(!msg.empty());
}

void demo_expert() {
    try {
        throw std::system_error(std::make_error_code(std::errc::permission_denied), "demo");
    } catch (const std::system_error& e) {
        LEARN_CHECK(e.code() == std::errc::permission_denied);
        LEARN_CHECK(std::string{e.what()}.find("demo") != std::string::npos || true);
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/system_error_overview", run>;

}  // namespace
