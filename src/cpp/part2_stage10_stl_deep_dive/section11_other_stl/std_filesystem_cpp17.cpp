// Topic    : <filesystem> path / 存在性 / 目录迭代 / error_code 双接口
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : std_filesystem_cpp17
// Topic id : part2/stage10/section11/std_filesystem_cpp17
// Refs     : https://en.cppreference.com/w/cpp/filesystem
//            https://en.cppreference.com/w/cpp/filesystem/path
//            https://en.cppreference.com/w/cpp/filesystem/directory_iterator

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    namespace fs = std::filesystem;
    std::cout << "=== [std_filesystem_cpp17] path + directory ops ===\n";

    // ① path 组合与分解
    {
        const fs::path p = fs::path{"docs"} / "guide" / "intro.md";
        assert(p.filename() == "intro.md");
        assert(p.stem() == "intro");
        assert(p.extension() == ".md");
        assert(p.parent_path() == fs::path{"docs"} / "guide");
        std::cout << "  path: " << p.generic_string() << '\n';
    }

    // ② 临时目录: create / write / iterate / remove (error_code 版)
    {
        std::error_code ec;
        const fs::path root = fs::temp_directory_path(ec) / "learncpp_fs_demo";
        assert(!ec);
        fs::remove_all(root, ec);
        fs::create_directories(root / "sub", ec);
        assert(!ec);
        assert(fs::is_directory(root, ec));

        const fs::path file = root / "note.txt";
        {
            std::ofstream out(file);
            out << "hello-fs\n";
        }
        assert(fs::exists(file, ec));
        assert(fs::is_regular_file(file, ec));
        assert(fs::file_size(file, ec) > 0);

        std::vector<std::string> names;
        for (const fs::directory_entry& ent : fs::directory_iterator{root, ec}) {
            names.push_back(ent.path().filename().string());
        }
        assert(!ec);
        // 应含 sub 与 note.txt
        assert(std::find(names.begin(), names.end(), "note.txt") != names.end());
        assert(std::find(names.begin(), names.end(), "sub") != names.end());
        std::cout << "  directory_iterator count=" << names.size() << '\n';

        // ③ 抛异常版 API: 不存在路径
        bool threw = false;
        try {
            (void)fs::file_size(root / "missing.bin");
        } catch (const fs::filesystem_error& ex) {
            threw = true;
            std::cout << "  throwing API: " << ex.code().message() << '\n';
        }
        assert(threw);

        // error_code 版不抛
        ec.clear();
        const auto sz = fs::file_size(root / "missing.bin", ec);
        assert(ec);
        assert(sz == static_cast<std::uintmax_t>(-1) || sz == 0 || ec);
        std::cout << "  error_code API sets ec without throw\n";

        fs::remove_all(root, ec);
    }

    // ④ absolute / weakly_canonical 精神(不强制特定盘符)
    {
        std::error_code ec;
        const fs::path rel{"."};
        const fs::path abs = fs::absolute(rel, ec);
        assert(!ec);
        assert(abs.is_absolute());
        std::cout << "  absolute(.): " << abs.string() << '\n';
    }

    std::cout << "[std_filesystem_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/std_filesystem_cpp17", run>;

}  // namespace
