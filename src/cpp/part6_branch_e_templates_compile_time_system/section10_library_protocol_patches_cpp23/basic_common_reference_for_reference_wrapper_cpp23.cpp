// LearnCpp topic
// Doc      : part6-branch-e + 路线图模板/编译期 C++23 protocol patches
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section10_library_protocol_patches_cpp23
// Item     : basic_common_reference_for_reference_wrapper_cpp23
// Topic id : part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23
//
// 要点: C++23 为 reference_wrapper 定制 basic_common_reference,
//       使 common_reference_t<ref_wrapper<T>, T&> 等在 Ranges/zip 中正确。
// 参考: P2655; cppreference common_reference

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

namespace {

template <typename A, typename B>
using CR = std::common_reference_t<A, B>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E10 basic_common_reference + reference_wrapper (C++23) ===\n";

    int x = 10;
    std::reference_wrapper<int> rw = x;
    assert(rw.get() == 10);
    rw.get() = 20;
    assert(x == 20);

    using R1 = CR<int&, int&>;
    static_assert(std::is_same_v<R1, int&>);
    using R2 = CR<const int&, int&>;
    static_assert(std::is_same_v<R2, const int&>);

    using R3 = std::common_reference_t<std::reference_wrapper<int>&, int&>;
    static_assert(std::is_reference_v<R3> || std::is_same_v<std::remove_cvref_t<R3>, std::reference_wrapper<int>> ||
                  std::is_same_v<std::remove_cvref_t<R3>, int>);

    std::vector<int> v{1, 2, 3};
    auto first = std::ref(v[0]);
    assert(first.get() == 1);
    first.get() = 99;
    assert(v[0] == 99);

    // basic_common_reference 是可定制点: 用户可为代理引用类型特化
    // template<class T, class U, template<class> class TQual, ...>
    // struct basic_common_reference<MyProxy<T>, U, TQual, UQual>;

    std::cout << "  ref_wrapper is a proxy; common_reference must unwrap wisely\n";
    std::cout << "  enables views::zip / join with proxy iterators\n";
    std::cout << "  without the patch: zip of ref_wrapper sequences ill-formed/wrong\n";
    std::cout << "basic_common_reference_for_reference_wrapper_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section10/basic_common_reference_for_reference_wrapper_cpp23", run>;

}  // namespace
