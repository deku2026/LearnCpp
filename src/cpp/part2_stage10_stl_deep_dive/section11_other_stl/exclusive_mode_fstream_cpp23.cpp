// Topic    : C++23 fstream 独占创建(ios::noreplace ≈ fopen "x")
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : exclusive_mode_fstream_cpp23
// Topic id : part2/stage10/section11/exclusive_mode_fstream_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/ios_base/openmode
//            https://en.cppreference.com/w/cpp/io/basic_ofstream/open
//            Feature: ios_base::noreplace (C++23)

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [exclusive_mode_fstream_cpp23] ios::noreplace ===\n";

    namespace fs = std::filesystem;
    std::error_code ec;
    const fs::path dir = fs::temp_directory_path(ec) / "learncpp_noreplace_demo";
    fs::create_directories(dir, ec);
    const fs::path file = dir / "exclusive_target.txt";
    fs::remove(file, ec);  // 起始保证不存在

    // ① 独占创建: 文件不存在 → 成功
    {
        std::ofstream out(file, std::ios::out | std::ios::noreplace);
        assert(out.good());
        out << "first-writer\n";
        out.close();
        assert(fs::exists(file));
        std::cout << "  create exclusive: OK\n";
    }

    // ② 文件已存在 → noreplace 失败(不截断、不覆盖)
    {
        std::ofstream out(file, std::ios::out | std::ios::noreplace);
        assert(!out.good());
        assert(out.fail());
        std::cout << "  second exclusive open fails (exists)\n";
    }

    // ③ 对照: trunc 可覆盖
    {
        std::ofstream out(file, std::ios::out | std::ios::trunc);
        assert(out.good());
        out << "overwritten\n";
        out.close();

        std::ifstream in(file);
        std::string line;
        std::getline(in, line);
        assert(line == "overwritten");
        std::cout << "  trunc overwrite still works\n";
    }

    fs::remove(file, ec);
    fs::remove(dir, ec);

    // 语义: 类似 C fopen(..., "wx") 独占创建
    std::cout << "  noreplace ≈ fopen \"x\" exclusive create\n";
    std::cout << "[exclusive_mode_fstream_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/exclusive_mode_fstream_cpp23", run>;

}  // namespace
