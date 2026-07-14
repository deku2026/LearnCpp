#!/usr/bin/env python3
"""Audit LearnKotlin / LearnJava topic coverage vs ArchitectureDesign docs.

Compares practice ## headings in KotlinStudy / JavaStudy against registered topics.
"""
from __future__ import annotations

import re
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
KOTLIN_DOCS = Path(r"C:\MyFile\ArcForges\ArchitectureDesign\KotlinStudy")
JAVA_DOCS = Path(r"C:\MyFile\ArcForges\ArchitectureDesign\JavaStudy")
KOTLIN_TOPICS = ROOT / "src" / "kotlin" / "topics"
JAVA_TOPICS = ROOT / "src" / "java" / "learnj" / "topics"

# --- skip non-practice H2 titles (导读/收尾/纯资源) but still REPORT them ---
SKIP_TITLE = re.compile(
    r"(导读|收尾|完成标志|下一阶段|下一支线|下一章|有意后置|"
    r"与路线图|目录与路线图|一手资源|资源索引|"
    r"在哪看$|C\+\+ 对照小结|本阶段不要做的事|"
    r"易错清单$|贴墙)"
)

# Practice H2 patterns for Kotlin + Java
H2_PRACTICE = re.compile(
    r"^##\s+(?:"
    r"步骤\s*([0-9A-Za-z]+)\s*[·\.、:：]\s*(.+)"  # 步骤 N / 步骤 E
    r"|§\s*(\d+)\s*[·\.、:：]\s*(.+)"  # §N
    r"|([0-9]+(?:\.[0-9]+)+)\s*[·\.、:：]?\s*(.+)"  # 3.1.1 / 5.1
    r"|([A-H])(\d+)\s*[·\.、:：]\s*(.+)"  # A1 · / B12 ·
    r"|(15[A-D]|[A-G])\s*[·\.、:：]\s*(.+)"  # 15A · / A · / E ·
    r"|([0-9]+)\s*[·\.、:：]\s*(.+)"  # 1 · plain numbered (evolution/ledger)
    r")\s*$"
)

# Also capture ## 0 · 工具... as practice if starts with 步骤 0 or 工具/实验
H2_STEP0_ALT = re.compile(
    r"^##\s+(?:"
    r"0\s*[·\.、:：]\s*((?:工具|实验|依赖|最小|让例子|本阶段实证|心智).*)"
    r")\s*$"
)


@dataclass
class DocSection:
    doc: str
    heading: str
    kind: str  # step/sec/letter/numbered
    key: str  # normalized key for matching


def strip_marks(s: str) -> str:
    s = re.sub(r"[🟢🟡🔴⭐🆕⚠️🔷🔶📖🔁]", "", s)
    return re.sub(r"\s+", " ", s).strip()


def parse_practice_h2(line: str) -> tuple[str, str, str] | None:
    """Return (kind, key, title) or None."""
    line = line.strip()
    m = H2_PRACTICE.match(line)
    if m:
        if m.group(1) is not None:
            return "step", f"s{m.group(1).lower()}", strip_marks(m.group(2))
        if m.group(3) is not None:
            return "sec", f"s{int(m.group(3)):02d}", strip_marks(m.group(4))
        if m.group(5) is not None:
            return "dotted", "sec_" + m.group(5).replace(".", "_"), strip_marks(m.group(6))
        if m.group(7) is not None:
            letter, num, title = m.group(7), m.group(8), m.group(9)
            return "branch", f"{letter.lower()}{int(num):02d}", strip_marks(title)
        if m.group(10) is not None:
            return "block", m.group(10).lower(), strip_marks(m.group(11))
        if m.group(12) is not None:
            return "num", f"n{int(m.group(12)):02d}", strip_marks(m.group(13))
    m2 = H2_STEP0_ALT.match(line)
    if m2:
        return "step0", "s00", strip_marks(m2.group(1))
    return None


def load_doc_sections(doc_path: Path) -> list[DocSection]:
    out: list[DocSection] = []
    for line in doc_path.read_text(encoding="utf-8").splitlines():
        if not line.startswith("## "):
            continue
        parsed = parse_practice_h2(line)
        if not parsed:
            # record unmatched ## for visibility
            title = strip_marks(line[3:])
            if title.startswith("0 ·") or title.startswith("0·"):
                # intro — skip as non-practice unless tool-like already matched
                continue
            if SKIP_TITLE.search(title):
                continue
            # unmatched practice candidate
            out.append(DocSection(doc_path.name, title, "unmatched", "unmatched"))
            continue
        kind, key, title = parsed
        if SKIP_TITLE.search(title) and kind not in ("step", "sec", "dotted", "branch", "block", "num", "step0"):
            continue
        # still skip pure resource steps if title is only 资源
        if re.fullmatch(r"(一手资源|资源索引|一手资源索引).*", title):
            continue
        if re.search(r"一手资源|资源索引", title) and "验收" not in title:
            continue
        out.append(DocSection(doc_path.name, title, kind, key))
    return out


# Map doc filename -> stage directory name(s) in repo
KOTLIN_DOC_MAP = {
    "第2部分-阶段2-程序结构与基础语义.md": "p2_s02_basics",
    "第2部分-阶段3-函数与参数.md": "p2_s03_functions",
    "第2部分-阶段4-类对象接口与属性.md": "p2_s04_classes",
    "第2部分-阶段5-可空性与类型系统.md": "p2_s05_null_types",
    "第2部分-阶段6-Lambda扩展与内联.md": "p2_s06_lambda_inline",
    "第2部分-阶段7-集合序列与标准库.md": "p2_s07_collections",
    "第2部分-阶段8-操作符解构与委托.md": "p2_s08_operators",
    "第2部分-阶段9-泛型型变与reified.md": "p2_s09_generics",
    "第2部分-阶段10-注解反射KSP与序列化.md": "p2_s10_reflect_ser",
    "第2部分-阶段11-DSL上下文参数与contracts.md": "p2_s11_dsl_ctx",
    "第2部分-阶段12-协程与结构化并发.md": "p2_s12_coroutines",
    "第2部分-阶段13-Flow热流与Channel.md": "p2_s13_flow",
    "第2部分-阶段14-JVM互操作与字节码.md": "p2_s14_jvm",
    "第2部分-阶段15-KMP与多平台.md": "p2_s15_kmp",
    "第2部分-阶段16-工程构建测试与质量.md": "p2_s16_eng",
    "第3部分-阶段17-版本演进与废弃迁移.md": "p3_s17_evolution",
    "第4部分-支线A-K2与编译器流水线.md": "p4_a_k2",
    "第4部分-支线B-类型系统与重载解析.md": "p4_b_types",
    "第4部分-支线C-协程状态机与Flow实现.md": "p4_c_coro_flow",
    "第4部分-支线D-JVM元数据与ABI.md": "p4_d_abi",
    "第4部分-支线E-KMP-KLIB与Native互操作.md": "p4_e_native",
    "第4部分-支线F-JS与Wasm后端.md": "p4_f_js_wasm",
    "第4部分-支线G-KSP序列化与编译器插件.md": "p4_g_ksp",
    "第4部分-支线H-源码性能与API设计.md": "p4_h_api",
    "第5部分-完整语言表面与知识覆盖账本.md": "p5_ledger",
}

JAVA_DOC_MAP = {
    "第2部分-阶段1-程序结构-JDK-JVM心智模型与工具链.md": "p2_s01_structure",
    "第2部分-阶段2-类型系统-基本类型-引用-数组-String-var-文本块.md": "p2_s02_types",
    "第2部分-阶段3-控制流与模式匹配.md": "p2_s03_control",
    "第2部分-阶段4-类与对象模型基础.md": "p2_s04_classes",
    "第2部分-阶段5-继承-多态-抽象类-Object契约.md": "p2_s05_inheritance",
    "第2部分-阶段6-接口-嵌套类-enum-record-sealed.md": "p2_s06_interfaces",
    "第2部分-阶段7-异常-断言-资源管理-try-with-resources.md": "p2_s07_exceptions",
    "第2部分-阶段8-泛型-类型擦除-通配符-PECS-桥接方法.md": "p2_s08_generics",
    "第2部分-阶段9-Lambda-方法引用-Optional-Stream-Collector.md": "p2_s09_lambda_stream",
    "第2部分-阶段10-集合框架-不可变集合与Sequenced-Collections.md": "p2_s10_collections",
    "第2部分-阶段11-并发基础与Java-Memory-Model.md": "p2_s11_concurrency",
    "第2部分-阶段12-Virtual-Threads-Structured-Concurrency-Scoped-Values-Loom.md": "p2_s12_loom",
    "第2部分-阶段13-关键字完整表-语法特性分类总表-现代写法收口.md": "p2_s13_keywords",
    "第2部分-阶段14-Effective级惯用法与API设计.md": "p2_s14_effective",
    "第3部分-标准库系统化.md": "p3_stdlib",
    "第4部分-工程系统.md": "p4_engineering",
    "第5部分-Java演进史.md": "p5_evolution",
    "第6部分-支线A-JPMS模块系统.md": "p6_branch_a_jpms",
    "第6部分-支线B-反射注解动态代理-MethodHandle-SPI.md": "p6_branch_b_reflect",
    "第6部分-支线C-Class文件-字节码-类加载.md": "p6_branch_c_classfile",
    "第6部分-支线D-JVM架构-GC-对象布局.md": "p6_branch_d_jvm_gc",
    "第6部分-支线E-JIT-逃逸分析-运行时优化-CDS-AOT.md": "p6_branch_e_jit_aot",
    "第7部分-性能工程线.md": "p7_perf",
    "第8部分-本地互操作-JNI-FFM-Vector-API.md": "p8_native",
}


def count_topics(stage_dir: Path) -> int:
    if not stage_dir.is_dir():
        return 0
    n = 0
    for p in stage_dir.rglob("*"):
        if p.suffix in {".kt", ".java"}:
            n += 1
    return n


def topic_keys_from_files(stage_dir: Path) -> set[str]:
    """Extract step-like keys from filenames (s01_..., P2...S01..., sec_3_1_1)."""
    keys: set[str] = set()
    if not stage_dir.is_dir():
        return keys
    for p in stage_dir.iterdir():
        if p.suffix not in {".kt", ".java"}:
            continue
        name = p.stem
        # kotlin: s01_package_import / s00_...
        m = re.match(r"s(\d+)_", name, re.I)
        if m:
            keys.add(f"s{int(m.group(1)):02d}")
            keys.add(f"s{m.group(1)}")  # raw
            continue
        # java class style: ...S01Jdk... or ...Sec_3_1_1...
        m = re.search(r"S(\d{2})", name)
        if m:
            keys.add(f"s{m.group(1)}")
            keys.add(f"s{int(m.group(1))}")
        m = re.search(r"Sec_(\d+(?:_\d+)*)", name, re.I)
        if m:
            keys.add("sec_" + m.group(1))
        # letter steps: A01 via A1 in class? skip
        m = re.match(r"s([a-z]\d*)_", name, re.I)
        if m:
            keys.add(m.group(1).lower())
    return keys


def list_all_h2(doc_path: Path) -> list[str]:
    return [
        strip_marks(line[3:].strip())
        for line in doc_path.read_text(encoding="utf-8").splitlines()
        if line.startswith("## ")
    ]


def audit_lang(label: str, docs_dir: Path, topics_root: Path, doc_map: dict[str, str]) -> list[str]:
    report: list[str] = []
    report.append(f"# {label} coverage audit\n")
    total_doc = 0
    total_topics = 0
    missing_stages: list[str] = []
    short_stages: list[str] = []

    for doc_name, stage in sorted(doc_map.items(), key=lambda x: x[1]):
        doc_path = docs_dir / doc_name
        if not doc_path.is_file():
            report.append(f"## MISSING DOC: {doc_name}\n")
            continue
        sections = load_doc_sections(doc_path)
        # filter unmatched for separate reporting
        practice = [s for s in sections if s.kind != "unmatched"]
        unmatched = [s for s in sections if s.kind == "unmatched"]
        stage_dir = topics_root / stage
        n_topics = count_topics(stage_dir)
        keys = topic_keys_from_files(stage_dir)
        total_doc += len(practice)
        total_topics += n_topics

        # expected keys from doc
        exp_keys = []
        for s in practice:
            if s.kind in ("step", "sec", "step0"):
                # normalize s0 -> s00, s1 -> s01, sE stays
                k = s.key
                m = re.fullmatch(r"s(\d+)", k)
                if m:
                    k = f"s{int(m.group(1)):02d}"
                exp_keys.append((k, s.heading))
            elif s.kind == "dotted":
                exp_keys.append((s.key, s.heading))
            elif s.kind == "branch":
                exp_keys.append((s.key, s.heading))
            elif s.kind == "block":
                exp_keys.append((s.key, s.heading))
            elif s.kind == "num":
                exp_keys.append((s.key, s.heading))

        missing = []
        for k, title in exp_keys:
            # check key presence loosely
            ok = False
            if k in keys:
                ok = True
            else:
                m = re.fullmatch(r"s(\d+)", k)
                if m and (f"s{int(m.group(1)):02d}" in keys or f"s{int(m.group(1))}" in keys):
                    ok = True
                # branch keys a01 vs a1
                m = re.fullmatch(r"([a-h])(\d+)", k)
                if m and f"{m.group(1)}{int(m.group(2)):02d}" in keys:
                    ok = True
            if not ok:
                missing.append((k, title))

        status = "OK"
        if n_topics < len(practice):
            status = "SHORT"
            short_stages.append(stage)
        if missing:
            status = "GAPS"
            missing_stages.append(stage)

        report.append(f"## {stage}  [{status}]")
        report.append(f"- doc: `{doc_name}`")
        report.append(f"- practice H2 parsed: **{len(practice)}** | topics on disk: **{n_topics}** | key hits: {len(exp_keys)-len(missing)}/{len(exp_keys)}")
        if missing:
            report.append(f"- **missing keys ({len(missing)}):**")
            for k, t in missing:
                report.append(f"  - `{k}` — {t}")
        if unmatched:
            report.append(f"- unmatched H2 (not classified as practice pattern, {len(unmatched)}):")
            for s in unmatched[:20]:
                report.append(f"  - {s.heading}")
            if len(unmatched) > 20:
                report.append(f"  - … +{len(unmatched)-20} more")
        # full H2 dump for stages that look short
        if n_topics < len(practice) or missing or unmatched:
            report.append("- all ## headings in doc:")
            for h in list_all_h2(doc_path):
                report.append(f"  - {h}")
        report.append("")

    report.append("---")
    report.append(f"TOTAL practice H2: {total_doc}")
    report.append(f"TOTAL topics: {total_topics}")
    report.append(f"Stages with gaps: {missing_stages}")
    report.append(f"Stages short count: {short_stages}")
    return report


def main() -> None:
    out_dir = ROOT / "docs" / "coverage"
    out_dir.mkdir(parents=True, exist_ok=True)

    k_report = audit_lang("LearnKotlin", KOTLIN_DOCS, KOTLIN_TOPICS, KOTLIN_DOC_MAP)
    j_report = audit_lang("LearnJava", JAVA_DOCS, JAVA_TOPICS, JAVA_DOC_MAP)

    (out_dir / "kotlin_coverage.md").write_text("\n".join(k_report) + "\n", encoding="utf-8")
    (out_dir / "java_coverage.md").write_text("\n".join(j_report) + "\n", encoding="utf-8")

    print("Wrote", out_dir / "kotlin_coverage.md")
    print("Wrote", out_dir / "java_coverage.md")
    # print summaries
    for line in k_report:
        if line.startswith("TOTAL") or line.startswith("Stages"):
            print("[KT]", line)
    for line in j_report:
        if line.startswith("TOTAL") or line.startswith("Stages"):
            print("[JV]", line)


if __name__ == "__main__":
    main()
