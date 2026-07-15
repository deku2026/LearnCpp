// Topic     : 未定义引用（undefined reference）诊断
// Doc       : 第2部分-阶段1 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/definition
//               https://en.cppreference.com/cpp/language/language_linkage
//
// 要点: 有声明无定义 / 定义未参与链接 / 签名不一致(mangling) / 缺库；
//       与 multiple definition 对照；nm 读符号；不在本进程故意触发链接失败。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace diag {

// 正确配对：声明 + 本 TU 定义（链接成功的对照）
int healthy_api(int x);
int healthy_api(int x) {
    return x + 1;
}

// 仅声明：若被 odr-use 且别处无定义 → 链接 undefined reference
// 本文件故意**不调用**它，避免搞挂 learn_cpp。
int missing_body(int x);

// 签名陷阱：声明与定义看似"同名"但参数类型不同 → mangling 不同 → 仍 U
void paint(int);
void paint(double d) {  // 注意: 这是另一个重载的定义，不是 paint(int) 的定义
    (void)d;
}

}  // namespace diag

namespace {

enum class URefCause {
    NoDefinition,
    ObjectNotLinked,
    SignatureMismatch,
    LibraryNotLinked,
};

const char* cause_text(URefCause c) {
    switch (c) {
        case URefCause::NoDefinition:
            return "declared but never defined";
        case URefCause::ObjectNotLinked:
            return "defined in a .cpp/.obj that was not passed to the linker";
        case URefCause::SignatureMismatch:
            return "definition exists but mangled name differs (params/cv/ref/abi)";
        case URefCause::LibraryNotLinked:
            return "symbol lives in a library; forgot -l / .lib dependency";
    }
    return "?";
}

struct LinkJob {
    std::vector<std::string> objects;
    std::vector<std::string> libs;
    std::vector<std::string> defined_symbols;
    std::vector<std::string> undefined_from_objects;

    bool resolves(std::string_view sym) const {
        for (const auto& d : defined_symbols) {
            if (d == sym) {
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> remaining_undefined() const {
        std::vector<std::string> bad;
        for (const auto& u : undefined_from_objects) {
            if (!resolves(u)) {
                bad.push_back(u);
            }
        }
        return bad;
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [undefined_reference_diagnosis] ===\n";

    // -------------------------------------------------------------------------
    // §入门：错误长什么样
    // -------------------------------------------------------------------------
    // ld: main.o: undefined reference to `mu::add(int, int)'
    // MSVC: error LNK2019: unresolved external symbol ...
    // 含义: 某个 .o 记录了"需要符号 X"，链接输入里找不到 X 的定义。

    assert(diag::healthy_api(41) == 42);
    std::cout << "[intro] healthy_api(41)=" << diag::healthy_api(41) << " (decl+def matched)\n";

    // 不调用 diag::missing_body / paint(int)——否则本教学程序无法链接。
    std::cout << "[intro] missing_body is declared only — calling it would LNK2019\n";

    // -------------------------------------------------------------------------
    // §进阶：四大成因 + 模拟链接任务
    // -------------------------------------------------------------------------
    for (URefCause c : {URefCause::NoDefinition, URefCause::ObjectNotLinked, URefCause::SignatureMismatch,
                        URefCause::LibraryNotLinked}) {
        std::cout << "  - " << cause_text(c) << '\n';
    }

    // 场景: main.o 需要 mu::add，定义在 math.o 但链接命令忘了 math.o
    LinkJob forgot_obj;
    forgot_obj.objects = {"main.obj"};
    forgot_obj.undefined_from_objects = {"mu::add(int, int)"};
    forgot_obj.defined_symbols = {"main"};  // 没有 mu::add
    assert(!forgot_obj.remaining_undefined().empty());

    LinkJob fixed = forgot_obj;
    fixed.objects.push_back("math.obj");
    fixed.defined_symbols.push_back("mu::add(int, int)");
    assert(fixed.remaining_undefined().empty());
    std::cout << "[advanced] simulated fix: add math.obj providing mu::add\n";

    // 签名不一致: 头文件 int foo(int); 源文件 int foo(long) { ... }
    // 调用点生成对 foo(int) 的 U，源提供 foo(long) 的 T → 对不上。
    diag::paint(3.0);  // OK: 匹配 paint(double)
    // diag::paint(3); // 若取消注释且无 paint(int) 定义 → 链接失败
    std::cout << "[advanced] signature mismatch => different mangled symbol\n";

    // -------------------------------------------------------------------------
    // §专家：诊断步骤与对照错误
    // -------------------------------------------------------------------------
    // 1) 读报错: 哪个 .o 引用了哪个 mangled/ demangled 名？
    // 2) nm -C main.o | findstr add     → 应为 U
    // 3) nm -C math.o | findstr add     → 应为 T；若没有，定义没编进来或签名不同
    // 4) dumpbin /SYMBOLS (MSVC) / llvm-nm
    // 5) c++filt / undname 解码 C++ 修饰名
    // 6) 检查 CMake target_link_libraries 是否漏库
    //
    // multiple definition 是对偶病: 同一非 inline 符号多个 T。
    // undefined reference: 需要的 T 个数是 0；multiple: ≥2。
    //
    // extern "C" 关闭 mangling：C 与 C++ 混链时符号名规则不同，也常导致 U。
    //
    // 验收自检（独立工程）:
    //   只链接 main.o → 复现 U；补上 math.o → 消失。

    const std::string playbook =
        "identify U symbol → locate expected defining TU → ensure that .obj/lib is linked → verify signature";
    assert(playbook.find(".obj") != std::string::npos);

    std::cout << "[expert] " << playbook << '\n';
    std::cout << "=== undefined_reference_diagnosis: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/undefined_reference_diagnosis", run>;

}  // namespace
