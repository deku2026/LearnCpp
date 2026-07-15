// Topic     : RAII 资源句柄
// Doc       : 第2部分-阶段4 · 步骤 2.1–2.3
// cppreference: https://en.cppreference.com/cpp/language/raii
//
// 要点: 构造获取资源、析构释放；析构不抛异常；独占资源删拷贝、提供移动；
//       作用域结束（含异常路径）自动释放。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

class FileHandle {
    std::FILE* fp_ = nullptr;

public:
    explicit FileHandle(const char* path, const char* mode) {
        fp_ = std::fopen(path, mode);
        if (!fp_) {
            throw std::runtime_error(std::string("open failed: ") + path);
        }
    }

    ~FileHandle() {
        if (fp_) {
            std::fclose(fp_);
        }
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept : fp_(std::exchange(other.fp_, nullptr)) {}

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (fp_) {
                std::fclose(fp_);
            }
            fp_ = std::exchange(other.fp_, nullptr);
        }
        return *this;
    }

    std::FILE* get() const { return fp_; }

    void write_line(const char* s) {
        assert(fp_);
        std::fputs(s, fp_);
        std::fputc('\n', fp_);
    }
};

// 教学用：计数「打开中」资源，验证析构配对
class CountedResource {
    static inline int live_ = 0;
    int id_;

public:
    explicit CountedResource(int id) : id_(id) { ++live_; }
    ~CountedResource() { --live_; }

    CountedResource(const CountedResource&) = delete;
    CountedResource& operator=(const CountedResource&) = delete;

    CountedResource(CountedResource&& o) noexcept : id_(std::exchange(o.id_, -1)) {
        // 所有权转移：live_ 不变（源不再负责释放计数——此处简化：移动不增减）
        // 为清晰起见本类禁止「空源析构减计数」歧义：源 id_=-1 时析构不减
    }

    // 上面移动语义与 live_ 简化冲突，改为更干净的实现：
    // 重新定义：live_ 只计「持有 valid」——见下方 Revised。
    int id() const { return id_; }
    static int live() { return live_; }
};

// 更干净的计数 RAII
class Ticket {
    static inline int live_ = 0;
    bool owned_ = false;
    int id_ = -1;

public:
    explicit Ticket(int id) : owned_(true), id_(id) { ++live_; }

    ~Ticket() {
        if (owned_) {
            --live_;
        }
    }

    Ticket(const Ticket&) = delete;
    Ticket& operator=(const Ticket&) = delete;

    Ticket(Ticket&& o) noexcept : owned_(std::exchange(o.owned_, false)), id_(o.id_) {}

    Ticket& operator=(Ticket&& o) noexcept {
        if (this != &o) {
            if (owned_) {
                --live_;
            }
            owned_ = std::exchange(o.owned_, false);
            id_ = o.id_;
        }
        return *this;
    }

    int id() const { return id_; }
    static int live() { return live_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [raii_resource_handle] ===\n";

    // -------------------------------------------------------------------------
    // §入门：Ticket 作用域结束自动释放
    // -------------------------------------------------------------------------
    assert(Ticket::live() == 0);
    {
        Ticket t{1};
        assert(Ticket::live() == 1);
        assert(t.id() == 1);
    }
    assert(Ticket::live() == 0);
    std::cout << "[intro] Ticket live back to 0 after scope\n";

    // -------------------------------------------------------------------------
    // §进阶：移动转移所有权
    // -------------------------------------------------------------------------
    Ticket a{7};
    Ticket b{std::move(a)};
    assert(Ticket::live() == 1);
    assert(b.id() == 7);
    std::cout << "[advanced] move keeps single ownership live=" << Ticket::live() << '\n';

    // -------------------------------------------------------------------------
    // §专家：文件句柄 + 临时路径
    // -------------------------------------------------------------------------
    const auto path = std::filesystem::temp_directory_path() / "learncpp_stage04_raii.txt";
    {
        FileHandle f(path.string().c_str(), "w");
        f.write_line("hello-raii");
    }  // fclose

    {
        FileHandle f(path.string().c_str(), "r");
        char buf[64]{};
        assert(std::fgets(buf, sizeof(buf), f.get()) != nullptr);
        // fgets 保留换行
        std::string line = buf;
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }
        assert(line == "hello-raii");
        std::cout << "[expert] FileHandle round-trip: " << line << '\n';
    }

    std::error_code ec;
    std::filesystem::remove(path, ec);

    std::cout << "=== raii_resource_handle: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section02/raii_resource_handle", run>;

}  // namespace
