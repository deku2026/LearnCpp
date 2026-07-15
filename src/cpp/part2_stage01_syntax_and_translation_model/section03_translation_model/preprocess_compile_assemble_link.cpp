// Topic     : 预处理 → 编译 → 汇编 → 链接（翻译模型全景）
// Doc       : 第2部分-阶段1 · 步骤 2
// cppreference: https://en.cppreference.com/cpp/language/translation_phases
// ISO       : [lex.phases]
//
// 要点: 工程四阶段 vs 标准九阶段；每阶段产物；每个 .cpp 独立走完前段；
//       链接才拼符号；本地 -E/-S/-c 命令。

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>
#include <string>
#include <string_view>

namespace {

enum class EngStage {
    Preprocess,  // 阶段 1–4 工程视图
    Compile,     // 阶段 5–7 → 汇编文本
    Assemble,    // 汇编器 → 目标文件
    Link         // 阶段 9 → 可执行/库
};

struct Artifact {
    EngStage stage;
    std::string_view typical_name;
    std::string_view contains;
};

constexpr std::array<Artifact, 4> k_pipeline{{
    {EngStage::Preprocess, "hello.i", "fully expanded source; no # directives left"},
    {EngStage::Compile, "hello.s", "assembly text for this TU only"},
    {EngStage::Assemble, "hello.o/.obj", "machine code + symbol table (U/T/...)"},
    {EngStage::Link, "hello.exe/a.out", "resolved image: all TUs + libraries"},
}};

const char* stage_name(EngStage s) {
    switch (s) {
        case EngStage::Preprocess:
            return "preprocess";
        case EngStage::Compile:
            return "compile";
        case EngStage::Assemble:
            return "assemble";
        case EngStage::Link:
            return "link";
    }
    return "?";
}

// 标准 9 阶段的压缩记忆（运行时打印，便于背诵；C++23 [lex.phases] 口径）
std::string_view phase_blurb(int phase) {
    switch (phase) {
        case 1:
            // C++23(P2314 等): translation character set；通用字符名处理简化
            return "map source bytes → translation character set; normalize newlines";
        case 2:
            // C++23(P2223): 行尾反斜杠后的尾随空白先修剪再拼接——旧坑 `\ ` 不再断裂
            return "line splicing via trailing backslash (C++23 trims whitespace after \\)";
        case 3:
            return "preprocessing tokens; comments → one space; maximal munch";
        case 4:
            return "preprocessor: #include/#define/#if...; directives removed after";
        case 5:
            return "character/string literals → execution character set";
        case 6:
            return "adjacent string literal concatenation";
        case 7:
            return "compile: tokens, syntax, semantics for one translation unit";
        case 8:
            return "template instantiation as needed";
        case 9:
            return "link: TUs + instantiations + libraries → program image";
        default:
            return "";
    }
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [preprocess_compile_assemble_link] ===\n";

    // -------------------------------------------------------------------------
    // §入门：工程四步
    // -------------------------------------------------------------------------
    assert(k_pipeline.size() == 4);
    for (const auto& a : k_pipeline) {
        std::cout << "  [" << stage_name(a.stage) << "] " << a.typical_name << " → " << a.contains << '\n';
    }

    // 核心句：每个 .cpp（+其 include − 条件剔除）独立预处理/编译/汇编成 .o，
    // 彼此在链接前看不见对方的定义。
    std::cout << "[intro] independence: TU_A cannot see definitions only in TU_B until link\n";

    // -------------------------------------------------------------------------
    // §进阶：标准 9 阶段 ↔ 四步分组
    // -------------------------------------------------------------------------
    // 1–4 预处理（文本层，不懂 C++ 语法）
    // 5–8 编译（理解 C++；7 为主，8 模板）
    // 9   链接
    // 工程上的"汇编"夹在编译产出 .s 与目标文件之间。
    for (int p = 1; p <= 9; ++p) {
        assert(!phase_blurb(p).empty());
    }
    // 记忆抓手：1–4 文本层（预处理器不懂 C++）；5–8 编译；9 链接
    assert(phase_blurb(2).find("backslash") != std::string_view::npos);
    assert(phase_blurb(4).find("preprocessor") != std::string_view::npos);
    std::cout << "[advanced] phases 1-4 text; 5-8 compile; 9 link (see phase_blurb)\n";
    std::cout << "  e.g. phase2 (C++23 P2223): " << phase_blurb(2) << '\n';
    std::cout << "  e.g. phase4: " << phase_blurb(4) << '\n';
    std::cout << "  e.g. phase9: " << phase_blurb(9) << '\n';

    // -------------------------------------------------------------------------
    // §专家：本地拆解命令 + 产物里看什么
    // -------------------------------------------------------------------------
    // GCC/Clang:
    //   g++ -std=c++23 -E hello.cpp -o hello.i   # .i: 无 # 指令；#include 已粘完
    //   g++ -std=c++23 -S hello.cpp -o hello.s   # .s: main: / call …（≈ godbolt）
    //   g++ -std=c++23 -c hello.cpp -o hello.o
    //   g++ hello.o -o hello
    //   nm -C hello.o    # U = undefined (cout 等), T = defined text (main)
    // MSVC:
    //   cl /std:c++latest /P /C hello.cpp      # preprocess → .i
    //   cl /std:c++latest /FA /c hello.cpp     # assembly listing
    //   cl /std:c++latest /c hello.cpp         # .obj
    //   link hello.obj
    // clang-cl: 类似 MSVC 驱动，也可用 -E / -S / -c
    //
    // 符号表把"分离编译"变肌肉记忆：本 TU 的 main 是 T；库符号是 U，链接才补上。
    // 本 learn_cpp 二进制已是链接完成后的产物；本 topic 固化心智与命令清单。

    // 阶段与工程四步的映射断言（教学探针，非运行期真跑 -E）
    assert(k_pipeline[0].stage == EngStage::Preprocess);
    assert(k_pipeline[3].stage == EngStage::Link);
    assert(k_pipeline[0].typical_name.find(".i") != std::string_view::npos);
    assert(k_pipeline[2].contains.find("symbol") != std::string_view::npos);

    const std::string_view key =
        "separate compilation => headers carry declarations; one definition for non-inline entities";
    assert(key.find("separate compilation") != std::string_view::npos);
    assert(key.find("one definition") != std::string_view::npos);

    // 迷你健全性：流水线顺序编号
    assert(static_cast<int>(EngStage::Preprocess) < static_cast<int>(EngStage::Link));

    std::cout << "[expert] " << key << '\n';
    std::cout << "=== preprocess_compile_assemble_link: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/preprocess_compile_assemble_link", run>;

}  // namespace
