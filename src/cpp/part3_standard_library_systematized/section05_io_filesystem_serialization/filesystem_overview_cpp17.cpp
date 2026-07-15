// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : filesystem_overview_cpp17
// Topic id : part3/section05/filesystem_overview_cpp17
//
// Covers: std::filesystem path status directory ops

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace {

void demo_basics() {
    std::filesystem::path p = "dir/file.txt";
    LEARN_CHECK(p.filename() == "file.txt");
    LEARN_CHECK(p.extension() == ".txt");
}

void demo_intermediate() {
    auto dir = std::filesystem::temp_directory_path() / "learncpp_fs_demo";
    std::filesystem::create_directories(dir);
    LEARN_CHECK(std::filesystem::is_directory(dir));
    auto file = dir / "a.txt";
    {
        std::ofstream out(file);
        out << "x";
    }
    LEARN_CHECK(std::filesystem::exists(file));
    std::filesystem::remove_all(dir);
}

void demo_expert() {
    std::filesystem::path p = "a/b/../c";
    auto n = p.lexically_normal();
    LEARN_CHECK(n.filename() == "c" || n.generic_string().find('c') != std::string::npos);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/filesystem_overview_cpp17", run>;

}  // namespace
