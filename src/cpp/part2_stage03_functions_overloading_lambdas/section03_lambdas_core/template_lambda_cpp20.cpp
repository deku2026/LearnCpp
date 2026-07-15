// Topic     : 模板 lambda（C++20 显式模板参数）
// Doc       : 第2部分-阶段3 · 步骤 5.7
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: []<class T>(...) 拿到类型名；可约束、可 sizeof(T)、可 partial 使用类型；
//       比纯 auto 参数更可控。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [template_lambda_cpp20] ===\n";

    // -------------------------------------------------------------------------
    // §入门：显式 <class T>
    // -------------------------------------------------------------------------
    auto first_of = []<class T>(const std::vector<T>& v) -> T { return v.empty() ? T{} : v.front(); };
    std::vector<int> vi{10, 20, 30};
    assert(first_of(vi) == 10);
    std::vector<std::string> vs{"x", "y"};
    assert(first_of(vs) == "x");
    std::cout << "[intro] template parameter list on lambda\n";

    // -------------------------------------------------------------------------
    // §进阶：使用类型本身
    // -------------------------------------------------------------------------
    auto size_of_elem = []<class T>(const std::vector<T>&) { return sizeof(T); };
    assert(size_of_elem(vi) == sizeof(int));

    auto as_string = []<class T>(const T& v) {
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(v);
        } else {
            return std::string{v};
        }
    };
    assert(as_string(42) == "42");
    assert(as_string(std::string{"z"}) == "z");
    std::cout << "[advanced] use T in sizeof / if constexpr\n";

    // -------------------------------------------------------------------------
    // §专家：约束与参数包
    // -------------------------------------------------------------------------
    auto add_integral = []<std::integral T>(T a, T b) { return a + b; };
    assert(add_integral(2, 3) == 5);
    // add_integral(1.0, 2.0); // ❌ 不满足 integral

    auto sum_pack = []<class... Ts>(Ts... xs) { return (xs + ...); };
    assert(sum_pack(1, 2, 3) == 6);

    // 显式指定模板实参（较少用，但合法）
    auto id = []<class T>(T x) { return x; };
    assert(id.operator()<int>(7) == 7);

    std::cout << "[expert] concepts + packs on template lambdas; explicit operator()<T>\n";
    std::cout << "=== template_lambda_cpp20: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/template_lambda_cpp20", run>;

}  // namespace
