// Topic     : 每个 .cpp → 独立目标文件
// Doc       : 第2部分-阶段1 · 步骤 2 / 5
// cppreference: https://en.cppreference.com/cpp/language/translation_phases
//               https://en.cppreference.com/cpp/language/storage_duration  (linkage)
//
// 要点: 分离编译独立性；目标文件符号表 U/T；内部链接互不可见；
//       改一个 cpp 不必重编全部；链接才合并。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 模拟两个目标文件里的"对外"与"对内"符号
enum class SymBind { Defined /*T*/, Undefined /*U*/, Local /*t*/ };

struct Symbol {
    std::string_view name;
    SymBind bind;
};

struct ObjectFile {
    std::string_view tu_name;
    std::vector<Symbol> syms;

    bool defines(std::string_view n) const {
        for (const auto& s : syms) {
            if (s.name == n && s.bind == SymBind::Defined) {
                return true;
            }
        }
        return false;
    }
    bool needs(std::string_view n) const {
        for (const auto& s : syms) {
            if (s.name == n && s.bind == SymBind::Undefined) {
                return true;
            }
        }
        return false;
    }
};

// 链接器简化模型：每个 U 必须在某个 .o 或库中找到唯一 T
[[maybe_unused]] bool link_ok(const std::vector<ObjectFile>& objs, std::string_view symbol) {
    int defs = 0;
    bool used = false;
    for (const auto& o : objs) {
        if (o.defines(symbol)) {
            ++defs;
        }
        if (o.needs(symbol)) {
            used = true;
        }
    }
    if (!used) {
        return true;  // 无人引用则无需定义（简化）
    }
    return defs == 1;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [per_cpp_object_file_independence] ===\n";

    // -------------------------------------------------------------------------
    // §入门：两个 TU，两份 .o
    // -------------------------------------------------------------------------
    // net.cpp 定义 send()；main.cpp 调用 send() 但只有声明。
    ObjectFile net_o{
        "net.cpp",
        {
            {"mu::send(int)", SymBind::Defined},
            {"net_local_helper()", SymBind::Local},  // 匿名命名空间 / static
        },
    };
    ObjectFile main_o{
        "main.cpp",
        {
            {"main", SymBind::Defined},
            {"mu::send(int)", SymBind::Undefined},  // 调用点 → U
        },
    };

    assert(net_o.defines("mu::send(int)"));
    assert(main_o.needs("mu::send(int)"));
    assert(!main_o.defines("mu::send(int)"));
    std::cout << "[intro] main.o has U for send; net.o has T — independent until link\n";

    // -------------------------------------------------------------------------
    // §进阶：链接合并 vs 内部符号隔离
    // -------------------------------------------------------------------------
    std::vector<ObjectFile> prog{net_o, main_o};
    assert(link_ok(prog, "mu::send(int)"));

    // 若忘记把 net.o 加入链接:
    std::vector<ObjectFile> broken{main_o};
    assert(!link_ok(broken, "mu::send(int)"));
    std::cout << "[advanced] omitting net.o => undefined reference to send\n";

    // 两个 TU 各有 local helper 同名：Local 不导出，不冲突
    ObjectFile a_o{"a.cpp", {{"helper()", SymBind::Local}}};
    ObjectFile b_o{"b.cpp", {{"helper()", SymBind::Local}}};
    // 若 helper 是 external 且两 TU 都 Defined → multiple definition
    ObjectFile a_bad{"a.cpp", {{"helper()", SymBind::Defined}}};
    ObjectFile b_bad{"b.cpp", {{"helper()", SymBind::Defined}}};
    [[maybe_unused]] int ext_defs = 0;
    for (const auto& o : {a_bad, b_bad}) {
        if (o.defines("helper()")) {
            ++ext_defs;
        }
    }
    assert(ext_defs == 2);  // 冲突场景
    std::cout << "[advanced] internal linkage helpers may share names across TUs safely\n";

    // -------------------------------------------------------------------------
    // §专家：工程含义与工具
    // -------------------------------------------------------------------------
    // · 并行编译: 各 .cpp → .o 可 -j 并行；链接串行汇合。
    // · 增量构建: 只重编变更的 TU 及其依赖头触及的 TU。
    // · nm -C file.o | findstr send
    //     T = text defined here; U = undefined reference recorded in this .o
    // · dumpbin /SYMBOLS file.obj  (MSVC)
    // · 独立性边界: 仍共享语言运行时约定、ODR、ABI；"看不见"的是其它 TU 的**定义体**。
    // · Unity build 把多 cpp 合成一个 TU，是工程权衡，不改变标准模型的教学意义。

    const std::string_view tip = "read object symbols with nm/dumpbin before blaming the linker";
    assert(!tip.empty());
    std::cout << "[expert] " << tip << '\n';
    std::cout << "=== per_cpp_object_file_independence: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/per_cpp_object_file_independence", run>;

}  // namespace
