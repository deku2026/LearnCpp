// LearnCpp topic
// Doc      : 第4部分-工程系统.md · header / module / import std 对照
// Stage    : part4_engineering_system
// Section  : section07_import_std_exception
// Item     : header_versus_module_versus_import_std
// Topic id : part4/section07/header_versus_module_versus_import_std
//
// 要点: 头文件文本包含 vs 命名模块 BMI vs 标准库 std 模块。
//       构建模型与隔离性差异用可运行成本/可见性模型表达。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class ImportModel { Header, NamedModule, ImportStd };

struct ModelTraits {
    ImportModel model;
    bool textual_inclusion;
    bool rebuild_all_includers_on_edit;
    bool strong_ownership;  // 模块接口拥有权更清晰
    int parse_cost_per_tu;  // 相对
};

ModelTraits traits(ImportModel m) {
    switch (m) {
        case ImportModel::Header:
            return {m, true, true, false, 100};
        case ImportModel::NamedModule:
            return {m, false, false, true, 30};
        case ImportModel::ImportStd:
            return {m, false, false, true, 20};  // 一次 BMI, 多 TU 复用
    }
    return {m, true, true, false, 100};
}

// 可见性: 头文件宏泄漏 vs 模块不泄漏
[[maybe_unused]] bool macro_leaks_across_units(ImportModel m) {
    return m == ImportModel::Header;
}

const char* label(ImportModel m) {
    switch (m) {
        case ImportModel::Header:
            return "header #include";
        case ImportModel::NamedModule:
            return "named module import";
        case ImportModel::ImportStd:
            return "import std";
    }
    return "?";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== header_versus_module_versus_import_std ===\n";

    [[maybe_unused]] auto h = traits(ImportModel::Header);
    [[maybe_unused]] auto n = traits(ImportModel::NamedModule);
    [[maybe_unused]] auto s = traits(ImportModel::ImportStd);

    assert(h.textual_inclusion);
    assert(!n.textual_inclusion);
    assert(h.parse_cost_per_tu > n.parse_cost_per_tu);
    assert(s.parse_cost_per_tu <= n.parse_cost_per_tu);
    assert(macro_leaks_across_units(ImportModel::Header));
    assert(!macro_leaks_across_units(ImportModel::NamedModule));

    for (auto m : {ImportModel::Header, ImportModel::NamedModule, ImportModel::ImportStd}) {
        auto t = traits(m);
        std::cout << "  " << label(m) << " parse~" << t.parse_cost_per_tu << " strong_own=" << std::boolalpha
                  << t.strong_ownership << '\n';
    }

    // 工程默认建议(文档): 仍用 headers; 模块/import std 实验
    [[maybe_unused]] const ImportModel production_default = ImportModel::Header;
    assert(production_default == ImportModel::Header);
    std::cout << "  production default today: headers\n";

    std::cout << "header_versus_module_versus_import_std: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section07/header_versus_module_versus_import_std", run>;

}  // namespace
