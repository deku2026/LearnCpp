// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.5 Ninja 后端
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : ninja_backend
// Topic id : part4/section01/ninja_backend
//
// 要点: cmake -G Ninja 生成 build.ninja; 依赖图精细 + 默认高并行。
// 仓库: CMakePresets.json base.generator = "Ninja"

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

// 迷你任务图: 模拟 Ninja 可按边就绪并行调度
struct Dag {
    std::unordered_map<std::string, std::vector<std::string>> outs_of;  // edge: dep -> node
    std::unordered_map<std::string, int> indeg;
    std::unordered_set<std::string> nodes;

    void edge(const std::string& dep, const std::string& node) {
        nodes.insert(dep);
        nodes.insert(node);
        outs_of[dep].push_back(node);
        indeg[node]++;
        indeg.try_emplace(dep, 0);
    }
};

// 返回拓扑层数(可并行宽度上界的简化: 层数越少链路越深)
int topo_layers(Dag g) {
    std::queue<std::string> q;
    for (const auto& n : g.nodes) {
        if (g.indeg[n] == 0) {
            q.push(n);
        }
    }
    int layers = 0;
    [[maybe_unused]] int seen = 0;
    while (!q.empty()) {
        const int sz = static_cast<int>(q.size());
        for (int i = 0; i < sz; ++i) {
            auto u = q.front();
            q.pop();
            ++seen;
            for (const auto& v : g.outs_of[u]) {
                if (--g.indeg[v] == 0) {
                    q.push(v);
                }
            }
        }
        ++layers;
    }
    assert(seen == static_cast<int>(g.nodes.size()));
    return layers;
}

// 理想并行度: 单层最大就绪节点数(再跑一遍记录 max width)
int max_ready_width(Dag g) {
    std::queue<std::string> q;
    for (const auto& n : g.nodes) {
        if (g.indeg[n] == 0) {
            q.push(n);
        }
    }
    int maxw = 0;
    while (!q.empty()) {
        const int sz = static_cast<int>(q.size());
        maxw = std::max(maxw, sz);
        for (int i = 0; i < sz; ++i) {
            auto u = q.front();
            q.pop();
            for (const auto& v : g.outs_of[u]) {
                if (--g.indeg[v] == 0) {
                    q.push(v);
                }
            }
        }
    }
    return maxw;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== ninja_backend (DAG parallel model) ===\n";

    // --- 入门: 预设固定 Ninja ---
    const std::string preset_generator = "Ninja";  // CMakePresets base
    assert(preset_generator == "Ninja");
    std::cout << "  LearnCpp preset generator=" << preset_generator << '\n';

    // --- 进阶: 独立 .cpp 可并行, 链接依赖全部 obj ---
    Dag d;
    // compile edges: headers conceptually ignored; sources independent
    for (const char* s : {"a.cpp", "b.cpp", "c.cpp", "d.cpp"}) {
        d.edge(s, std::string(s) + ".obj");
    }
    d.edge("a.cpp.obj", "app.exe");
    d.edge("b.cpp.obj", "app.exe");
    d.edge("c.cpp.obj", "app.exe");
    d.edge("d.cpp.obj", "app.exe");

    // 宽度: 4 个编译可并行; 层: 源→obj→link
    // 注: 我们的边是 src->obj, obj->exe; src 入度0
    const int width = max_ready_width(d);
    // 重新构图算 layers
    Dag d2;
    for (const char* s : {"a.cpp", "b.cpp", "c.cpp", "d.cpp"}) {
        d2.edge(s, std::string(s) + ".obj");
    }
    d2.edge("a.cpp.obj", "app.exe");
    d2.edge("b.cpp.obj", "app.exe");
    d2.edge("c.cpp.obj", "app.exe");
    d2.edge("d.cpp.obj", "app.exe");
    const int layers = topo_layers(d2);
    assert(width >= 4);
    assert(layers >= 2);
    std::cout << "  max parallel compile width=" << width << " topo layers=" << layers << '\n';

    // --- 专家: Ninja 相对 Make 的卖点(可检测的抽象) ---
    // 1) 显式 deps (depfile) 2) 无递归 make 3) 低开销调度
    [[maybe_unused]] const bool uses_depfiles = true;
    [[maybe_unused]] const bool recursive_make = false;
    assert(uses_depfiles && !recursive_make);

    // 生成器选择口令
    // cmake -S . -B build -G Ninja
    // cmake --preset windows-debug   # 已含 generator Ninja
    std::cout << "  tip: cmake --preset windows-debug  (generator Ninja)\n";

    std::cout << "ninja_backend: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/ninja_backend", run>;

}  // namespace
