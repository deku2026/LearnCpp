// Topic     : std::get 按类型 / 按下标; tuple_element
// Doc       : 第2部分-阶段10 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/utility/tuple/get
//
// 要点: get<I> 与 get<T>; 类型唯一才能 get<T>; array/pair/variant 也支持 get。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_get_and_index] ===\n";

    std::tuple<int, double, std::string> t{1, 2.5, "x"};
    assert(std::get<0>(t) == 1);
    assert(std::get<double>(t) == 2.5);
    assert(std::get<std::string>(t) == "x");

    // 修改
    std::get<0>(t) = 9;
    assert(std::get<int>(t) == 9);

    using T0 = std::tuple_element_t<0, decltype(t)>;
    using T2 = std::tuple_element_t<2, decltype(t)>;
    static_assert(std::is_same_v<T0, int>);
    static_assert(std::is_same_v<T2, std::string>);

    // pair
    std::pair p{10, std::string{"ten"}};
    assert(std::get<0>(p) == 10);
    assert(std::get<std::string>(p) == "ten");

    // array
    std::array<int, 3> a{1, 2, 3};
    assert(std::get<2>(a) == 3);

    // variant: get 按类型/下标
    std::variant<int, std::string> v{"hi"};
    assert(std::get<1>(v) == "hi");
    assert(std::get<std::string>(v) == "hi");

    // ⚠️ 重复类型不能 get<T>
    // std::tuple<int,int> bad; std::get<int>(bad); // ill-formed
    std::tuple<int, int> dup{1, 2};
    assert(std::get<0>(dup) == 1 && std::get<1>(dup) == 2);

    std::cout << "[get] tuple/pair/array/variant by index & type OK\n";
    std::cout << "std_get_and_index: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_get_and_index", run>;

}  // namespace
