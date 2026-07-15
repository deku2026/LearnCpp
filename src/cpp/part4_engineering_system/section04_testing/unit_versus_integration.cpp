// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.4 单元 vs 集成
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : unit_versus_integration
// Topic id : part4/section04/unit_versus_integration
//
// 要点: 单元=隔离快、无 IO; 集成=多组件协作。金字塔: 多单元、少集成。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace {

// --- 领域: 订单计价 ---
struct Line {
    int qty;
    int price_cents;
};

int subtotal_cents(const std::vector<Line>& lines) {
    int s = 0;
    for (const auto& l : lines) {
        s += l.qty * l.price_cents;
    }
    return s;
}

int tax_cents(int subtotal, int rate_bp) {  // basis points, 1000=10%
    return static_cast<int>(static_cast<long long>(subtotal) * rate_bp / 10000);
}

// 假「数据库」— 集成层
struct FakeDb {
    std::vector<Line> load_cart(const std::string& user) {
        if (user == "alice") {
            return {{2, 500}, {1, 1000}};
        }
        return {};
    }
};

// 单元测试: 纯函数, 无 FakeDb
void unit_tests() {
    assert(subtotal_cents({{1, 100}, {2, 50}}) == 200);
    assert(tax_cents(2000, 1000) == 200);
    assert(tax_cents(0, 1000) == 0);
}

// 集成测试: 计价 + 载入协作
void integration_tests() {
    FakeDb db;
    auto lines = db.load_cart("alice");
    int sub = subtotal_cents(lines);
    int tax = tax_cents(sub, 1000);
    assert(sub == 2000);
    assert(tax == 200);
    assert(sub + tax == 2200);
}

enum class Kind { Unit, Integration };

struct SuiteMeta {
    Kind kind;
    int cases;
    bool touches_io;
    bool needs_network;
};

int preferred_count(Kind k) {
    // 金字塔: 单元 >> 集成
    return k == Kind::Unit ? 100 : 10;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== unit_versus_integration ===\n";

    unit_tests();
    std::cout << "  unit: pure pricing OK\n";

    integration_tests();
    std::cout << "  integration: FakeDb + pricing OK\n";

    SuiteMeta unit{Kind::Unit, 3, false, false};
    SuiteMeta integ{Kind::Integration, 1, true, false};
    assert(!unit.touches_io);
    assert(integ.touches_io);
    assert(preferred_count(Kind::Unit) > preferred_count(Kind::Integration));

    // 速度心智: 单元应毫秒级; 集成可更慢
    using clock = std::chrono::steady_clock;
    auto t0 = clock::now();
    for (int i = 0; i < 1000; ++i) {
        (void)subtotal_cents({{i % 5, 10}});
    }
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - t0).count();
    std::cout << "  1000 unit pricing calls ~" << us << " us\n";
    assert(us >= 0);

    std::cout << "unit_versus_integration: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/unit_versus_integration", run>;

}  // namespace
