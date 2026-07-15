// Topic    : std::exception 家族层次与自定义异常
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 1.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : std_exception_hierarchy
// Topic id : part2/stage09/section01/std_exception_hierarchy
// Refs     : https://en.cppreference.com/w/cpp/error/exception
//            https://en.cppreference.com/w/cpp/header/stdexcept
//            ISO [std.exceptions] [exception]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace {

// 惯例：自定义异常派生自 runtime_error / logic_error（带 string 构造）
class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class InvariantBroken : public std::logic_error {
public:
    explicit InvariantBroken(const std::string& msg) : std::logic_error(msg) {}
};

const char* classify(const std::exception& e) {
    // 演示层次：logic_error vs runtime_error 等
    if (dynamic_cast<const std::logic_error*>(&e)) {
        return "logic_error-family";
    }
    if (dynamic_cast<const std::runtime_error*>(&e)) {
        return "runtime_error-family";
    }
    if (dynamic_cast<const std::bad_alloc*>(&e)) {
        return "bad_alloc";
    }
    return "other-std::exception";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_exception_hierarchy] 主干：标准异常 what() ===\n";
    {
        try {
            throw std::invalid_argument("bad arg");
        } catch (const std::exception& e) {
            assert(std::string(e.what()).find("bad arg") != std::string::npos);
            std::cout << "invalid_argument -> " << classify(e) << ": " << e.what() << '\n';
        }

        try {
            throw std::out_of_range("index");
        } catch (const std::logic_error& e) {
            // out_of_range 是 logic_error 的子类
            std::cout << "out_of_range is logic_error: " << e.what() << '\n';
        }

        try {
            throw std::overflow_error("too big");
        } catch (const std::runtime_error& e) {
            std::cout << "overflow_error is runtime_error: " << e.what() << '\n';
        }
    }

    std::cout << "=== 对抗：vector::at 抛 out_of_range（安全边界检查）===\n";
    {
        std::vector<int> v{1, 2, 3};
        try {
            (void)v.at(99);  // 有边界检查，失败抛异常（非 UB）
            assert(false);
        } catch (const std::out_of_range& e) {
            std::cout << "v.at(99): " << e.what() << '\n';
        }
        // 对照：operator[] 不做检查，越界是 UB —— 见 section08
    }

    std::cout << "=== 对抗：自定义异常 + 统一 catch(std::exception&) ===\n";
    {
        try {
            throw ParseError("json broken");
        } catch (const ParseError& e) {
            std::cout << "specific ParseError: " << e.what() << '\n';
        }

        try {
            throw InvariantBroken("size mismatch");
        } catch (const std::exception& e) {
            // 统一兜底：自定义派生自标准层次即可被 catch
            std::cout << "unified catch [" << classify(e) << "]: " << e.what() << '\n';
            assert(dynamic_cast<const InvariantBroken*>(&e) != nullptr);
        }
    }

    std::cout << "=== 专节：层次心智图（摘录）===\n";
    // exception
    //  ├ logic_error → invalid_argument / domain_error / length_error / out_of_range
    //  ├ runtime_error → range_error / overflow_error / underflow_error
    //  ├ bad_alloc / bad_cast / bad_expected_access (C++23) ...
    std::cout << "logic_error ≈ programming bug; runtime_error ≈ runtime condition\n";
    std::cout << "typeid(ParseError).name()=" << typeid(ParseError).name() << '\n';

    std::cout << "[std_exception_hierarchy] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/std_exception_hierarchy", run>;

}  // namespace
