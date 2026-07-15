// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_filesystem_cpp17
// Topic id : part2/stage10/section11_other_stl/std_filesystem_cpp17
//
// Covers: std::filesystem path status directory

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace {

void demo_basics() {
    fs::path p = "foo/bar.txt";
    LEARN_CHECK(p.filename() == "bar.txt");
    LEARN_CHECK(p.extension() == ".txt");
    LEARN_CHECK(p.stem() == "bar");
}

void demo_intermediate() {
    fs::path a = "dir";
    fs::path b = "file.cpp";
    auto c = a / b;
    LEARN_CHECK(c.filename() == "file.cpp");
    LEARN_CHECK(fs::path("/a/b/../c").lexically_normal().filename() == "c" ||
                !fs::path("/a/b/../c").lexically_normal().empty());
}

void demo_expert() {
    const auto tmp = fs::temp_directory_path();
    LEARN_CHECK(!tmp.empty());
    const auto file = tmp / "learncpp_stage10_fs_demo.txt";
    {
        std::ofstream out(file);
        out << "ok";
    }
    LEARN_CHECK(fs::exists(file));
    LEARN_CHECK(fs::is_regular_file(file));
    LEARN_CHECK(fs::file_size(file) == 2);
    fs::remove(file);
    LEARN_CHECK(!fs::exists(file));
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11_other_stl/std_filesystem_cpp17", run>;

}  // namespace
