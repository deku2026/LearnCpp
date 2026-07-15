// Topic    : fstream 独占打开 ios::noreplace（C++23，防覆盖）
// Doc      : 第3部分-标准库系统化.md · 库 5.2 noreplace
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : exclusive_mode_fstream_cpp23
// Topic id : part3/section05/exclusive_mode_fstream_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/ios_base/openmode
//            P2467；__cpp_lib_ios_noreplace

#include "learn/topic_registry.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace fs = std::filesystem;

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [exclusive_mode_fstream_cpp23] ===\n";

    const fs::path dir = fs::temp_directory_path() / "learncpp_noreplace";
    fs::create_directories(dir);
    const fs::path path = dir / "exclusive.txt";
    std::error_code ec;
    fs::remove(path, ec);

#if defined(__cpp_lib_ios_noreplace) && __cpp_lib_ios_noreplace >= 202207L
    std::cout << "__cpp_lib_ios_noreplace=" << __cpp_lib_ios_noreplace << '\n';

    // 第一次：文件不存在 → 创建成功
    {
        std::ofstream out(path, std::ios::out | std::ios::noreplace);
        assert(static_cast<bool>(out));
        out << "first-writer\n";
    }
    assert(fs::exists(path));
    std::cout << "first exclusive create: ok\n";

    // 第二次：文件已存在 → noreplace 必须失败（不截断/覆盖）
    {
        std::ofstream out(path, std::ios::out | std::ios::noreplace);
        assert(!out);
        std::cout << "second exclusive open fail()=" << out.fail() << " (expected)\n";
    }

    // 确认内容未被覆盖
    {
        std::ifstream in(path);
        std::string line;
        std::getline(in, line);
        assert(line == "first-writer");
        std::cout << "content preserved: " << line << '\n';
    }
#else
    std::cout << "__cpp_lib_ios_noreplace missing; emulate exclusive create\n";
    // 可移植模拟：先探测存在性再创建（有 TOCTOU，仅教学）
    assert(!fs::exists(path));
    {
        std::ofstream out(path);
        assert(out);
        out << "first-writer\n";
    }
    if (fs::exists(path)) {
        std::cout << "file exists → refuse open (emulated noreplace)\n";
    }
    {
        std::ifstream in(path);
        std::string line;
        std::getline(in, line);
        assert(line == "first-writer");
    }
#endif

    fs::remove_all(dir, ec);
    std::cout << "use case: crash-safe config write, avoid clobbering secrets\n";
    std::cout << "[exclusive_mode_fstream_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/exclusive_mode_fstream_cpp23", run>;

}  // namespace
