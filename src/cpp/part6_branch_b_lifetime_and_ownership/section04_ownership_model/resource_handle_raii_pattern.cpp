// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B6 RAII 资源句柄)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section04_ownership_model
// Item     : resource_handle_raii_pattern
// Topic id : part6/b/section04/resource_handle_raii_pattern
//
// 要点: 构造获取、析构释放；禁止拷贝、允许移动；优先 Rule of 0。
// 参考: https://en.cppreference.com/w/cpp/language/raii

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

// 手写 RAII：模拟「文件描述符/句柄」
class SlotHandle {
    int id_ = -1;
    static int live_;

public:
    explicit SlotHandle(int id) : id_(id) {
        if (id_ < 0) throw std::runtime_error("bad slot");
        ++live_;
    }
    ~SlotHandle() {
        if (id_ >= 0) {
            --live_;
            id_ = -1;
        }
    }
    SlotHandle(const SlotHandle&) = delete;
    SlotHandle& operator=(const SlotHandle&) = delete;
    SlotHandle(SlotHandle&& o) noexcept : id_(std::exchange(o.id_, -1)) {}
    SlotHandle& operator=(SlotHandle&& o) noexcept {
        if (this != &o) {
            if (id_ >= 0) --live_;
            id_ = std::exchange(o.id_, -1);
        }
        return *this;
    }
    int get() const { return id_; }
    static int live() { return live_; }
};
int SlotHandle::live_ = 0;

// Rule of 0：成员是 RAII
struct BetterFile {
    std::unique_ptr<std::FILE, decltype(&std::fclose)> file_{nullptr, &std::fclose};
    explicit BetterFile(const char* path, const char* mode) {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdeprecated-enum-float-conversion"
#endif
        std::FILE* f = std::fopen(path, mode);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
        if (!f) {
            // 演示路径：用临时内存「假打开」时允许失败路径
            file_.reset();
            return;
        }
        file_.reset(f);
    }
    bool open() const { return static_cast<bool>(file_); }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B6 resource handle RAII pattern ===\n";

    assert(SlotHandle::live() == 0);
    {
        SlotHandle a{1};
        assert(a.get() == 1);
        assert(SlotHandle::live() == 1);
        SlotHandle b = std::move(a);
        assert(a.get() == -1);
        assert(b.get() == 1);
        assert(SlotHandle::live() == 1);
    }
    assert(SlotHandle::live() == 0);

    // 异常路径也释放
    try {
        SlotHandle ok{2};
        throw std::runtime_error("boom");
    } catch (const std::exception&) {
        assert(SlotHandle::live() == 0);
    }

    // Rule of 0 封装 C FILE*（可能因无写权限失败——不强制打开成功）
    BetterFile maybe{"nul", "r"};  // Windows NUL 设备通常可读
#if !defined(_WIN32)
    BetterFile maybe2{"/dev/null", "r"};
    (void)maybe2;
#endif
    (void)maybe;

    std::cout << "  acquire in ctor, release in dtor; move leaves empty source\n";
    std::cout << "resource_handle_raii_pattern: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section04/resource_handle_raii_pattern", run>;

}  // namespace
