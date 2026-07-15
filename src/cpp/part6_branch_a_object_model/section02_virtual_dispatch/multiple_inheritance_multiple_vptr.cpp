// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A5 多继承 / 多 vptr)
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : multiple_inheritance_multiple_vptr
// Topic id : part6/a/section02/multiple_inheritance_multiple_vptr
//
// 要点: 多个多态基类 → 对象内多个 vptr (每部分一个);
//       转到非主基类指针通常会调整地址; 勿用 reinterpret_cast 在基类间乱转;
//       sizeof 近似为各基类子对象 + 派生成员之和 (含对齐)。
// 参考: Itanium ABI multiple inheritance; [class.mi]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

struct Printable {
    int p = 10;
    virtual ~Printable() = default;
    virtual std::string print() const { return "Printable"; }
};

struct Serializable {
    int s = 20;
    virtual ~Serializable() = default;
    virtual std::string serialize() const { return "Serializable"; }
};

struct Document : Printable, Serializable {
    int body = 30;
    std::string print() const override { return "Document::print"; }
    std::string serialize() const override { return "Document::serialize"; }
};

static_assert(std::is_polymorphic_v<Printable>);
static_assert(std::is_polymorphic_v<Serializable>);
static_assert(std::is_polymorphic_v<Document>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== multiple inheritance / multiple vptr ===\n";

    Document doc;
    Printable* pp = &doc;
    Serializable* sp = &doc;

    // --- 核心实证: 非主基类指针地址往往不同于完整对象 ---
    const auto doc_u = reinterpret_cast<std::uintptr_t>(&doc);
    const auto pp_u = reinterpret_cast<std::uintptr_t>(pp);
    const auto sp_u = reinterpret_cast<std::uintptr_t>(sp);

    std::cout << "  &Document      = 0x" << std::hex << doc_u << std::dec << '\n';
    std::cout << "  Printable*     = 0x" << std::hex << pp_u << "  delta=" << std::dec
              << static_cast<std::ptrdiff_t>(pp_u - doc_u) << '\n';
    std::cout << "  Serializable*  = 0x" << std::hex << sp_u << "  delta=" << std::dec
              << static_cast<std::ptrdiff_t>(sp_u - doc_u) << '\n';

    // 第一个多态基类通常与完整对象同址; 第二个常有正偏移
    assert(pp_u == doc_u);
    assert(sp_u != doc_u);  // 多继承布局的关键观察 (主流 ABI)
    assert(sp_u > doc_u);

    // static_cast 会正确调整地址; 按“字节地址原样解读”则不会
    [[maybe_unused]] auto* sp2 = static_cast<Serializable*>(&doc);
    assert(sp2 == sp);
    // 用 uintptr 模拟错误的“不调整强转”(避免 -Wreinterpret-base-class)
    auto* wrong = reinterpret_cast<Serializable*>(reinterpret_cast<std::uintptr_t>(&doc));
    assert(wrong != sp);  // 危险: 地址未调整到 Serializable 子对象
    // 切勿解引用 wrong — 布局错位; 仅比较指针值作教学
    (void)wrong;

    // 从次基类转回完整类型: static_cast / dynamic_cast 会反向调整
    [[maybe_unused]] Document* back = static_cast<Document*>(sp);
    assert(back == &doc);
    assert(reinterpret_cast<std::uintptr_t>(back) == doc_u);

    // --- 虚调用: 各自 vptr 视图 ---
    assert(pp->print() == "Document::print");
    assert(sp->serialize() == "Document::serialize");

    // sizeof: 两个多态子对象 + 成员
    std::cout << "  sizeof(Printable)=" << sizeof(Printable) << " Serializable=" << sizeof(Serializable)
              << " Document=" << sizeof(Document) << '\n';
    assert(sizeof(Document) >= sizeof(Printable) + sizeof(int));  // 至少容纳
    assert(sizeof(Document) > sizeof(Printable));
    assert(sizeof(Document) > sizeof(Serializable));

    // 成员访问经不同子对象
    assert(pp->p == 10);
    assert(sp->s == 20);
    assert(doc.body == 30);

    // void* 跨完整对象: dynamic_cast<void*> 从任意多态指针回到起点
    [[maybe_unused]] void* from_sec = dynamic_cast<void*>(sp);
    [[maybe_unused]] void* from_pri = dynamic_cast<void*>(pp);
    assert(from_sec == from_pri);
    assert(from_sec == static_cast<void*>(&doc));

    // 布局心智 (Itanium 示意, MSVC 细节不同):
    // Document:
    //   [vptr_Printable][p][pad][vptr_Serializable][s][pad][body][pad]
    // 主基类 Printable 共用偏移 0 的 vptr; Serializable 自带次 vptr。

    std::cout << "multiple_inheritance_multiple_vptr: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/multiple_inheritance_multiple_vptr", run>;

}  // namespace
