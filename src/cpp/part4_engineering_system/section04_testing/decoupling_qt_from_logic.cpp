// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.4 核心逻辑解耦 Qt/UI
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : decoupling_qt_from_logic
// Topic id : part4/section04/decoupling_qt_from_logic
//
// 要点: 算法/数据处理做成纯 C++ 库; UI 只做展示与事件转发。
//       无需 Qt 事件循环即可 GoogleTest 测核心。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

// ========== core 库(不依赖 Qt) ==========
struct ParseResult {
    bool ok;
    int value;
    std::string error;
};

ParseResult parse_score(const std::string& text) {
    if (text.empty()) {
        return {false, 0, "empty"};
    }
    int v = 0;
    for (char c : text) {
        if (c < '0' || c > '9') {
            return {false, 0, "not a number"};
        }
        v = v * 10 + (c - '0');
    }
    if (v > 100) {
        return {false, 0, "out of range"};
    }
    return {true, v, {}};
}

std::string grade_letter(int score) {
    if (score >= 90) {
        return "A";
    }
    if (score >= 80) {
        return "B";
    }
    if (score >= 70) {
        return "C";
    }
    if (score >= 60) {
        return "D";
    }
    return "F";
}

// ========== UI 适配层(模拟 QObject 槽, 无 Qt 依赖) ==========
class ScoreViewModel {
public:
    explicit ScoreViewModel(std::function<void(const std::string&)> set_label) : set_label_(std::move(set_label)) {}

    // 对应槽: onSubmitClicked
    void on_submit(const std::string& line_edit_text) {
        auto r = parse_score(line_edit_text);
        if (!r.ok) {
            set_label_("error: " + r.error);
            return;
        }
        set_label_("grade " + grade_letter(r.value));
    }

private:
    std::function<void(const std::string&)> set_label_;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== decoupling_qt_from_logic ===\n";

    // --- 单元: 直接测 core, 零 UI ---
    assert(parse_score("95").ok && parse_score("95").value == 95);
    assert(!parse_score("x").ok);
    assert(!parse_score("101").ok);
    assert(grade_letter(95) == "A");
    assert(grade_letter(42) == "F");
    std::cout << "  core library tests without any UI framework\n";

    // --- 薄 UI: 只转发 ---
    std::string label;
    ScoreViewModel vm{[&](const std::string& s) { label = s; }};
    vm.on_submit("88");
    assert(label == "grade B");
    vm.on_submit("nope");
    assert(label.rfind("error:", 0) == 0);
    std::cout << "  view-model adapts core -> label text\n";

    // 架构: src/core 纯 C++ + tests; src/ui 依赖 Qt 且薄
    const bool core_depends_on_qt = false;
    const bool ui_depends_on_core = true;
    assert(!core_depends_on_qt && ui_depends_on_core);

    std::cout << "decoupling_qt_from_logic: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/decoupling_qt_from_logic", run>;

}  // namespace
