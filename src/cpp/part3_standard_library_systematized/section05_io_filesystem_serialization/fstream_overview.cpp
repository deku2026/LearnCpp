// Topic    : ifstream / ofstream / fstream 文件流（RAII）
// Doc      : 第3部分-标准库系统化.md · 库 5.2
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : fstream_overview
// Topic id : part3/section05/fstream_overview
// Refs     : https://en.cppreference.com/w/cpp/io/basic_fstream
//            https://en.cppreference.com/w/cpp/io/basic_ofstream

#include "learn/topic_registry.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [fstream_overview] 写 + 读 + 二进制 ===\n";

    const fs::path dir = fs::temp_directory_path() / "learncpp_fstream_overview";
    fs::create_directories(dir);
    const fs::path text_path = dir / "demo.txt";
    const fs::path bin_path = dir / "demo.bin";

    // 文本写（RAII：作用域结束自动 close）
    {
        std::ofstream out(text_path);
        assert(out);
        out << "line 1\n" << "line 2\n" << 42 << '\n';
    }

    // 逐行读
    {
        std::ifstream in(text_path);
        assert(in);
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(in, line)) {
            lines.push_back(line);
        }
        assert(lines.size() == 3);
        assert(lines[0] == "line 1");
        assert(lines[2] == "42");
        std::cout << "text lines=" << lines.size() << " last=" << lines.back() << '\n';
    }

    // 二进制：ios::binary 避免 Windows 换行翻译
    {
        const std::vector<unsigned char> payload{0x00, 0x0A, 0xFF, 0x0D};
        {
            std::ofstream out(bin_path, std::ios::binary | std::ios::trunc);
            assert(out);
            out.write(reinterpret_cast<const char*>(payload.data()), static_cast<std::streamsize>(payload.size()));
        }
        std::ifstream in(bin_path, std::ios::binary);
        assert(in);
        std::vector<unsigned char> got(payload.size());
        in.read(reinterpret_cast<char*>(got.data()), static_cast<std::streamsize>(got.size()));
        assert(in.gcount() == static_cast<std::streamsize>(payload.size()));
        assert(got == payload);
        std::cout << "binary round-trip bytes=" << got.size() << '\n';
    }

    // app 追加
    {
        {
            std::ofstream out(text_path, std::ios::app);
            out << "appended\n";
        }
        std::ifstream in(text_path);
        std::string all((std::istreambuf_iterator<char>(in)), {});
        assert(all.find("appended") != std::string::npos);
        std::cout << "append ok, size=" << all.size() << '\n';
    }

    // 打开失败
    {
        std::ifstream missing(dir / "no_such_file_zzz.txt");
        assert(!missing);
        std::cout << "missing file: fail()=" << missing.fail() << '\n';
    }

    std::error_code ec;
    fs::remove_all(dir, ec);
    std::cout << "[fstream_overview] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/fstream_overview", run>;

}  // namespace
