// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D9 反射方向)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : reflection_future_direction_note
// Topic id : part6/d/section04/reflection_future_direction_note
//
// 要点: C++26 静态反射可能改变元编程/定制点写法；当前用 traits+CPO。
// 参考: P2996 direction (educational note + today's portable alternative)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// 今日可移植的「描述类型」：traits / 概念（反射前身）
template <typename T>
struct type_name {
    static constexpr const char* value = "unknown";
};

template <>
struct type_name<int> {
    static constexpr const char* value = "int";
};

template <>
struct type_name<std::string> {
    static constexpr const char* value = "string";
};

template <typename T>
concept has_size = requires(const T& t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

template <has_size R>
std::size_t size_of(const R& r) {
    return r.size();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D9 reflection future direction (note) ===\n";

    assert((std::string{type_name<int>::value} == "int"));
    assert((std::string{type_name<std::string>::value} == "string"));

    std::vector<int> v{1, 2, 3};
    assert(size_of(v) == 3);

    std::cout << "  today: traits, concepts, CPO/ADL for customization\n";
    std::cout << "  tomorrow (C++26+): static reflection may generate/query\n";
    std::cout << "  members without hand-written specializations (P2996 dir.)\n";
    std::cout << "reflection_future_direction_note: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/reflection_future_direction_note", run>;

}  // namespace
