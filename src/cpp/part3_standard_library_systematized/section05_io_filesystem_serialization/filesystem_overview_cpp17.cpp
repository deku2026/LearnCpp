// Topic    : <filesystem> 路径 / 查询 / 遍历 / 增删
// Doc      : 第3部分-标准库系统化.md · 库 5.3
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : filesystem_overview_cpp17
// Topic id : part3/section05/filesystem_overview_cpp17
// Refs     : https://en.cppreference.com/w/cpp/filesystem
//            https://en.cppreference.com/w/cpp/filesystem/path

#include "learn/topic_registry.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

namespace fs = std::filesystem;

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [filesystem_overview_cpp17] path 组成 ===\n";
    {
        fs::path p = fs::path("dir") / "sub" / "file.cpp";
        std::cout << "native: " << p.string() << '\n';
        std::cout << "filename=" << p.filename().string() << " stem=" << p.stem().string()
                  << " ext=" << p.extension().string() << '\n';
        assert(p.filename() == "file.cpp");
        assert(p.extension() == ".cpp");
        // generic 用 '/'，跨平台比较更友好
        assert(p.generic_string().find("dir/sub/file.cpp") != std::string::npos ||
               p.string().find("file.cpp") != std::string::npos);
    }

    const fs::path root = fs::temp_directory_path() / "learncpp_fs_overview";
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "nested");

    const fs::path f1 = root / "a.txt";
    const fs::path f2 = root / "nested" / "b.cpp";
    {
        std::ofstream(f1) << "hello";
        std::ofstream(f2) << "int main(){}\n";
    }

    std::cout << "=== exists / size / is_* ===\n";
    assert(fs::exists(f1));
    assert(fs::is_regular_file(f1));
    assert(fs::is_directory(root / "nested"));
    assert(fs::file_size(f1) == 5);
    std::cout << "a.txt size=" << fs::file_size(f1) << '\n';

    std::cout << "=== directory_iterator ===\n";
    {
        std::vector<std::string> names;
        for (const auto& entry : fs::directory_iterator(root)) {
            names.push_back(entry.path().filename().string());
        }
        assert(!names.empty());
        std::cout << "top-level entries=" << names.size() << '\n';
        for (const auto& n : names) {
            std::cout << "  " << n << '\n';
        }
    }

    std::cout << "=== recursive_directory_iterator ===\n";
    {
        int files = 0;
        for (const auto& entry : fs::recursive_directory_iterator(root)) {
            if (entry.is_regular_file()) {
                ++files;
            }
        }
        assert(files == 2);
        std::cout << "recursive regular files=" << files << '\n';
    }

    std::cout << "=== absolute / relative / rename / copy ===\n";
    {
        const auto abs = fs::absolute(f1);
        assert(abs.is_absolute());
        std::cout << "absolute: " << abs.string() << '\n';

        const fs::path copied = root / "a_copy.txt";
        fs::copy_file(f1, copied, fs::copy_options::overwrite_existing);
        assert(fs::exists(copied));

        const fs::path renamed = root / "a_renamed.txt";
        fs::rename(copied, renamed);
        assert(fs::exists(renamed) && !fs::exists(copied));
    }

    std::cout << "=== error_code 不抛 vs 异常路径 ===\n";
    {
        std::error_code lec;
        const auto sz = fs::file_size(root / "missing.zzz", lec);
        assert(lec);  // 应有错误
        (void)sz;
        std::cout << "file_size missing: " << lec.message() << '\n';

        try {
            (void)fs::file_size(root / "missing.zzz");
            assert(false && "should throw");
        } catch (const fs::filesystem_error& ex) {
            std::cout << "caught filesystem_error: " << ex.what() << '\n';
        }
    }

    fs::remove_all(root, ec);
    std::cout << "[filesystem_overview_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/filesystem_overview_cpp17", run>;

}  // namespace
