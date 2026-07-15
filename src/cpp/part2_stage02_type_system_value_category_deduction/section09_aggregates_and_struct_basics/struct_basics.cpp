// Topic    : struct 基础：与 class 仅差默认访问；纯数据聚合习惯
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 9.1
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : struct_basics
// Topic id : part2/stage02/section09/struct_basics
// Refs     : https://en.cppreference.com/cpp/language/classes
//            ISO [class] [class.access]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 习惯：纯数据聚合用 struct（默认 public）
struct Point {
    int x;
    int y;
};

// 习惯：有不变量/封装用 class（默认 private）
class Account {
    double balance_;

public:
    explicit Account(double b) : balance_(b) {}
    double balance() const { return balance_; }
    void deposit(double v) {
        if (v > 0) {
            balance_ += v;
        }
    }
};

// struct 也可写 private / 成员函数——与 class 能力相同
struct Widget {
    std::string name;
    int id = 0;

    Widget() = default;
    Widget(std::string n, int i) : name(std::move(n)), id(i) {}

    int next_id() const { return id + 1; }

private:
    int secret_ = 42;

public:
    int secret() const { return secret_; }
};

// 继承默认权限：struct 公有继承，class 私有继承
struct Base {
    int b = 1;
};
struct PublicDerived : Base {  // 默认 public Base
    int d = 2;
};
class PrivateDerived : Base {  // 默认 private Base
public:
    int peek() const { return b; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [struct_basics] 入门：默认 public 成员 ===\n";
    {
        Point p;
        p.x = 3;
        p.y = 4;
        assert(p.x == 3 && p.y == 4);

        // 聚合初始化（细节见下一题）
        Point q{10, 20};
        assert(q.x + q.y == 30);

        Account acc{100.0};
        // acc.balance_ = 0;  // ❌ private
        acc.deposit(50.0);
        assert(acc.balance() == 150.0);
        std::cout << "[intro] struct default public; class default private\n";
    }

    std::cout << "=== 进阶：struct 可有函数与访问控制；能力等价 ===\n";
    {
        Widget w("dial", 7);  // 有私有成员/用户构造 → 非聚合，走构造函数
        assert(w.next_id() == 8);
        assert(w.secret() == 42);
        // w.secret_ = 0;  // ❌ private

        static_assert(std::is_class_v<Point>);
        static_assert(std::is_class_v<Account>);
        // 二者都是 class-key 引入的类类型
        std::cout << "[advanced] same language feature; style difference\n";
    }

    std::cout << "=== 专家：继承默认；何时用 struct ===\n";
    {
        PublicDerived pd;
        assert(pd.b == 1 && pd.d == 2);  // b 因 public 继承可见

        PrivateDerived pr;
        // pr.b;  // ❌ private 继承
        assert(pr.peek() == 1);

        // 经验法则：
        // - POD/聚合/DTO/配置袋 → struct + 公有数据成员
        // - 不变量、虚函数、资源所有权 → class + 私有数据
        std::cout << "[expert] choose struct for aggregates, class for invariants\n";
    }

    std::cout << "[struct_basics] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/struct_basics", run>;

}  // namespace
