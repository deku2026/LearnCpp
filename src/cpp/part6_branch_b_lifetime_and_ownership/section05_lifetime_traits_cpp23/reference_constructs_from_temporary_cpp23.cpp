// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B8 constructs_from_temporary)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section05_lifetime_traits_cpp23
// Item     : reference_constructs_from_temporary_cpp23
// Topic id : part6/b/section05/reference_constructs_from_temporary_cpp23
//
// 要点: 编译期检测「用 U 初始化 T 引用是否会绑到临时」。
// 参考: https://en.cppreference.com/w/cpp/types/reference_constructs_from_temporary

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

template <class Ref, class Arg>
constexpr bool dangerous_bind = std::reference_constructs_from_temporary_v<Ref, Arg>;

template <class T, class Arg>
void store_ref_checked(const T& /*slot*/, Arg&& /*arg*/) {
    static_assert(!std::reference_constructs_from_temporary_v<const T&, Arg>,
                  "argument would bind reference to a temporary");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B8 reference_constructs_from_temporary (C++23) ===\n";

#if defined(__cpp_lib_reference_from_temporary) && __cpp_lib_reference_from_temporary >= 202202L
    static_assert(std::reference_constructs_from_temporary_v<const std::string&, std::string>);
    static_assert(!std::reference_constructs_from_temporary_v<const std::string&, const std::string&>);
    static_assert(!std::reference_constructs_from_temporary_v<const int&, int&>);
    static_assert(std::reference_constructs_from_temporary_v<const int&, int>);

    assert((dangerous_bind<const std::string&, std::string>));
    assert((!dangerous_bind<const std::string&, const std::string&>));

    std::string owned = "ok";
    store_ref_checked(owned, owned);  // 安全：已有对象

    std::cout << "  true  => initializing ref would materialize temporary\n";
    std::cout << "  false => binds to existing object\n";
#else
    std::cout << "  trait not available on this library; skipped asserts\n";
#endif

    std::cout << "reference_constructs_from_temporary_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section05/reference_constructs_from_temporary_cpp23", run>;

}  // namespace
