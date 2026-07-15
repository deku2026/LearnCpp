// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : exclusive_mode_fstream_cpp23
// Topic id : part2/stage10/section11_other_stl/exclusive_mode_fstream_cpp23
//
// Covers: fstream exclusive create mode C++23 noreplace

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <version>

namespace fs = std::filesystem;

namespace {

void demo_basics() {
    const auto path = fs::temp_directory_path() / "learncpp_excl_demo_a.txt";
    fs::remove(path);
    {
        std::ofstream out(path);
        out << "x";
    }
    LEARN_CHECK(fs::exists(path));
    fs::remove(path);
}

void demo_intermediate() {
#if defined(__cpp_lib_ios_noreplace) && __cpp_lib_ios_noreplace >= 202207L
    const auto path = fs::temp_directory_path() / "learncpp_excl_demo_b.txt";
    fs::remove(path);
    {
        std::ofstream out(path, std::ios::out | std::ios::noreplace);
        LEARN_CHECK(static_cast<bool>(out));
        out << "first";
    }
    {
        std::ofstream out2(path, std::ios::out | std::ios::noreplace);
        LEARN_CHECK(!out2);  // exclusive create fails if exists
    }
    fs::remove(path);
#else
    // Portable approximation: fail if exists
    const auto path = fs::temp_directory_path() / "learncpp_excl_demo_b.txt";
    fs::remove(path);
    LEARN_CHECK(!fs::exists(path));
    {
        std::ofstream out(path);
        out << "first";
    }
    LEARN_CHECK(fs::exists(path));
    fs::remove(path);
#endif
}

void demo_expert() {
    const auto path = fs::temp_directory_path() / "learncpp_excl_demo_c.txt";
    fs::remove(path);
    std::ofstream out(path, std::ios::binary);
    out.write("OK", 2);
    out.close();
    LEARN_CHECK(fs::file_size(path) == 2);
    fs::remove(path);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/exclusive_mode_fstream_cpp23", run>;

}  // namespace
