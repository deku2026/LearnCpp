// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.2 Fixture / 参数化
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : parameterized_and_fixture
// Topic id : part4/section04/parameterized_and_fixture
//
// 要点: SetUp/TearDown 共享夹具; TEST_P + Values 多数据同逻辑。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// 被测
int factorial(int n) {
    assert(n >= 0);
    int r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

// Fixture
class DatabaseFixture {
public:
    void SetUp() {
        connected_ = true;
        store_.clear();
        ++setup_count;
    }
    void TearDown() {
        store_.clear();
        connected_ = false;
        ++teardown_count;
    }
    void insert(const std::string& k, const std::string& v) {
        assert(connected_);
        store_.emplace_back(k, v);
    }
    std::string query(const std::string& k) const {
        for (const auto& [kk, vv] : store_) {
            if (kk == k) {
                return vv;
            }
        }
        return {};
    }
    static int setup_count;
    static int teardown_count;

private:
    bool connected_ = false;
    std::vector<std::pair<std::string, std::string>> store_;
};

int DatabaseFixture::setup_count = 0;
int DatabaseFixture::teardown_count = 0;

void run_with_fixture(std::function<void(DatabaseFixture&)> body) {
    DatabaseFixture db;
    db.SetUp();
    body(db);
    db.TearDown();
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== parameterized_and_fixture ===\n";

    DatabaseFixture::setup_count = 0;
    DatabaseFixture::teardown_count = 0;

    // --- Fixture: 每个用例独立 setup/teardown ---
    run_with_fixture([](DatabaseFixture& db) {
        db.insert("k", "v");
        assert(db.query("k") == "v");
    });
    run_with_fixture([](DatabaseFixture& db) {
        assert(db.query("k").empty());  // 不共享状态
    });
    assert(DatabaseFixture::setup_count == 2);
    assert(DatabaseFixture::teardown_count == 2);
    std::cout << "  fixture isolation: setup=" << DatabaseFixture::setup_count << '\n';

    // --- 参数化: 同逻辑多组数据 ---
    const std::vector<std::pair<int, int>> cases{
        {0, 1},
        {1, 1},
        {5, 120},
        {6, 720},
    };
    int param_pass = 0;
    for (const auto& [in, expected] : cases) {
        assert(factorial(in) == expected);
        ++param_pass;
    }
    assert(param_pass == 4);
    std::cout << "  parameterized factorial cases=" << param_pass << '\n';

    // gtest: INSTANTIATE_TEST_SUITE_P(Various, FactorialTest, Values(...))
    std::cout << "parameterized_and_fixture: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/parameterized_and_fixture", run>;

}  // namespace
