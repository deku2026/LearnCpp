// Topic    : C++23 std::is_scoped_enum
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 8.5
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : is_scoped_enum_cpp23
// Topic id : part2/stage02/section08/is_scoped_enum_cpp23
// Refs     : https://en.cppreference.com/cpp/types/is_scoped_enum
//            提案 P1048

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

enum Unscoped { A };
enum class Scoped { B };
enum struct ScopedStruct { C };
enum class ScopedU8 : unsigned char { D };

// 泛型：仅允许 scoped enum 进入「强类型标签」路径
template <class E>
constexpr int tag_rank() {
    if constexpr (std::is_scoped_enum_v<E>) {
        return 2;
    } else if constexpr (std::is_enum_v<E>) {
        return 1;
    } else {
        return 0;
    }
}

// 要求 scoped，否则 static_assert
template <class E>
constexpr std::underlying_type_t<E> strong_code(E e) {
    static_assert(std::is_scoped_enum_v<E>, "use enum class for strong_code");
    return static_cast<std::underlying_type_t<E>>(e);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [is_scoped_enum_cpp23] 入门：区分 scoped / unscoped ===\n";
    {
        static_assert(std::is_scoped_enum_v<Scoped> == true);
        static_assert(std::is_scoped_enum_v<ScopedStruct> == true);
        static_assert(std::is_scoped_enum_v<Unscoped> == false);
        static_assert(std::is_scoped_enum_v<int> == false);
        static_assert(std::is_scoped_enum_v<ScopedU8> == true);

        // is_enum 对两者都为 true
        static_assert(std::is_enum_v<Scoped>);
        static_assert(std::is_enum_v<Unscoped>);
        static_assert(!std::is_enum_v<int>);

        std::cout << "[intro] is_scoped_enum_v separates enum class from bare enum\n";
    }

    std::cout << "=== 进阶：泛型分支；与 is_enum 组合 ===\n";
    {
        static_assert(tag_rank<Scoped>() == 2);
        static_assert(tag_rank<Unscoped>() == 1);
        static_assert(tag_rank<double>() == 0);

        assert(strong_code(Scoped::B) == 0);
        assert(strong_code(ScopedU8::D) == 0);
        // strong_code(A);  // ❌ static_assert: unscoped

        std::cout << "[advanced] constrain APIs to scoped enums\n";
    }

    std::cout << "=== 专家：feature 宏；库作者迁移检查 ===\n";
    {
#if defined(__cpp_lib_is_scoped_enum)
        std::cout << "[expert] __cpp_lib_is_scoped_enum=" << __cpp_lib_is_scoped_enum << '\n';
#else
        std::cout << "[expert] __cpp_lib_is_scoped_enum not defined\n";
#endif
        // 迁移清单：对公开 API 的枚举做 static_assert(is_scoped_enum_v<E>)
        // 可在单元测试里批量扫描「遗留 unscoped」
        static_assert(std::is_same_v<std::is_scoped_enum<Scoped>::type, std::true_type>);
        assert(std::is_scoped_enum_v<Scoped>);
        std::cout << "[expert] use in concepts/requires in later stages\n";
    }

    std::cout << "[is_scoped_enum_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/is_scoped_enum_cpp23", run>;

}  // namespace
