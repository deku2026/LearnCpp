// Topic    : 析构函数不得抛异常（栈展开中再抛 → terminate）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 10
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_must_not_throw
// Topic id : part2/stage09/section07/destructor_must_not_throw
// Refs     : https://en.cppreference.com/w/cpp/language/destructor
//            ISO [except.dtor] [class.dtor]
//            C++ Core Guidelines C.36 C.37 E.16

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

// ✅ 正确：清理失败只记录，绝不外抛
class ResourceHandle {
    bool open_ = false;
    bool fail_close_ = false;
    int dtor_logs_ = 0;

public:
    explicit ResourceHandle(bool fail_close = false) : open_(true), fail_close_(fail_close) {}

    // 需要知道关闭成败 → 显式 close（可抛或返回错误）
    bool close() {
        if (!open_) {
            return true;
        }
        open_ = false;
        if (fail_close_) {
            return false;  // 报告失败，但不在 dtor 里抛
        }
        return true;
    }

    ~ResourceHandle() {
        if (open_) {
            try {
                // 兜底清理：记录失败，吞掉一切异常
                if (fail_close_) {
                    ++dtor_logs_;
                    std::cerr << "warning: close failed in dtor (swallowed)\n";
                }
                open_ = false;
            } catch (...) {
                // 析构里绝不让异常逃逸
            }
        }
    }

    bool is_open() const noexcept { return open_; }

    ResourceHandle(const ResourceHandle&) = delete;
    ResourceHandle& operator=(const ResourceHandle&) = delete;
};

// 反面结构（仅注释说明，不运行 terminate 路径）：
// struct Bad { ~Bad() { throw std::runtime_error("no"); } };
// void f() { Bad b; throw std::runtime_error("first"); }
// → 栈展开析构 b → 双异常 → std::terminate

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [destructor_must_not_throw] 主干：显式 close + 兜底析构 ===\n";
    {
        ResourceHandle fh;
        assert(fh.is_open());
        [[maybe_unused]] const bool ok = fh.close();
        assert(ok);
        assert(!fh.is_open());
        std::cout << "explicit close ok; dtor is no-op afterward\n";
    }

    std::cout << "=== 对抗：close 失败用返回值报告（非 dtor 抛）===\n";
    {
        ResourceHandle fh(/*fail_close=*/true);
        [[maybe_unused]] const bool ok = fh.close();
        assert(!ok);
        std::cout << "explicit close reported failure without throwing\n";
    }

    std::cout << "=== 对抗：异常路径上析构仍安全 ===\n";
    {
        try {
            ResourceHandle fh;
            throw std::runtime_error("work failed");
        } catch (const std::runtime_error& e) {
            std::cout << "caught while ResourceHandle cleaned: " << e.what() << '\n';
        }
    }

    std::cout << "=== 专节：为何是铁律 ===\n";
    // 栈展开期间若析构再抛，C++ 无法同时处理两个异常 → terminate。
    // 设计：需要报告失败的清理 → 显式方法；析构只做最后兜底、不抛。
    std::cout << "dtor must not throw; report close errors via explicit API\n";
    std::cout << "double-exception during unwind → std::terminate (not demoed)\n";

    std::cout << "[destructor_must_not_throw] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section07/destructor_must_not_throw", run>;

}  // namespace
