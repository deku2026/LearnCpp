// Topic    : try / catch / throw 基础（按 const 引用捕获、顺序匹配、catch(...)）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 1.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : try_catch_throw_basics
// Topic id : part2/stage09/section01/try_catch_throw_basics
// Refs     : https://en.cppreference.com/w/cpp/language/exceptions
//            https://en.cppreference.com/w/cpp/language/try_catch
//            https://en.cppreference.com/w/cpp/language/throw
//            ISO [except] [except.handle]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

double divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("divisor is zero");
    }
    return static_cast<double>(a) / static_cast<double>(b);
}

// 派生异常：用于演示“按值捕获会切片”
struct ConfigError : std::runtime_error {
    explicit ConfigError(const std::string& msg) : std::runtime_error(msg) {}
    int code = 42;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [try_catch_throw_basics] 主干：throw + 按 const& 捕获 ===\n";
    {
        try {
            const double ok = divide(10, 2);
            assert(ok == 5.0);
            std::cout << "divide(10,2)=" << ok << '\n';

            (void)divide(1, 0);  // 抛出
            assert(false && "should not reach");
        } catch (const std::invalid_argument& e) {
            // 推荐：const 引用 —— 无切片、无多余拷贝
            std::cout << "caught invalid_argument: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "caught std::exception: " << e.what() << '\n';
            assert(false && "more specific catch should win");
        } catch (...) {
            std::cout << "caught unknown\n";
            assert(false);
        }
    }

    std::cout << "=== 对抗：catch 顺序（具体在前，宽泛在后）===\n";
    {
        bool hit_specific = false;
        try {
            throw ConfigError("bad cfg");
        } catch (const ConfigError& e) {
            hit_specific = true;
            assert(e.code == 42);
            std::cout << "ConfigError: " << e.what() << " code=" << e.code << '\n';
        } catch (const std::exception& e) {
            (void)e;
            assert(false && "ConfigError must match first");
        }
        assert(hit_specific);
    }

    std::cout << "=== 边界：按值捕获切片 vs catch(...) ===\n";
    {
        // 按值捕获会切片：派生成员丢失，what() 仍可用（基类部分）
        try {
            throw ConfigError("sliced?");
        } catch (std::exception e_by_value) {
            // e_by_value 是切片后的 std::exception / runtime_error 部分
            // 不能访问 ConfigError::code
            std::cout << "by-value catch what=" << e_by_value.what() << " (derived members sliced away)\n";
        }

        // catch(...)：捕获一切，但拿不到对象；常用于最外层兜底/转换
        bool hit_all = false;
        try {
            throw 123;  // 非 std::exception 也能被 catch(...) 接住
        } catch (const std::exception&) {
            assert(false);
        } catch (...) {
            hit_all = true;
            std::cout << "catch(...) swallowed non-exception object (int)\n";
        }
        assert(hit_all);
    }

    std::cout << "=== 专节：Qt 对照（提示）===\n";
    // Qt 多用返回码/信号；标准 C++ 把异常当一等错误手段。
    // 跨 Qt 信号槽/事件循环边界不要让异常逃逸。
    std::cout << "prefer const& catch; order specific→base→(...)\n";

    std::cout << "[try_catch_throw_basics] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/try_catch_throw_basics", run>;

}  // namespace
