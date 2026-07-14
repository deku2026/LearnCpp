#!/usr/bin/env python3
"""Full regenerate LearnKotlin + LearnJava topics from study docs (H2 practice units)."""
from __future__ import annotations

import hashlib
import re
import shutil
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
KOTLIN_DOCS = Path(r"C:\MyFile\ArcForges\ArchitectureDesign\KotlinStudy")
JAVA_DOCS = Path(r"C:\MyFile\ArcForges\ArchitectureDesign\JavaStudy")

# ── doc maps: filename -> (stage_dir, stage_id_prefix, short_class_prefix) ──

KOTLIN_MAP: dict[str, tuple[str, str, str]] = {
    "第2部分-阶段2-程序结构与基础语义.md": ("p2_s02_basics", "part2/stage02", "P2S02Basics"),
    "第2部分-阶段3-函数与参数.md": ("p2_s03_functions", "part2/stage03", "P2S03Functions"),
    "第2部分-阶段4-类对象接口与属性.md": ("p2_s04_classes", "part2/stage04", "P2S04Classes"),
    "第2部分-阶段5-可空性与类型系统.md": ("p2_s05_null_types", "part2/stage05", "P2S05NullTypes"),
    "第2部分-阶段6-Lambda扩展与内联.md": ("p2_s06_lambda_inline", "part2/stage06", "P2S06LambdaInline"),
    "第2部分-阶段7-集合序列与标准库.md": ("p2_s07_collections", "part2/stage07", "P2S07Collections"),
    "第2部分-阶段8-操作符解构与委托.md": ("p2_s08_operators", "part2/stage08", "P2S08Operators"),
    "第2部分-阶段9-泛型型变与reified.md": ("p2_s09_generics", "part2/stage09", "P2S09Generics"),
    "第2部分-阶段10-注解反射KSP与序列化.md": ("p2_s10_reflect_ser", "part2/stage10", "P2S10ReflectSer"),
    "第2部分-阶段11-DSL上下文参数与contracts.md": ("p2_s11_dsl_ctx", "part2/stage11", "P2S11DslCtx"),
    "第2部分-阶段12-协程与结构化并发.md": ("p2_s12_coroutines", "part2/stage12", "P2S12Coroutines"),
    "第2部分-阶段13-Flow热流与Channel.md": ("p2_s13_flow", "part2/stage13", "P2S13Flow"),
    "第2部分-阶段14-JVM互操作与字节码.md": ("p2_s14_jvm", "part2/stage14", "P2S14Jvm"),
    "第2部分-阶段15-KMP与多平台.md": ("p2_s15_kmp", "part2/stage15", "P2S15Kmp"),
    "第2部分-阶段16-工程构建测试与质量.md": ("p2_s16_eng", "part2/stage16", "P2S16Eng"),
    "第3部分-阶段17-版本演进与废弃迁移.md": ("p3_s17_evolution", "part3/stage17", "P3S17Evolution"),
    "第4部分-支线A-K2与编译器流水线.md": ("p4_a_k2", "part4/branch_a", "P4AK2"),
    "第4部分-支线B-类型系统与重载解析.md": ("p4_b_types", "part4/branch_b", "P4BTypes"),
    "第4部分-支线C-协程状态机与Flow实现.md": ("p4_c_coro_flow", "part4/branch_c", "P4CCoroFlow"),
    "第4部分-支线D-JVM元数据与ABI.md": ("p4_d_abi", "part4/branch_d", "P4DAbi"),
    "第4部分-支线E-KMP-KLIB与Native互操作.md": ("p4_e_native", "part4/branch_e", "P4ENative"),
    "第4部分-支线F-JS与Wasm后端.md": ("p4_f_js_wasm", "part4/branch_f", "P4FJsWasm"),
    "第4部分-支线G-KSP序列化与编译器插件.md": ("p4_g_ksp", "part4/branch_g", "P4GKsp"),
    "第4部分-支线H-源码性能与API设计.md": ("p4_h_api", "part4/branch_h", "P4HApi"),
    "第5部分-完整语言表面与知识覆盖账本.md": ("p5_ledger", "part5/ledger", "P5Ledger"),
}

JAVA_MAP: dict[str, tuple[str, str, str]] = {
    "第2部分-阶段1-程序结构-JDK-JVM心智模型与工具链.md": ("p2_s01_structure", "part2/stage01", "P2S01Structure"),
    "第2部分-阶段2-类型系统-基本类型-引用-数组-String-var-文本块.md": ("p2_s02_types", "part2/stage02", "P2S02Types"),
    "第2部分-阶段3-控制流与模式匹配.md": ("p2_s03_control", "part2/stage03", "P2S03Control"),
    "第2部分-阶段4-类与对象模型基础.md": ("p2_s04_classes", "part2/stage04", "P2S04Classes"),
    "第2部分-阶段5-继承-多态-抽象类-Object契约.md": ("p2_s05_inheritance", "part2/stage05", "P2S05Inheritance"),
    "第2部分-阶段6-接口-嵌套类-enum-record-sealed.md": ("p2_s06_interfaces", "part2/stage06", "P2S06Interfaces"),
    "第2部分-阶段7-异常-断言-资源管理-try-with-resources.md": ("p2_s07_exceptions", "part2/stage07", "P2S07Exceptions"),
    "第2部分-阶段8-泛型-类型擦除-通配符-PECS-桥接方法.md": ("p2_s08_generics", "part2/stage08", "P2S08Generics"),
    "第2部分-阶段9-Lambda-方法引用-Optional-Stream-Collector.md": ("p2_s09_lambda_stream", "part2/stage09", "P2S09LambdaStream"),
    "第2部分-阶段10-集合框架-不可变集合与Sequenced-Collections.md": ("p2_s10_collections", "part2/stage10", "P2S10Collections"),
    "第2部分-阶段11-并发基础与Java-Memory-Model.md": ("p2_s11_concurrency", "part2/stage11", "P2S11Concurrency"),
    "第2部分-阶段12-Virtual-Threads-Structured-Concurrency-Scoped-Values-Loom.md": ("p2_s12_loom", "part2/stage12", "P2S12Loom"),
    "第2部分-阶段13-关键字完整表-语法特性分类总表-现代写法收口.md": ("p2_s13_keywords", "part2/stage13", "P2S13Keywords"),
    "第2部分-阶段14-Effective级惯用法与API设计.md": ("p2_s14_effective", "part2/stage14", "P2S14Effective"),
    "第3部分-标准库系统化.md": ("p3_stdlib", "part3/stdlib", "P3Stdlib"),
    "第4部分-工程系统.md": ("p4_engineering", "part4/engineering", "P4Engineering"),
    "第5部分-Java演进史.md": ("p5_evolution", "part5/evolution", "P5Evolution"),
    "第6部分-支线A-JPMS模块系统.md": ("p6_branch_a_jpms", "part6/branch_a", "P6AJpms"),
    "第6部分-支线B-反射注解动态代理-MethodHandle-SPI.md": ("p6_branch_b_reflect", "part6/branch_b", "P6BReflect"),
    "第6部分-支线C-Class文件-字节码-类加载.md": ("p6_branch_c_classfile", "part6/branch_c", "P6CClassfile"),
    "第6部分-支线D-JVM架构-GC-对象布局.md": ("p6_branch_d_jvm_gc", "part6/branch_d", "P6DJvmGc"),
    "第6部分-支线E-JIT-逃逸分析-运行时优化-CDS-AOT.md": ("p6_branch_e_jit_aot", "part6/branch_e", "P6EJitAot"),
    "第7部分-性能工程线.md": ("p7_perf", "part7/perf", "P7Perf"),
    "第8部分-本地互操作-JNI-FFM-Vector-API.md": ("p8_native", "part8/native", "P8Native"),
}

SKIP_TITLE = re.compile(
    r"^(?:"
    r"本阶段导读|本支线导读|本部分导读|导读$|导读 ·|"
    r"收尾|完成标志与去向|下一阶段|下一支线|下一章|"
    r"附录 · 一手|一手资源|资源索引|"
    r"有意后置|与路线图|目录与路线图|编排自检$"
    r")"
)

# H2 practice matchers (order matters)
PATTERNS: list[tuple[str, re.Pattern[str]]] = [
    ("step", re.compile(r"^##\s+步骤\s*([0-9A-Za-z]+)\s*[·\.、:：]\s*(.+)\s*$")),
    ("sec", re.compile(r"^##\s+§\s*(\d+)\s*[·\.、:：]\s*(.+)\s*$")),
    ("dotted", re.compile(r"^##\s+(\d+(?:\.\d+)+)\s*[·\.、:：]?\s*(.+)\s*$")),
    ("letter_num", re.compile(r"^##\s+([A-Ha-h])(\d+)([a-z]?)\s*[·\.、:：]\s*(.+)\s*$")),  # A1, D12b
    ("block", re.compile(r"^##\s+(15[A-Da-d]|[A-Ga-g])\s*[·\.、:：]\s*(.+)\s*$")),  # 15A · / A ·
    ("num", re.compile(r"^##\s+(\d+)\s*[·\.、:：]\s*(.+)\s*$")),  # 1 · ...
]


def strip_marks(s: str) -> str:
    s = re.sub(r"[🟢🟡🔴⭐🆕⚠️🔷🔶📖🔁*]", "", s)
    return re.sub(r"\s+", " ", s).strip()


def should_skip_title(title: str) -> bool:
    t = strip_marks(title)
    if SKIP_TITLE.search(t):
        return True
    if re.search(r"一手资源|资源索引", t) and "验收" not in t:
        return True
    # pure intro "0 · 本阶段导读..."
    if re.match(r"^0\s*[·\.、:：]", t) and re.search(r"导读", t):
        return True
    return False


def slugify(text: str, max_len: int = 48) -> str:
    text = strip_marks(text)
    tokens: list[str] = []
    for m in re.finditer(r"`([^`]+)`", text):
        part = re.sub(r"[^A-Za-z0-9]+", "_", m.group(1)).strip("_").lower()
        if part and part not in tokens:
            tokens.append(part)
    stop = {"vs", "the", "and", "or", "to", "of", "in", "for", "with", "a", "an"}
    for m in re.finditer(r"[A-Za-z][A-Za-z0-9_.$/-]{0,}", text):
        w = m.group(0).replace(".", "_").replace("-", "_").replace("/", "_").lower()
        w = re.sub(r"_+", "_", w).strip("_")
        if w and w not in tokens and w not in stop and len(w) >= 2:
            tokens.append(w)
    zh_map = [
        ("让例子能跑起来", "run_examples"),
        ("实验约定", "lab_setup"),
        ("最小可跑", "minimal_run"),
        ("最小可运行", "minimal_run"),
        ("验收", "acceptance"),
        ("练习", "practice"),
        ("概念图", "concept_map"),
        ("迁移卡", "cpp_migration"),
        ("状态", "status"),
        ("过时", "deprecated"),
        ("非主线", "non_mainline"),
        ("边界", "boundary"),
        ("底层实证", "bytecode_evidence"),
        ("毕业", "graduation"),
        ("检查表", "checklist"),
        ("口述", "oral"),
        ("常见问题", "faq"),
        ("小结", "summary"),
        ("导读", "intro"),
        ("工具", "tooling"),
        ("对照", "contrast"),
        ("易错", "pitfalls"),
        ("设计", "design"),
        ("性能", "performance"),
        ("并发", "concurrency"),
        ("集合", "collections"),
        ("泛型", "generics"),
        ("协程", "coroutines"),
        ("序列化", "serialization"),
        ("模块", "module"),
        ("反射", "reflection"),
        ("注解", "annotations"),
        ("字节码", "bytecode"),
        ("内存", "memory"),
        ("逃逸", "escape"),
        ("本地", "native"),
        ("构建", "build"),
        ("测试", "testing"),
        ("文档", "docs"),
        ("类型", "types"),
        ("空", "null"),
        ("委托", "delegate"),
        ("操作符", "operators"),
        ("函数", "functions"),
        ("类", "classes"),
        ("接口", "interfaces"),
        ("异常", "exceptions"),
        ("流", "flow"),
        ("通道", "channel"),
    ]
    for zh, en in zh_map:
        if zh in text and en not in tokens:
            tokens.append(en)
    if not tokens:
        return "topic_" + hashlib.sha1(text.encode("utf-8")).hexdigest()[:8]
    slug = re.sub(r"_+", "_", "_".join(tokens)).strip("_")
    if len(slug) > max_len:
        slug = slug[:max_len].rstrip("_")
    return slug or "topic"


def java_ident(s: str) -> str:
    parts = [p for p in re.split(r"[^A-Za-z0-9]+", s) if p]
    if not parts:
        return "Topic"
    out = "".join(p[0].upper() + p[1:] for p in parts)
    if out[0].isdigit():
        out = "T" + out
    return out[:70]


@dataclass
class Unit:
    doc: str
    stage_dir: str
    stage_id: str
    short_prefix: str
    step_key: str
    heading: str
    slug: str
    topic_id: str
    class_name: str
    file_stem: str


def parse_units(doc_path: Path, stage_dir: str, stage_id: str, short_prefix: str) -> list[Unit]:
    units: list[Unit] = []
    used_slugs: dict[str, int] = defaultdict(int)
    used_classes: dict[str, int] = defaultdict(int)
    used_keys: dict[str, int] = defaultdict(int)

    for line in doc_path.read_text(encoding="utf-8").splitlines():
        if not line.startswith("## "):
            continue
        step_key = None
        title = None
        for kind, pat in PATTERNS:
            m = pat.match(line.strip())
            if not m:
                continue
            if kind == "step":
                raw, title = m.group(1), m.group(2)
                if raw.isdigit():
                    step_key = f"s{int(raw):02d}"
                else:
                    step_key = f"s{raw.lower()}"
            elif kind == "sec":
                step_key = f"s{int(m.group(1)):02d}"
                title = m.group(2)
            elif kind == "dotted":
                step_key = "sec_" + m.group(1).replace(".", "_")
                title = m.group(2)
            elif kind == "letter_num":
                letter, num, suf, title = m.group(1).lower(), m.group(2), m.group(3) or "", m.group(4)
                step_key = f"{letter}{int(num):02d}{suf}"
            elif kind == "block":
                step_key = m.group(1).lower()
                title = m.group(2)
            elif kind == "num":
                n = int(m.group(1))
                title = m.group(2)
                # skip pure intro numbered 0 with 导读
                step_key = f"n{n:02d}"
            break
        if step_key is None or title is None:
            continue
        title = strip_marks(title)
        if should_skip_title(title) or should_skip_title(line[3:]):
            continue
        # skip intro n00 style 导读 already handled
        if step_key == "n00" and re.search(r"导读", title):
            continue

        used_keys[step_key] += 1
        if used_keys[step_key] > 1:
            step_key = f"{step_key}_{used_keys[step_key]}"

        slug = slugify(title)
        used_slugs[slug] += 1
        if used_slugs[slug] > 1:
            slug = f"{slug}_{used_slugs[slug]}"

        class_name = f"{short_prefix}{java_ident(step_key)}{java_ident(slug)}"
        used_classes[class_name] += 1
        if used_classes[class_name] > 1:
            class_name = f"{class_name}{used_classes[class_name]}"
        if len(class_name) > 90:
            class_name = class_name[:90]

        topic_id = f"{stage_id}/{step_key}/{slug}"
        file_stem = f"{step_key}_{slug}"
        units.append(
            Unit(
                doc=doc_path.name,
                stage_dir=stage_dir,
                stage_id=stage_id,
                short_prefix=short_prefix,
                step_key=step_key,
                heading=title,
                slug=slug,
                topic_id=topic_id,
                class_name=class_name,
                file_stem=file_stem,
            )
        )
    return units


KT_TEMPLATE = """\
@file:JvmName("{facade}")

package learn.topics

import learn.registerTopic

// LearnKotlin placeholder
// Doc      : {doc}
// Stage    : {stage_dir}
// Step     : {step_key}
// Item     : {slug}
// Topic id : {topic_id}
// Heading  : {heading}
//
// TODO: read the KotlinStudy section, write a minimal runnable demo,
//       then replace this empty body with real practice code.

@Suppress("unused")
private val REGISTER = registerTopic("{topic_id}") {{ _ ->
    // practice: {heading}
    0
}}
"""

JAVA_TEMPLATE = """\
package learnj.topics.{stage_dir};

import learnj.TopicRegistry;

/**
 * LearnJava placeholder
 * <p>
 * Doc      : {doc}
 * Stage    : {stage_dir}
 * Step     : {step_key}
 * Item     : {slug}
 * Topic id : {topic_id}
 * Heading  : {heading}
 * <p>
 * TODO: read the JavaStudy section, write a minimal runnable demo,
 *       then replace this empty body with real practice code.
 */
public final class {class_name} {{
    static {{
        TopicRegistry.register("{topic_id}", args -> {{
            // practice: {heading}
            return 0;
        }});
    }}

    private {class_name}() {{
    }}
}}
"""


def gen_kotlin(units: list[Unit]) -> None:
    topics_root = ROOT / "src" / "kotlin" / "topics"
    learn = ROOT / "src" / "kotlin" / "learn"
    if topics_root.exists():
        shutil.rmtree(topics_root)
    topics_root.mkdir(parents=True)

    by_stage: dict[str, list[Unit]] = defaultdict(list)
    for u in units:
        by_stage[u.stage_dir].append(u)

    facades: list[str] = []
    for stage, us in by_stage.items():
        d = topics_root / stage
        d.mkdir(parents=True, exist_ok=True)
        (d / "README.md").write_text(
            f"# {stage}\n\nDoc: `{us[0].doc}`\n\nPrefix: `{us[0].stage_id}/…`\n",
            encoding="utf-8",
        )
        for u in us:
            facade = f"{u.class_name}KtFacade"
            facades.append(facade)
            body = KT_TEMPLATE.format(
                facade=facade,
                doc=u.doc,
                stage_dir=u.stage_dir,
                step_key=u.step_key,
                slug=u.slug,
                topic_id=u.topic_id,
                heading=u.heading.replace("*/", "* /"),
            )
            (d / f"{u.file_stem}.kt").write_text(body, encoding="utf-8")

    # loader
    lines = [
        "package learn",
        "",
        "/**",
        " * Force-loads every topic facade so [registerTopic] top-level vals run.",
        " */",
        "internal fun loadAllTopics() {",
    ]
    for f in facades:
        lines.append(f'    Class.forName("learn.topics.{f}")')
    lines.append("}")
    lines.append("")
    (learn / "GeneratedTopicLoader.kt").write_text("\n".join(lines), encoding="utf-8")

    idx = ["# LearnKotlin topic index", "", f"Total topics: {len(units)}", ""]
    for u in units:
        idx.append(f"- `{u.topic_id}` → `topics/{u.stage_dir}/{u.file_stem}.kt`")
    idx.append("")
    (ROOT / "src" / "kotlin" / "TOPIC_INDEX.md").write_text("\n".join(idx), encoding="utf-8")


def gen_java(units: list[Unit]) -> None:
    learnj = ROOT / "src" / "java" / "learnj"
    topics_root = learnj / "topics"
    if topics_root.exists():
        shutil.rmtree(topics_root)
    topics_root.mkdir(parents=True)

    # ensure core classes exist (do not wipe Main/Registry)
    by_stage: dict[str, list[Unit]] = defaultdict(list)
    for u in units:
        by_stage[u.stage_dir].append(u)

    fqn_list: list[str] = []
    for stage, us in by_stage.items():
        d = topics_root / stage
        d.mkdir(parents=True, exist_ok=True)
        (d / "README.md").write_text(
            f"# {stage}\n\nDoc: `{us[0].doc}`\n\nPrefix: `{us[0].stage_id}/…`\n",
            encoding="utf-8",
        )
        for u in us:
            fqn = f"learnj.topics.{u.stage_dir}.{u.class_name}"
            fqn_list.append(fqn)
            body = JAVA_TEMPLATE.format(
                stage_dir=u.stage_dir,
                doc=u.doc,
                step_key=u.step_key,
                slug=u.slug,
                topic_id=u.topic_id,
                heading=u.heading.replace("*/", "* /"),
                class_name=u.class_name,
            )
            (d / f"{u.class_name}.java").write_text(body, encoding="utf-8")

    lines = [
        "package learnj;",
        "",
        "/**",
        " * Force-loads every topic facade so {@code static} registration blocks run.",
        " */",
        "public final class GeneratedTopicLoader {",
        "    private GeneratedTopicLoader() {}",
        "",
        "    public static void loadAll() {",
        "        try {",
    ]
    for fqn in fqn_list:
        lines.append(f'            Class.forName("{fqn}");')
    lines += [
        "        } catch (ClassNotFoundException e) {",
        "            throw new ExceptionInInitializerError(e);",
        "        }",
        "    }",
        "}",
        "",
    ]
    (learnj / "GeneratedTopicLoader.java").write_text("\n".join(lines), encoding="utf-8")

    idx = ["# LearnJava topic index", "", f"Total topics: {len(units)}", ""]
    for u in units:
        idx.append(f"- `{u.topic_id}` → `learnj/topics/{u.stage_dir}/{u.class_name}.java`")
    idx.append("")
    (ROOT / "src" / "java" / "TOPIC_INDEX.md").write_text("\n".join(idx), encoding="utf-8")

    (ROOT / "src" / "java" / "README.md").write_text(
        """\
# LearnJava sources (`src/java`)

- Package `learnj` (isolated from Kotlin `learn.*`)
- Topics: `learnj.topics.<stage_dir>` matching directory path
- Entry: `learnj.Main` · `gradlew runJava`
""",
        encoding="utf-8",
    )


def collect(docs: Path, mapping: dict[str, tuple[str, str, str]]) -> list[Unit]:
    all_u: list[Unit] = []
    for doc_name, (stage_dir, stage_id, prefix) in mapping.items():
        path = docs / doc_name
        if not path.is_file():
            raise SystemExit(f"missing doc: {path}")
        us = parse_units(path, stage_dir, stage_id, prefix)
        print(f"  {doc_name}: {len(us)} units")
        all_u.extend(us)
    # uniqueness
    ids = [u.topic_id for u in all_u]
    if len(ids) != len(set(ids)):
        from collections import Counter

        dups = [k for k, v in Counter(ids).items() if v > 1]
        raise SystemExit(f"duplicate topic ids: {dups[:15]}")
    return all_u


def main() -> None:
    print("=== Kotlin ===")
    k = collect(KOTLIN_DOCS, KOTLIN_MAP)
    gen_kotlin(k)
    print(f"Kotlin total: {len(k)}")

    print("=== Java ===")
    j = collect(JAVA_DOCS, JAVA_MAP)
    gen_java(j)
    print(f"Java total: {len(j)}")

    summary = ROOT / "docs" / "coverage" / "REGEN_SUMMARY.md"
    summary.parent.mkdir(parents=True, exist_ok=True)
    summary.write_text(
        f"# Full regen summary\n\n"
        f"- LearnKotlin topics: **{len(k)}**\n"
        f"- LearnJava topics: **{len(j)}**\n"
        f"- Source: KotlinStudy + JavaStudy practice `##` units (步骤/§/N.N/A1/…)\n"
        f"- Skipped: pure 导读, 收尾, 一手资源索引, 附录链接\n",
        encoding="utf-8",
    )
    print("Wrote", summary)


if __name__ == "__main__":
    main()
