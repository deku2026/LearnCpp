// Topic    : 内存映射文件（标准库无 mmap；可运行探测 + 边界说明）
// Doc      : 第3部分-标准库系统化.md · 库 5 扩展笔记
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : memory_mapped_files_note
// Topic id : part3/section05/memory_mapped_files_note
// Refs     : POSIX mmap / Win32 MapViewOfFile；标准侧用 fstream/filesystem 对照

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

namespace {

// 标准库【没有】mmap API。本 topic 用“整文件读入 vector”模拟只读映射的使用面，
// 并打印平台相关入口，避免空讲。
std::vector<char> read_file_bytes(const fs::path& p) {
    std::ifstream in(p, std::ios::binary);
    assert(in);
    return std::vector<char>(std::istreambuf_iterator<char>(in), {});
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [memory_mapped_files_note] 标准库边界 ===\n";
    std::cout << "C++ standard: NO portable memory-map API\n";
    std::cout << "POSIX : mmap / munmap / madvise\n";
    std::cout << "Win32 : CreateFileMapping / MapViewOfFile\n";
    std::cout << "boost : boost::iostreams::mapped_file\n";

    const fs::path dir = fs::temp_directory_path() / "learncpp_mmap_note";
    fs::create_directories(dir);
    const fs::path path = dir / "blob.bin";

    // 写一块“像映射源”的数据
    {
        std::ofstream out(path, std::ios::binary);
        const char payload[] = "MMAP-PROBE\x00\x01\x02";
        out.write(payload, static_cast<std::streamsize>(sizeof(payload) - 1));
    }

    std::cout << "=== 模拟只读映射：一次性读入 + 零拷贝视图 ===\n";
    {
        const auto bytes = read_file_bytes(path);
        assert(bytes.size() >= 10);
        // string_view 风格“映射视图”（数据在堆 vector，不是 OS 页映射）
        const std::string_view view{bytes.data(), bytes.size()};
        assert(view.starts_with("MMAP-PROBE"));
        std::cout << "loaded bytes=" << bytes.size() << " prefix=" << view.substr(0, 10) << '\n';

        // 真实 mmap 的价值：巨大文件不必读进进程堆；多进程共享；按需缺页
        std::cout << "real mmap benefits: huge files, shared pages, demand paging\n";
        std::cout << "costs: alignment, page size, sync (msync/FlushViewOfFile), "
                     "signal/Windows structured exceptions on faults\n";
    }

    std::cout << "=== 与 fstream 选型 ===\n";
    std::cout << "small/medium files: fstream / filesystem 足够\n";
    std::cout << "large random access / IPC: platform mmap or library\n";

    std::error_code ec;
    fs::remove_all(dir, ec);
    std::cout << "[memory_mapped_files_note] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/memory_mapped_files_note", run>;

}  // namespace
