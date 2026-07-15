// Topic     : 命名空间 namespace
// Doc       : 第2部分-阶段1 · 步骤 7
// cppreference: https://en.cppreference.com/cpp/language/namespace
//               https://en.cppreference.com/cpp/language/using_declaration
//
// 要点: 解决名字冲突；限定名；嵌套与 C++17 嵌套简写；using 声明 vs using 指令；
//       匿名命名空间 = internal linkage；命名空间别名；头文件禁用 using namespace。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <vector>

// —— 具名命名空间（开放：可多次追加）——
namespace inventory {
int stock = 0;

void restock(int n) {
    stock += n;
}

namespace detail {  // 嵌套
int clamp_nonneg(int v) {
    return v < 0 ? 0 : v;
}
}  // namespace detail

// C++17: namespace inventory::ledger { ... } 等价于再嵌一层
namespace ledger {
int total_in = 0;
void record_in(int n) {
    total_in += n;
}
}  // namespace ledger
}  // namespace inventory

namespace inventory {
// 再次打开同一命名空间追加声明
bool is_empty() {
    return stock == 0;
}
}  // namespace inventory

// 全局同名函数：与 inventory:: 隔离，演示冲突消解
int stock() {
    return -1;
}  // "全局假货"，故意不同语义

// 两个库撞名的简化模型
namespace lib_alpha {
struct Logger {
    int id = 1;
};
}  // namespace lib_alpha

namespace lib_beta {
struct Logger {
    int id = 2;
};
}  // namespace lib_beta

// 命名空间别名
namespace inv = inventory;
namespace inv_ledger = inventory::ledger;

// 本 TU 私有助手：匿名命名空间 → 内部链接，其它 .cpp 看不见
namespace {
int tu_local_seed = 42;
[[maybe_unused]] int bump_seed() {
    return ++tu_local_seed;
}
}  // namespace

namespace {

void demo_using_rules() {
    // using 声明：只引入一个名字，精确可控
    using inv::restock;
    restock(3);
    assert(inventory::stock >= 3);

    // using 指令：打开整个命名空间（演示后立即离开作用域）
    {
        using namespace inventory::detail;
        assert(clamp_nonneg(-5) == 0);
        assert(clamp_nonneg(9) == 9);
    }
    // 离开块后 clamp_nonneg 不再作为非限定名可见（未再声明）
}

void demo_adl_swap_idiom() {
    // 阶段 1 只埋伏笔：using std::swap + 非限定 swap，触发 ADL
    std::vector<int> a{1, 2}, b{9, 8};
    using std::swap;
    swap(a, b);  // 找到 vector 的高效 swap（经 ADL / std）
    assert(a[0] == 9 && b[0] == 1);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [namespaces] ===\n";

    // -------------------------------------------------------------------------
    // §入门：限定名与嵌套
    // -------------------------------------------------------------------------
    inventory::stock = 0;
    inventory::restock(10);
    inventory::ledger::record_in(10);
    assert(inventory::stock == 10);
    assert(inventory::ledger::total_in == 10);
    assert(!inventory::is_empty());

    // 全局 stock() 与 inventory::stock 变量互不干扰
    assert(::stock() == -1);
    assert(inv::stock == 10);  // 别名
    inv_ledger::record_in(5);
    assert(inventory::ledger::total_in == 15);

    std::cout << "[intro] inventory::stock=" << inventory::stock << " ledger.total_in=" << inventory::ledger::total_in
              << " ::stock()=" << ::stock() << '\n';

    // 撞名类型靠命名空间区分
    [[maybe_unused]] lib_alpha::Logger la{};
    [[maybe_unused]] lib_beta::Logger lb{};
    assert(la.id == 1 && lb.id == 2);
    std::cout << "[intro] two Logger types coexist via namespaces\n";

    // -------------------------------------------------------------------------
    // §进阶：using 声明 vs using 指令；头文件纪律
    // -------------------------------------------------------------------------
    demo_using_rules();
    // 反面教材（切勿在头文件）:
    //   using namespace std;  // 污染所有 #include 本头的 TU
    // 正确: 头里写 std::vector；.cpp 函数内可局部 using std::cout;
    std::cout << "[pitfall] never put 'using namespace std;' in a header\n";

    // 匿名命名空间：本 TU 专属
    [[maybe_unused]] const int s0 = tu_local_seed;
    assert(bump_seed() == s0 + 1);
    std::cout << "[advanced] unnamed namespace => internal linkage (TU-local)\n";

    // -------------------------------------------------------------------------
    // §专家：链接性与开放命名空间
    // -------------------------------------------------------------------------
    // · 命名空间作用域的非 const 实体默认 external linkage（跨 TU 需唯一 ODR 定义）
    // · 命名空间作用域 const 对象默认 internal linkage（C 不同！）
    // · 匿名命名空间成员 internal linkage（现代替代文件作用域 static）
    // · 命名空间可跨头/源多次打开（std 即如此）
    // · 全局命名空间用前置 :: 强制查找，避免被局部名遮蔽
    // · ADL 完整规则见支线 D；此处仅 swap 惯用法
    demo_adl_swap_idiom();

    // 遮蔽演示
    int stock = 99;  // 局部名遮蔽
    assert(stock == 99);
    // demo_using_rules() 又 restock(3)，故 inventory::stock == 13
    assert(::inventory::stock == 13);  // 限定名不受局部遮蔽影响
    (void)stock;

    std::cout << "[expert] qualify names in headers; unnamed ns for TU-private helpers\n";
    std::cout << "=== namespaces: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/namespaces", run>;

}  // namespace
