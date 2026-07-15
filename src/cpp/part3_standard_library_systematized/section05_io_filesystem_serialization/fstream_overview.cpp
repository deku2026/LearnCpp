// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : fstream_overview
// Topic id : part3/section05/fstream_overview
//
// Covers: fstream write/read round-trip

#include "learn/topic_registry.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

namespace {

void demo_basics() {
    const auto path = std::filesystem::temp_directory_path() / "learncpp_fstream_demo.txt";
    {
        std::ofstream out(path);
        assert(out);
        out << "hello";
    }
    {
        std::ifstream in(path);
        std::string s;
        in >> s;
        assert(s == "hello");
    }
    std::filesystem::remove(path);
}

void demo_intermediate() {
    const auto path = std::filesystem::temp_directory_path() / "learncpp_fstream_bin.bin";
    {
        std::ofstream out(path, std::ios::binary);
        const char data[] = {'A', 'B', 'C'};
        out.write(data, 3);
    }
    {
        std::ifstream in(path, std::ios::binary);
        char buf[3]{};
        in.read(buf, 3);
        assert(buf[0] == 'A' && buf[2] == 'C');
    }
    std::filesystem::remove(path);
}

void demo_expert() {
    const auto path = std::filesystem::temp_directory_path() / "learncpp_fstream_app.txt";
    {
        std::ofstream out(path);
        out << "a";
    }
    {
        std::ofstream out(path, std::ios::app);
        out << "b";
    }
    {
        std::ifstream in(path);
        std::string s;
        std::getline(in, s);
        assert(s == "ab");
    }
    std::filesystem::remove(path);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/fstream_overview", run>;

}  // namespace
