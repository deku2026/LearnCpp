// Topic     : 注释（// 与 /* */）
// Doc       : 第2部分-阶段1 · 步骤 9
// cppreference: https://en.cppreference.com/cpp/comment
// 相关       : https://en.cppreference.com/cpp/language/translation_phases  (阶段 3)
//
// 要点: 两种注释语法；阶段 3 把注释换成一个空格；块注释不可嵌套；
//       maximal munch 与 /* 吞掉后续代码；注释写"为什么"而非复述"做什么"。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

// 单行注释：从 // 到物理行尾（行拼接阶段之后的逻辑行）
// 块注释：/* ... */，可跨行，但第一个 */ 即结束——不可嵌套。

// 翻译阶段 3：每个注释被替换成**一个空格**。因此下面两个声明在语义上等价于
// 用空格隔开的 token，注释本身不进入后续编译记号流。
int /* spacer */ token_a = 7;
int  // line-comment also becomes one space before next line's tokens merge? No:
     // 行注释吃到行尾；下一物理行是新内容。
    token_b = 11;

// 演示：用块注释"挖掉"中间操作数，剩余仍是合法表达式
constexpr int masked_sum() {
    return 10 + /* 20 + */ 30;  // 等价于 10 +  30
}

// 故意展示"不能嵌套"的文本形态（外层用行注释说明，避免真的写出非法嵌套源）
// 错误形态: /* outer /* inner */ still-in-comment? */  —— 内层 */ 提前结束外层。
// 安全注释掉大段含 /* */ 的代码: 用 #if 0 ... #endif（预处理器，阶段 4）。

int count_slashes_in_doc(std::string_view s) {
    int n = 0;
    for (char c : s) {
        if (c == '/') {
            ++n;
        }
    }
    return n;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [comments] ===\n";

    // -------------------------------------------------------------------------
    // §入门：两种写法与行尾注释
    // -------------------------------------------------------------------------
    int score = 0;  // 行尾注释：说明局部意图即可
    score += token_a + token_b;
    assert(token_a == 7 && token_b == 11);
    assert(masked_sum() == 40);
    std::cout << "[intro] token_a+token_b=" << (token_a + token_b) << " masked_sum=" << masked_sum() << '\n';

    // -------------------------------------------------------------------------
    // §进阶：注释替换为空格 → 粘连风险；maximal munch 与除法/解引用
    // -------------------------------------------------------------------------
    // 经典 maximal munch 坑: y/*z 会被看成"开始块注释"，不是 y / *z。
    // 正确写法: y / (*z) 或在 / 与 * 之间留空格并避免 /* 序列。
    int z_val = 2;
    int* z = &z_val;
    int y = 8;
    int x = y / (*z);  // 不要写成 y/*z
    assert(x == 4);
    std::cout << "[pitfall] y/(*z)=" << x << " (avoid y/*z which opens a comment)\n";

    // 字符串与字符字面量里的 // 和 /* 不是注释
    const char* url = "https://example.com/*not-a-comment*/";
    const std::string path = "C:\\docs//notes";  // 两个 / 在字符串内
    assert(std::string_view{url}.find("/*") != std::string_view::npos);
    assert(count_slashes_in_doc(path) == 2);
    std::cout << "[intro] string may contain // or /* without starting a comment\n";

    // 用 #if 0 安全禁用大段代码（比嵌套块注释可靠）
#if 0
    this is not compiled even if it looks like prose;
    /* nested-looking junk */ int dead = 1;
#endif
    std::cout << "[advanced] prefer #if 0 ... #endif to disable blocks with /* */ inside\n";

    // -------------------------------------------------------------------------
    // §专家：风格与阶段边界
    // -------------------------------------------------------------------------
    // 好注释解释"为什么 / 权衡 / 不变量"，坏注释复述"i = i + 1 // 加一"。
    // 文档注释 /** ... */ 不是标准特殊形式，只是习惯；Doxygen 等工具识别。
    // 阶段关系: 注释在阶段 3 清除 → 预处理器（阶段 4）看不到注释内的 # 指令意图
    // （注释里的 #define 不会生效）。反过来，#if 0 块里的内容仍是预处理记号扫描对象，
    // 但被条件丢弃。
    //
    // 工具: clang -E / cl /P 看预处理结果时注释已消失；
    //       godbolt "Preprocessor output" 同理。

    const int header_skip = 1;  // CSV 首行是列名，不是数据记录
    assert(header_skip == 1);
    score += header_skip;
    assert(score == token_a + token_b + header_skip);

    std::cout << "[expert] comments become one space at phase 3; they never form tokens.\n";
    std::cout << "=== comments: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/comments", run>;

}  // namespace
