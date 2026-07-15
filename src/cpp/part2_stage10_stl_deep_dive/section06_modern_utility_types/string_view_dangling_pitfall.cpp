// Topic     : string_view 悬垂陷阱 + 零拷贝 split 验收
// Doc       : 第2部分-阶段10 · 步骤 6.2
// cppreference: https://en.cppreference.com/cpp/string/basic_string_view
//
// 要点: view 是借用; 别返回局部/临时; split 的 token 依赖输入存活。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 零拷贝分词: token 全部指向输入 sv 的底层
std::vector<std::string_view> split(std::string_view sv, char delim) {
    std::vector<std::string_view> tokens;
    std::size_t start = 0;
    while (start <= sv.size()) {
        const std::size_t end = sv.find(delim, start);
        if (end == std::string_view::npos) {
            tokens.push_back(sv.substr(start));
            break;
        }
        tokens.push_back(sv.substr(start, end - start));
        start = end + 1;
    }
    return tokens;
}

// ✅ 安全: static 存活
[[maybe_unused]] std::string_view ok_static_view() {
    static const std::string s = "ok-static";
    return s;
}

// 教学: 危险模式用注释展示, 不真正解引用悬垂 view
// std::string_view bad_local() {
//     std::string local = "temp";
//     return local;  // local 析构 → 悬垂
// }

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_dangling_pitfall] ===\n";

    // 安全 split: 底层 string 在 token 使用期间存活
    const std::string line = "a,bb,ccc";
    const auto tokens = split(line, ',');
    assert(tokens.size() == 3);
    assert(tokens[0] == "a" && tokens[1] == "bb" && tokens[2] == "ccc");
    // tokens[i].data() 指向 line 内部
    assert(tokens[0].data() >= line.data() && tokens[0].data() < line.data() + line.size());

    std::cout << "[split] tokens:";
    for (auto t : tokens) std::cout << " \"" << t << '"';
    std::cout << '\n';

    // ⚠️ 若写成: auto bad = split(std::string("a,b"), ',');
    //    临时 string 在全表达式结束析构 → tokens 全悬垂 → UB
    // 正确: 先具名
    {
        const std::string tmp = "x,y";
        auto toks = split(tmp, ',');
        assert(toks[0] == "x" && toks[1] == "y");
    }

    assert(ok_static_view() == "ok-static");

    // 临时拼接悬垂(勿使用结果)
    // std::string_view sv = std::string("temp") + "x"; // 悬垂

    // 安全拥有: 需要长命存储时拷贝成 string
    std::vector<std::string> owned;
    for (auto t : split(line, ',')) owned.emplace_back(t);
    assert(owned[2] == "ccc");

    std::cout << "[discipline] borrow only while underlying lives; own if needed\n";
    std::cout << "string_view_dangling_pitfall: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_dangling_pitfall", run>;

}  // namespace
