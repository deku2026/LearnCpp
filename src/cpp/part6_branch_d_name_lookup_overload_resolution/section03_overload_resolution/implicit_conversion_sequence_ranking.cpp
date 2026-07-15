// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D5 转换排名)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section03_overload_resolution
// Item     : implicit_conversion_sequence_ranking
// Topic id : part6/d/section03/implicit_conversion_sequence_ranking
//
// 要点: 标准转换 > 用户定义 > 省略号；标准内 exact > promotion > conversion。
// 参考: [over.ics.rank]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

struct Num {
    int v;
    Num(int x) : v(x) {}  // 用户定义转换 int→Num
};

[[maybe_unused]] std::string f(Num) {
    return "Num";
}
[[maybe_unused]] std::string f(double) {
    return "double";
}

[[maybe_unused]] std::string s(int) {
    return "int";
}
[[maybe_unused]] std::string s(double) {
    return "double";
}

[[maybe_unused]] std::string ellipsis(...) {
    return "ellipsis";
}
[[maybe_unused]] std::string ellipsis(int) {
    return "int";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D5 ICS ranking ===\n";

    // int→double 标准转换 优于 int→Num 用户定义
    assert(f(3) == "double");
    assert((f(Num{9}) == "Num"));

    assert(s('x') == "int");  // promotion
    [[maybe_unused]] float fl = 1.f;
    assert(s(fl) == "double");  // float→double promotion

    assert(ellipsis(1) == "int");  // 精确优于省略号

    std::cout << "implicit_conversion_sequence_ranking: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section03/implicit_conversion_sequence_ranking", run>;

}  // namespace
