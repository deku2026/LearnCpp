// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B8 converts_from_temporary)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_converts_from_temporary_cpp23
// Topic id : part6/b/section05/reference_converts_from_temporary_cpp23
//
// 要点: converts 版覆盖拷贝初始化/隐式转换产生临时（如 const string& 绑 const char*）。
// 参考: https://en.cppreference.com/w/cpp/types/reference_converts_from_temporary

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B8 reference_converts_from_temporary (C++23) ===\n";

#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    // const char* → 临时 string → 绑 const string&
    static_assert(std::reference_converts_from_temporary_v<const std::string&, const char*>);
    static_assert(std::reference_converts_from_temporary_v<const std::string&, std::string>);
    static_assert(!std::reference_converts_from_temporary_v<const std::string&, const std::string&>);

    // constructs vs converts：隐式转换语境用 converts 更敏感
    static_assert(std::reference_converts_from_temporary_v<const std::string&, const char*>);

    assert((std::reference_converts_from_temporary_v<const std::string&, const char*>));

    // 安全 API：接受 string_view / 按值，避免绑临时 string
    auto use_view = [](std::string_view sv) { return sv.size(); };
    assert(use_view("hello") == 5);
    std::string s = "world";
    assert(use_view(s) == 5);

    std::cout << "  converts catches implicit temporary (e.g. char*→string)\n";
#else
    std::cout << "  trait not available on this library; skipped asserts\n";
#endif

    std::cout << "reference_converts_from_temporary_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/reference_converts_from_temporary_cpp23", run>;

}  // namespace
