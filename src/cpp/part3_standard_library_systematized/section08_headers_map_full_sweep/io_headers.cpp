// Topic    : headers map · 输入输出 IO
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : io_headers
// Topic id : part3/section08/io_headers
// Headers  : iostream/fstream/sstream/spanstream/iomanip/streambuf
//            syncstream/filesystem/print

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <syncstream>
#include <system_error>

#if defined(__has_include)
#if __has_include(<spanstream>)
#include <spanstream>
#define LEARN_HAS_SPANSTREAM 1
#endif
#if __has_include(<print>)
#include <print>
#define LEARN_HAS_PRINT 1
#endif
#endif
#ifndef LEARN_HAS_SPANSTREAM
#define LEARN_HAS_SPANSTREAM 0
#endif
#ifndef LEARN_HAS_PRINT
#define LEARN_HAS_PRINT 0
#endif

namespace fs = std::filesystem;

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [io_headers] map probe ===\n";
    std::cout << "<iostream>/<istream>/<ostream>  standard streams\n";
    std::cout << "<fstream>   file streams (+ noreplace C++23)\n";
    std::cout << "<sstream>   string streams\n";
    std::cout << "<spanstream> C++23 fixed-buffer streams\n";
    std::cout << "<iomanip>   manipulators\n";
    std::cout << "<streambuf> buffer abstraction\n";
    std::cout << "<syncstream> C++20 synchronized ostream\n";
    std::cout << "<filesystem> C++17 paths\n";
    std::cout << "<print>      C++23 formatted output\n";

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << 7;
    assert(oss.str() == "0007");
    std::cout << "iomanip: " << oss.str() << '\n';

    std::osyncstream{std::cout} << "syncstream line\n";

    const fs::path p = fs::temp_directory_path() / "learncpp_io_headers.txt";
    {
        std::ofstream out(p);
        out << "fs+fstream";
    }
    {
        std::ifstream in(p);
        std::string s;
        std::getline(in, s);
        assert(s == "fs+fstream");
        std::cout << "fstream+filesystem: " << s << '\n';
    }
    std::error_code ec;
    fs::remove(p, ec);

#if LEARN_HAS_SPANSTREAM
    char buf[32]{};
    std::ospanstream ossp{std::span<char>{buf}};
    ossp << "sp";
    assert(ossp.span().size() == 2);
    std::cout << "spanstream ok\n";
#else
    std::cout << "spanstream: not available\n";
#endif

#if LEARN_HAS_PRINT
    std::println("print probe {}", 1);
#else
    std::cout << "print: not available\n";
#endif

    std::cout << "[io_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/io_headers", run>;

}  // namespace
