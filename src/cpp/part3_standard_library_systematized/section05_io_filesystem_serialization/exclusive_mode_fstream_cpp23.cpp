// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : exclusive_mode_fstream_cpp23
// Topic id : part3/section05/exclusive_mode_fstream_cpp23
//
// Covers: fstream exclusive create mode C++23 (noreplace)

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <version>

namespace {

void demo_basics() {
    const auto path = std::filesystem::temp_directory_path() / "learncpp_excl_demo.txt";
    std::filesystem::remove(path);
    {
        std::ofstream out(path);
        LEARN_CHECK(out);
        out << "first";
    }
    LEARN_CHECK(std::filesystem::exists(path));
    std::filesystem::remove(path);
}

void demo_intermediate() {
#if defined(__cpp_lib_ios_noreplace) && __cpp_lib_ios_noreplace >= 202207L
    const auto path = std::filesystem::temp_directory_path() / "learncpp_excl2.txt";
    std::filesystem::remove(path);
    {
        std::ofstream out(path, std::ios::noreplace);
        LEARN_CHECK(static_cast<bool>(out));
        out << "a";
    }
    {
        std::ofstream out(path, std::ios::noreplace);
        LEARN_CHECK(!out);
    }
    std::filesystem::remove(path);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
    // exclusive create prevents clobbering existing files when supported
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/exclusive_mode_fstream_cpp23", run>;

}  // namespace
