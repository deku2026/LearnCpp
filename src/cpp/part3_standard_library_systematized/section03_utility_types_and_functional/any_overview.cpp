// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 any
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : any_overview
// Topic id : part3/section03/any_overview
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/any
//
// 要点: 任意单值类型擦除; any_cast; type(); 优先 variant/optional 若类型集已知。

#include "learn/topic_registry.hpp"

#include <any>
#include <cassert>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/any_overview ===\n";

    std::any a = 42;
    assert(a.has_value());
    assert(std::any_cast<int>(a) == 42);

    a = std::string{"hi"};
    assert(std::any_cast<std::string>(a) == "hi");
    assert(a.type() == typeid(std::string));

    // 指针形式 any_cast: 失败返回 nullptr, 不抛
    a = 3.14;
    if (auto* p = std::any_cast<double>(&a)) {
        (void)p;
        assert(*p == 3.14);
    }
    assert(std::any_cast<int>(&a) == nullptr);

    // 错误类型抛 bad_any_cast
    [[maybe_unused]] bool threw = false;
    try {
        (void)std::any_cast<int>(a);
    } catch (const std::bad_any_cast&) {
        threw = true;
    }
    assert(threw);

    a.reset();
    assert(!a.has_value());

    // 容器中的异构: 最后手段; 已知集合用 variant
    std::vector<std::any> bag;
    bag.emplace_back(1);
    bag.emplace_back(std::string{"x"});
    assert(std::any_cast<int>(bag[0]) == 1);

    std::cout << "[intro] any_cast value/pointer forms ok\n";
    std::cout << "[expert] any = type erasure escape hatch; prefer variant "
                 "when alternatives are closed\n";
    std::cout << "any_overview: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/any_overview", run>;

}  // namespace
