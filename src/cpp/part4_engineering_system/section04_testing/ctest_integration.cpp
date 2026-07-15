// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.3 CTest + gtest_discover_tests
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : ctest_integration
// Topic id : part4/section04/ctest_integration
//
// 要点: enable_testing + add_test / gtest_discover_tests;
//       ctest --output-on-failure; 退出码 0=全过。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct CTestCase {
    std::string name;
    int (*fn)();
};

// 单个用例: 返回 0 成功, 非 0 失败(ctest 约定)
int test_add() {
    return (1 + 1 == 2) ? 0 : 1;
}
int test_sub() {
    return (5 - 3 == 2) ? 0 : 1;
}
int test_fail_demo() {
    return 1;  // 故意失败, 仅在隔离调用时用
}

struct CTestResult {
    int total = 0;
    int passed = 0;
    int failed = 0;
    std::vector<std::string> failed_names;
};

CTestResult run_ctest(const std::vector<CTestCase>& tests, bool output_on_failure) {
    CTestResult r;
    r.total = static_cast<int>(tests.size());
    for (const auto& t : tests) {
        const int code = t.fn();
        if (code == 0) {
            ++r.passed;
        } else {
            ++r.failed;
            r.failed_names.push_back(t.name);
            if (output_on_failure) {
                std::cout << "  FAILED: " << t.name << " exit=" << code << '\n';
            }
        }
    }
    return r;
}

// gtest_discover_tests: 从二进制 --gtest_list_tests 注册
std::vector<std::string> discover(const std::vector<std::string>& listed) {
    return listed;  // 每条 TEST 变成 ctest 用例
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== ctest_integration ===\n";

    std::vector<CTestCase> suite{
        {"Add.Works", test_add},
        {"Sub.Works", test_sub},
    };
    auto r = run_ctest(suite, true);
    assert(r.failed == 0 && r.passed == 2);
    std::cout << "  ctest summary: " << r.passed << "/" << r.total << " passed\n";

    // 发现机制
    auto names = discover({"Parser.Valid", "Parser.Invalid", "Engine.Start"});
    assert(names.size() == 3);
    std::cout << "  discovered tests=" << names.size() << '\n';

    // 失败退出码
    std::vector<CTestCase> with_fail{
        {"Add.Works", test_add},
        {"Fail.Demo", test_fail_demo},
    };
    auto r2 = run_ctest(with_fail, true);
    assert(r2.failed == 1);
    assert(r2.failed_names[0] == "Fail.Demo");
    const int ctest_exit = (r2.failed == 0) ? 0 : 1;
    assert(ctest_exit == 1);
    std::cout << "  ctest exit code on failure=" << ctest_exit << '\n';

    // CMake:
    // enable_testing()
    // gtest_discover_tests(unit_tests)
    // CI: ctest --test-dir build --output-on-failure
    std::cout << "ctest_integration: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/ctest_integration", run>;

}  // namespace
