// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.1 可见性 PUBLIC/PRIVATE/INTERFACE
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : public_private_interface
// Topic id : part4/section01/public_private_interface
//
// 要点: PRIVATE 不传播; PUBLIC = 自用+下游; INTERFACE = 仅下游(header-only)。
// 仓库对照: target_include_directories(learn_cpp PRIVATE include/) —— 不外传。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class Vis { Private, Public, Interface };

struct Prop {
    Vis vis;
    std::string value;
};

struct Node {
    std::string name;
    std::vector<Prop> props;
    std::vector<std::string> links;  // PRIVATE link 图简化
};

// 求「消费者 C 链接 A 后」实际看到的属性(简化传播规则)。
std::set<std::string> effective_for_consumer(const Node& provider) {
    std::set<std::string> out;
    for (const auto& p : provider.props) {
        if (p.vis == Vis::Public || p.vis == Vis::Interface) {
            out.insert(p.value);
        }
        // PRIVATE: 消费者看不到
    }
    return out;
}

std::set<std::string> used_by_self(const Node& self) {
    std::set<std::string> out;
    for (const auto& p : self.props) {
        if (p.vis == Vis::Private || p.vis == Vis::Public) {
            out.insert(p.value);
        }
        // INTERFACE: 自身编译不用
    }
    return out;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== public_private_interface visibility ===\n";

    // --- 入门: 三关键字口诀 ---
    Node impl_lib{};
    impl_lib.name = "impl_lib";
    impl_lib.props.push_back({Vis::Private, "fmt_impl"});     // 实现用
    impl_lib.props.push_back({Vis::Public, "core_headers"});  // 头里暴露
    impl_lib.props.push_back({Vis::Interface, "header_only_json"});

    auto self = used_by_self(impl_lib);
    auto down = effective_for_consumer(impl_lib);

    assert(self.contains("fmt_impl"));
    assert(self.contains("core_headers"));
    assert(!self.contains("header_only_json"));  // INTERFACE 自己不用

    assert(!down.contains("fmt_impl"));  // PRIVATE 不传
    assert(down.contains("core_headers"));
    assert(down.contains("header_only_json"));

    std::cout << "  self sees:";
    for (const auto& s : self) {
        std::cout << ' ' << s;
    }
    std::cout << "\n  consumer sees:";
    for (const auto& s : down) {
        std::cout << ' ' << s;
    }
    std::cout << '\n';

    // --- 进阶: 常见坑 — 实现依赖标成 PUBLIC 污染下游 ---
    Node bad{};
    bad.name = "bad_lib";
    bad.props.push_back({Vis::Public, "private_spdlog"});  // 错: 实现细节泄漏
    auto leak = effective_for_consumer(bad);
    assert(leak.contains("private_spdlog"));  // 下游被迫链 spdlog
    std::cout << "  pitfall: PUBLIC impl dep leaks to consumers\n";

    // --- 专家: header-only 库 = 全 INTERFACE ---
    Node header_only{};
    header_only.name = "nlohmann_json_like";
    header_only.props.push_back({Vis::Interface, "include/nlohmann"});
    assert(used_by_self(header_only).empty());
    assert(effective_for_consumer(header_only).contains("include/nlohmann"));

    // 仓库: learn_cpp 的 include 是 PRIVATE —— 下游(无)拿不到, 也不该当库导出
    Node learn{};
    learn.name = "learn_cpp";
    learn.props.push_back({Vis::Private, "LearnCpp/include"});
    assert(effective_for_consumer(learn).empty());
    assert(used_by_self(learn).contains("LearnCpp/include"));

    std::cout << "public_private_interface: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/public_private_interface", run>;

}  // namespace
