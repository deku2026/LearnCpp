// Topic    : using 别名替代 typedef（可读性 + 从左往右读）
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 7.1
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section07_type_aliases_and_traits
// Item     : using_alias_replaces_typedef
// Topic id : part2/stage02/section07/using_alias_replaces_typedef
// Refs     : https://en.cppreference.com/cpp/language/type_alias
//            ISO [dcl.typedef]（using 与 typedef 同属别名声明）

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// 旧：typedef
typedef unsigned long ulong_old;
typedef void (*CallbackOld)(int, int);
typedef std::vector<int> IntVecOld;

// 新：using（从左往右，函数指针可读性高很多）
using ulong = unsigned long;
using Callback = void (*)(int, int);
using IntVec = std::vector<int>;
using String = std::string;

// 嵌套：指针 / 数组 / 成员指针 的别名
using Matrix3 = int[3][3];
using IntPtr = int*;
using CString = const char*;

void on_event(int a, int b) {
    (void)a;
    (void)b;
}

int g_hits = 0;
void bump(int, int) {
    ++g_hits;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [using_alias_replaces_typedef] 入门：语义等价、写法更清晰 ===\n";
    {
        static_assert(std::is_same_v<ulong, unsigned long>);
        static_assert(std::is_same_v<ulong, ulong_old>);
        static_assert(std::is_same_v<Callback, CallbackOld>);
        static_assert(std::is_same_v<IntVec, IntVecOld>);
        static_assert(std::is_same_v<IntVec, std::vector<int>>);

        [[maybe_unused]] ulong x = 42ul;
        IntVec v{1, 2, 3};
        Callback cb = on_event;
        cb(1, 2);
        assert(x == 42ul && v.size() == 3);
        std::cout << "[intro] using and typedef name the same types\n";
    }

    std::cout << "=== 进阶：函数指针 / 复杂声明；与 Qt typedef 习惯对照 ===\n";
    {
        // typedef void (*Callback)(int,int) 要把名字塞进中间
        // using Callback = void(*)(int,int) 名字在左、类型在右
        Callback handlers[] = {on_event, bump};
        g_hits = 0;
        handlers[1](0, 0);
        assert(g_hits == 1);

        [[maybe_unused]] CString msg = "hello";
        assert(std::string{msg} == "hello");

        // 🔶 迁移：typedef QList<int> IntList; → using IntList = QList<int>;
        using Bytes = std::vector<std::uint8_t>;
        Bytes buf{0x00, 0xFF};
        assert(buf.size() == 2);

        std::cout << "[advanced] prefer using everywhere; alias templates need using\n";
    }

    std::cout << "=== 专家：别名不是新类型；可与依赖名 typename 搭配 ===\n";
    {
        // 别名不引入新类型，只是同义词（与 typedef 一样）
        ulong a = 1;
        unsigned long b = a;
        static_assert(std::is_same_v<decltype(a), decltype(b)>);

        // 依赖类型上的别名（预告模板章节）
        // template<class C> using ValueT = typename C::value_type;
        using VecValue = IntVec::value_type;
        static_assert(std::is_same_v<VecValue, int>);

        // 注意：using 不能直接做「部分特化」——那是别名模板 + 主模板的职责
        // （见 alias_template 题）
        Matrix3 m{};
        m[0][0] = 7;
        assert(m[0][0] == 7);
        std::cout << "[expert] alias is synonym, not distinct type\n";
    }

    std::cout << "[using_alias_replaces_typedef] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section07/using_alias_replaces_typedef", run>;

}  // namespace
