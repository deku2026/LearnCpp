// Topic    : throw; 重抛、异常转换、catch(...) 与 terminate 边界
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 1.4
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : rethrow_and_catch_all
// Topic id : part2/stage09/section01/rethrow_and_catch_all
// Refs     : https://en.cppreference.com/w/cpp/language/throw
//            https://en.cppreference.com/w/cpp/error/terminate
//            ISO [except.throw] [except.handle]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int g_log_count = 0;

void log_error(const char* where, const std::exception& e) {
    ++g_log_count;
    std::cout << "  [log@" << where << "] " << e.what() << '\n';
}

void low_level() {
    throw std::invalid_argument("bad input at low_level");
}

// 中间层：记录后重抛同一个异常对象（throw; 无操作数）
void mid_level() {
    try {
        low_level();
    } catch (const std::exception& e) {
        log_error("mid_level", e);
        throw;  // 重抛当前异常，不切片、不新建
    }
}

// 异常转换：把底层异常包装成更上层语义
void api_layer() {
    try {
        mid_level();
    } catch (const std::invalid_argument& e) {
        log_error("api_layer", e);
        throw std::runtime_error(std::string("API failed: ") + e.what());
    }
}

// catch(...) 兜底后转成统一错误
std::string run_job(bool blow_non_std) {
    try {
        if (blow_non_std) {
            throw 0xDEAD;  // 非 std::exception
        }
        api_layer();
        return "ok";
    } catch (const std::exception& e) {
        return std::string("std-ex: ") + e.what();
    } catch (...) {
        return "unknown-ex";
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [rethrow_and_catch_all] 主干：throw; 重抛 ===\n";
    {
        g_log_count = 0;
        const auto msg = run_job(/*blow_non_std=*/false);
        assert(msg.find("API failed:") != std::string::npos);
        assert(msg.find("bad input") != std::string::npos);
        // mid_level + api_layer 各记一次
        assert(g_log_count == 2);
        std::cout << "result: " << msg << " logs=" << g_log_count << '\n';
    }

    std::cout << "=== 对抗：catch(...) 捕获非标准异常对象 ===\n";
    {
        const auto msg = run_job(/*blow_non_std=*/true);
        assert(msg == "unknown-ex");
        std::cout << "non-std throw handled as: " << msg << '\n';
    }

    std::cout << "=== 对抗：nested / 当前异常查询 ===\n";
    {
        try {
            throw std::logic_error("inner");
        } catch (...) {
            // std::current_exception 可保存异常供稍后 rethrow_exception
            std::exception_ptr ep = std::current_exception();
            assert(ep != nullptr);
            try {
                std::rethrow_exception(ep);
            } catch (const std::logic_error& e) {
                std::cout << "rethrow_exception: " << e.what() << '\n';
            }
        }
    }

    std::cout << "=== 专节：危险边界（注释说明，不触发 terminate）===\n";
    // 1) 当前没有异常在处理时执行 throw; → std::terminate
    // 2) 异常逃出 main / 线程入口且未捕获 → std::terminate
    // 3) noexcept 函数里抛出且未在内部吃掉 → std::terminate
    // 本教学 topic 故意不触发这些路径，保证可运行验收。
    std::cout << "never call bare throw; outside an active catch handler\n";
    std::cout << "Qt: do not let exceptions escape event loop / signals\n";

    std::cout << "[rethrow_and_catch_all] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/rethrow_and_catch_all", run>;

}  // namespace
