#!/usr/bin/env python3
"""Generate LearnJava topic placeholders from JavaStudy markdown docs.

Mirrors LearnKotlin: one registration unit per practice step (§ / 步骤 / N.N.N).
"""
from __future__ import annotations

import re
import unicodedata
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

DOCS = Path(r"C:\MyFile\ArcForges\ArchitectureDesign\JavaStudy")
ROOT = Path(__file__).resolve().parents[1]
JAVA_SRC = ROOT / "src" / "java"
# Package learnj (not learn) — coexists with Kotlin learn.* on the same classpath.
LEARN_DIR = JAVA_SRC / "learnj"
TOPICS_DIR = LEARN_DIR / "topics"

# Doc filename -> (stage_dir, stage_id_prefix, stage_slug, short_prefix)
DOC_MAP: dict[str, tuple[str, str, str, str]] = {
    "第2部分-阶段1-程序结构-JDK-JVM心智模型与工具链.md": (
        "p2_s01_structure",
        "part2/stage01",
        "structure",
        "P2S01Structure",
    ),
    "第2部分-阶段2-类型系统-基本类型-引用-数组-String-var-文本块.md": (
        "p2_s02_types",
        "part2/stage02",
        "types",
        "P2S02Types",
    ),
    "第2部分-阶段3-控制流与模式匹配.md": (
        "p2_s03_control",
        "part2/stage03",
        "control",
        "P2S03Control",
    ),
    "第2部分-阶段4-类与对象模型基础.md": (
        "p2_s04_classes",
        "part2/stage04",
        "classes",
        "P2S04Classes",
    ),
    "第2部分-阶段5-继承-多态-抽象类-Object契约.md": (
        "p2_s05_inheritance",
        "part2/stage05",
        "inheritance",
        "P2S05Inheritance",
    ),
    "第2部分-阶段6-接口-嵌套类-enum-record-sealed.md": (
        "p2_s06_interfaces",
        "part2/stage06",
        "interfaces",
        "P2S06Interfaces",
    ),
    "第2部分-阶段7-异常-断言-资源管理-try-with-resources.md": (
        "p2_s07_exceptions",
        "part2/stage07",
        "exceptions",
        "P2S07Exceptions",
    ),
    "第2部分-阶段8-泛型-类型擦除-通配符-PECS-桥接方法.md": (
        "p2_s08_generics",
        "part2/stage08",
        "generics",
        "P2S08Generics",
    ),
    "第2部分-阶段9-Lambda-方法引用-Optional-Stream-Collector.md": (
        "p2_s09_lambda_stream",
        "part2/stage09",
        "lambda_stream",
        "P2S09LambdaStream",
    ),
    "第2部分-阶段10-集合框架-不可变集合与Sequenced-Collections.md": (
        "p2_s10_collections",
        "part2/stage10",
        "collections",
        "P2S10Collections",
    ),
    "第2部分-阶段11-并发基础与Java-Memory-Model.md": (
        "p2_s11_concurrency",
        "part2/stage11",
        "concurrency",
        "P2S11Concurrency",
    ),
    "第2部分-阶段12-Virtual-Threads-Structured-Concurrency-Scoped-Values-Loom.md": (
        "p2_s12_loom",
        "part2/stage12",
        "loom",
        "P2S12Loom",
    ),
    "第2部分-阶段13-关键字完整表-语法特性分类总表-现代写法收口.md": (
        "p2_s13_keywords",
        "part2/stage13",
        "keywords",
        "P2S13Keywords",
    ),
    "第2部分-阶段14-Effective级惯用法与API设计.md": (
        "p2_s14_effective",
        "part2/stage14",
        "effective",
        "P2S14Effective",
    ),
    "第3部分-标准库系统化.md": (
        "p3_stdlib",
        "part3/stdlib",
        "stdlib",
        "P3Stdlib",
    ),
    "第4部分-工程系统.md": (
        "p4_engineering",
        "part4/engineering",
        "engineering",
        "P4Engineering",
    ),
    "第5部分-Java演进史.md": (
        "p5_evolution",
        "part5/evolution",
        "evolution",
        "P5Evolution",
    ),
    "第6部分-支线A-JPMS模块系统.md": (
        "p6_branch_a_jpms",
        "part6/branch_a",
        "jpms",
        "P6AJpms",
    ),
    "第6部分-支线B-反射注解动态代理-MethodHandle-SPI.md": (
        "p6_branch_b_reflect",
        "part6/branch_b",
        "reflect",
        "P6BReflect",
    ),
    "第6部分-支线C-Class文件-字节码-类加载.md": (
        "p6_branch_c_classfile",
        "part6/branch_c",
        "classfile",
        "P6CClassfile",
    ),
    "第6部分-支线D-JVM架构-GC-对象布局.md": (
        "p6_branch_d_jvm_gc",
        "part6/branch_d",
        "jvm_gc",
        "P6DJvmGc",
    ),
    "第6部分-支线E-JIT-逃逸分析-运行时优化-CDS-AOT.md": (
        "p6_branch_e_jit_aot",
        "part6/branch_e",
        "jit_aot",
        "P6EJitAot",
    ),
    "第7部分-性能工程线.md": (
        "p7_perf",
        "part7/perf",
        "perf",
        "P7Perf",
    ),
    "第8部分-本地互操作-JNI-FFM-Vector-API.md": (
        "p8_native",
        "part8/native",
        "native",
        "P8Native",
    ),
}

SKIP_HEADING = re.compile(
    r"^(收尾|下一步|有意后置|与路线图|目录与路线图|下一阶段|下一支线|下一章)"
)

# Practice ## headings we keep
STEP_RE = re.compile(
    r"^##\s+(?:"
    r"步骤\s*(\d+)\s*[·\.、:：]\s*(.+)"  # 步骤 N · title
    r"|§\s*(\d+)\s*[·\.、:：]\s*(.+)"  # §N · title
    r"|(\d+(?:\.\d+)+)\s*[·\.、:：]?\s*(.+)"  # 3.1.1 title / 5.1 · title
    r")\s*$"
)


def strip_emoji_and_marks(s: str) -> str:
    s = re.sub(r"[🟢🟡🔴⭐🆕⚠️🔷🔶📖🔁]", "", s)
    s = re.sub(r"\s+", " ", s).strip()
    return s


# Common Chinese heading fragments -> stable English slug tokens
_ZH_TOKEN_MAP: list[tuple[str, str]] = [
    ("工具就位", "toolchain_ready"),
    ("工具约定", "tooling"),
    ("工具与", "tooling"),
    ("验收", "acceptance"),
    ("练习", "practice"),
    ("一手资源", "primary_sources"),
    ("资源索引", "resource_index"),
    ("易错", "pitfalls"),
    ("对照", "cpp_contrast"),
    ("完成标志", "done_flags"),
    ("不要做", "do_not"),
    ("心智", "mental_model"),
    ("纪律", "discipline"),
    ("全景", "overview"),
    ("总览", "overview"),
    ("总表", "table"),
    ("选择指南", "selection_guide"),
    ("选择", "choice"),
    ("边界", "boundary"),
    ("预告", "preview"),
    ("动机", "motivation"),
    ("为什么", "why"),
    ("是什么", "what_is"),
    ("如何", "how"),
    ("最小", "minimal"),
    ("完整", "full"),
    ("基础", "basics"),
    ("进阶", "advanced"),
    ("并发", "concurrency"),
    ("集合", "collections"),
    ("泛型", "generics"),
    ("异常", "exceptions"),
    ("接口", "interfaces"),
    ("继承", "inheritance"),
    ("多态", "polymorphism"),
    ("抽象", "abstract"),
    ("封装", "encapsulation"),
    ("构造", "constructors"),
    ("初始化", "init"),
    ("包", "package"),
    ("模块", "module"),
    ("反射", "reflection"),
    ("注解", "annotations"),
    ("代理", "proxy"),
    ("字节码", "bytecode"),
    ("类加载", "classloading"),
    ("垃圾回收", "gc"),
    ("对象布局", "object_layout"),
    ("逃逸分析", "escape_analysis"),
    ("性能", "performance"),
    ("微基准", "jmh"),
    ("火焰图", "flame_graph"),
    ("本地互操作", "native_interop"),
    ("受限", "restricted"),
    ("不可变", "immutable"),
    ("虚拟线程", "virtual_threads"),
    ("结构化并发", "structured_concurrency"),
    ("模式匹配", "pattern_matching"),
    ("文本块", "text_blocks"),
    ("类型擦除", "type_erasure"),
    ("通配符", "wildcards"),
    ("桥接", "bridge"),
    ("方法引用", "method_ref"),
    ("流", "stream"),
    ("可选", "optional"),
    ("断言", "assert"),
    ("资源管理", "resource_mgmt"),
    ("关键字", "keywords"),
    ("惯用法", "idioms"),
    ("序列化", "serialization"),
    ("安全", "security"),
    ("网络", "network"),
    ("时间", "time"),
    ("国际化", "i18n"),
    ("构建", "build"),
    ("测试", "testing"),
    ("打包", "packaging"),
    ("演进", "evolution"),
    ("废弃", "deprecated"),
    ("实验", "experimental"),
    ("内存模型", "jmm"),
    ("监视器", "monitor"),
    ("中断", "interrupt"),
    ("线程池", "thread_pool"),
    ("锁", "lock"),
    ("原子", "atomic"),
    ("泄漏", "leak"),
    ("容器", "container"),
    ("观测", "observability"),
    ("训练", "training"),
    ("启动", "startup"),
    ("预热", "warmup"),
    ("诊断", "diagnostics"),
    ("日志", "logging"),
    ("案例", "case_study"),
    ("指标", "metrics"),
    ("方法论", "methodology"),
]


def slugify(text: str, max_len: int = 48) -> str:
    import hashlib

    text = strip_emoji_and_marks(text)
    tokens: list[str] = []
    for m in re.finditer(r"`([^`]+)`", text):
        part = re.sub(r"[^A-Za-z0-9]+", "_", m.group(1)).strip("_").lower()
        if part and part not in tokens:
            tokens.append(part)
    # bare Java-ish / English words (keep jdk/jre/jvm/java)
    stop = {"vs", "the", "and", "or", "to", "of", "in", "for", "with", "a", "an"}
    for m in re.finditer(r"[A-Za-z][A-Za-z0-9_.$/-]{0,}", text):
        w = m.group(0).replace(".", "_").replace("-", "_").replace("/", "_").lower()
        w = re.sub(r"_+", "_", w).strip("_")
        if w and w not in tokens and w not in stop and len(w) >= 2:
            tokens.append(w)
    # Chinese keyword map (order of map matters: longer phrases first)
    for zh, en in _ZH_TOKEN_MAP:
        if zh in text and en not in tokens:
            tokens.append(en)
    if not tokens:
        digest = hashlib.sha1(text.encode("utf-8")).hexdigest()[:8]
        return f"topic_{digest}"
    slug = "_".join(tokens)
    slug = re.sub(r"_+", "_", slug).strip("_")
    if len(slug) > max_len:
        slug = slug[:max_len].rstrip("_")
    return slug or "topic"


def java_ident(s: str) -> str:
    """PascalCase-ish from slug, safe Java type identifier."""
    parts = [p for p in re.split(r"[^A-Za-z0-9]+", s) if p]
    if not parts:
        return "Topic"
    out = "".join(p[:1].upper() + p[1:] if p else "" for p in parts)
    if out[0].isdigit():
        out = "T" + out
    # max class name length keep reasonable
    if len(out) > 60:
        out = out[:60]
    return out


@dataclass
class Topic:
    doc: str
    stage_dir: str
    stage_id: str
    short_prefix: str
    step_key: str  # s00, s01, sec_3_1_1
    step_num_label: str
    heading: str
    slug: str
    topic_id: str
    class_name: str
    file_name: str  # relative under topics/


def parse_doc(doc_name: str, mapping: tuple[str, str, str, str]) -> list[Topic]:
    stage_dir, stage_id, _stage_slug, short_prefix = mapping
    path = DOCS / doc_name
    text = path.read_text(encoding="utf-8")
    topics: list[Topic] = []
    used_slugs: dict[str, int] = defaultdict(int)
    used_classes: dict[str, int] = defaultdict(int)

    for line in text.splitlines():
        m = STEP_RE.match(line.strip())
        if not m:
            continue
        if m.group(1) is not None:
            num, title = m.group(1), m.group(2)
            step_key = f"s{int(num):02d}"
            step_label = f"步骤 {num}"
        elif m.group(3) is not None:
            num, title = m.group(3), m.group(4)
            step_key = f"s{int(num):02d}"
            step_label = f"§{num}"
        else:
            num, title = m.group(5), m.group(6)
            # 3.1.1 -> sec_3_1_1
            step_key = "sec_" + num.replace(".", "_")
            step_label = num

        title = strip_emoji_and_marks(title)
        if SKIP_HEADING.match(title):
            continue
        # skip pure empty
        if not title:
            continue

        slug = slugify(title)
        used_slugs[slug] += 1
        if used_slugs[slug] > 1:
            slug = f"{slug}_{used_slugs[slug]}"

        class_base = f"{short_prefix}{java_ident(step_key)}_{java_ident(slug)}"
        # simplify class: ShortPrefix + Step + Slug
        class_name = f"{short_prefix}{java_ident(step_key)}{java_ident(slug)}"
        used_classes[class_name] += 1
        if used_classes[class_name] > 1:
            class_name = f"{class_name}{used_classes[class_name]}"
        if len(class_name) > 80:
            class_name = class_name[:80]

        topic_id = f"{stage_id}/{step_key}/{slug}"
        # public class name must match .java basename
        file_name = f"{stage_dir}/{class_name}.java"

        topics.append(
            Topic(
                doc=doc_name,
                stage_dir=stage_dir,
                stage_id=stage_id,
                short_prefix=short_prefix,
                step_key=step_key,
                step_num_label=step_label,
                heading=title,
                slug=slug,
                topic_id=topic_id,
                class_name=class_name,
                file_name=file_name,
            )
        )
    return topics


JAVA_TOPIC_TEMPLATE = """\
package learnj.topics;

import learnj.TopicRegistry;

/**
 * LearnJava placeholder
 * <p>
 * Doc      : {doc}
 * Stage    : {stage_dir}
 * Step     : {step_label}
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

README_STAGE = """\
# {stage_dir}

Source: `{doc}`

Topic prefix: `{stage_id}/…`

See `src/java/TOPIC_INDEX.md` for full ids.
"""


def write_core() -> None:
    LEARN_DIR.mkdir(parents=True, exist_ok=True)
    (LEARN_DIR / "TopicRegistry.java").write_text(
        """\
package learnj;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Topic registry for LearnJava — same job as Kotlin {@code learn.TopicRegistry}
 * and C++ {@code learn::topic}.
 *
 * <p>Package {@code learnj} avoids clashing with Kotlin {@code learn.*} on the same
 * classpath (both live under {@code src/java} + {@code src/kotlin}).
 *
 * <p>Each placeholder class registers in a {@code static} block. JVM only loads a
 * class when first referenced, so {@link GeneratedTopicLoader#loadAll()}
 * force-loads every facade before main dispatches.
 */
public final class TopicRegistry {
    @FunctionalInterface
    public interface TopicFn {
        int run(String[] args);
    }

    private static final Map<String, TopicFn> TOPICS = new LinkedHashMap<>();

    private TopicRegistry() {}

    public static void register(String id, TopicFn fn) {
        TopicFn prev = TOPICS.put(id, fn);
        if (prev != null) {
            throw new IllegalStateException("duplicate topic id: " + id);
        }
    }

    public static int run(String id, String[] args) {
        TopicFn fn = TOPICS.get(id);
        if (fn == null) {
            System.err.println("learn_java: unknown topic '" + id + "'");
            System.err.println("  run with no args to iterate all; or --list");
            return 2;
        }
        return fn.run(args);
    }

    public static List<String> list() {
        return new ArrayList<>(TOPICS.keySet());
    }

    public static int size() {
        return TOPICS.size();
    }

    public static void listTopics() {
        System.out.println("learn_java: " + TOPICS.size() + " topics registered");
        for (String id : TOPICS.keySet()) {
            System.out.println("  " + id);
        }
    }

    /**
     * No topic id: iterate the whole map (C++ Debug / Kotlin empty-args path).
     * IDE Run/Debug with empty program args lands in every topic that has a breakpoint.
     */
    public static int runAll(String[] args) {
        int failures = 0;
        System.out.println("learn_java [debug]: iterating " + TOPICS.size() + " topics");
        for (Map.Entry<String, TopicFn> e : TOPICS.entrySet()) {
            int rc = e.getValue().run(args);
            if (rc != 0) {
                System.err.println("  ! " + e.getKey() + " returned " + rc);
                failures++;
            }
        }
        if (failures > 0) {
            System.err.println("learn_java [debug]: " + failures + " topic(s) returned non-zero");
        }
        return failures == 0 ? 0 : 1;
    }
}
""",
        encoding="utf-8",
    )

    (LEARN_DIR / "Main.java").write_text(
        "\n".join(
            [
                "package learnj;",
                "",
                "/**",
                " * Single entry — same role as {@code src/cpp/main.cpp} and Kotlin {@code learn.MainKt}.",
                " *",
                " * <p>Package {@code learnj} avoids clashing with Kotlin {@code learn.*}.",
                " *",
                " * <p>Empty argv (IDE Run / Debug with no program args): walk every registered topic",
                " * so a breakpoint in any placeholder hits.",
                " *",
                " * <p>Optional CLI: {@code topic_id}, {@code --list}, {@code --help}.",
                " */",
                "public final class Main {",
                "    private Main() {}",
                "",
                "    public static void main(String[] args) {",
                "        GeneratedTopicLoader.loadAll();",
                "",
                "        if (args.length == 0) {",
                "            System.exit(TopicRegistry.runAll(args));",
                "        }",
                "",
                "        String first = args[0];",
                '        if ("--help".equals(first) || "-h".equals(first) || "help".equals(first)) {',
                "            System.out.println(",
                '                    """',
                "                    usage: learn_java [topic_id [extra_args...]]",
                "                      (no args)   iterate every topic — IDE Run/Debug with empty args",
                "                      topic_id    run that topic only",
                "                      --list      list registered topic ids",
                "                      --help/-h   this message",
                '                    """.stripIndent());',
                "            return;",
                "        }",
                '        if ("--list".equals(first) || "list".equals(first)) {',
                "            TopicRegistry.listTopics();",
                "            return;",
                "        }",
                "",
                "        String[] rest = new String[args.length - 1];",
                "        System.arraycopy(args, 1, rest, 0, rest.length);",
                "        System.exit(TopicRegistry.run(first, rest));",
                "    }",
                "}",
                "",
            ]
        ),
        encoding="utf-8",
    )


def write_loader(topics: list[Topic]) -> None:
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
    for t in topics:
        lines.append(f'            Class.forName("learnj.topics.{t.class_name}");')
    lines += [
        "        } catch (ClassNotFoundException e) {",
        "            throw new ExceptionInInitializerError(e);",
        "        }",
        "    }",
        "}",
        "",
    ]
    (LEARN_DIR / "GeneratedTopicLoader.java").write_text("\n".join(lines), encoding="utf-8")


def main() -> None:
    if not DOCS.is_dir():
        raise SystemExit(f"JavaStudy docs not found: {DOCS}")

    all_topics: list[Topic] = []
    for doc_name, mapping in DOC_MAP.items():
        if not (DOCS / doc_name).is_file():
            raise SystemExit(f"missing doc: {doc_name}")
        ts = parse_doc(doc_name, mapping)
        print(f"{doc_name}: {len(ts)} topics")
        all_topics.extend(ts)

    # uniqueness checks
    ids = [t.topic_id for t in all_topics]
    classes = [t.class_name for t in all_topics]
    if len(ids) != len(set(ids)):
        from collections import Counter

        dups = [k for k, v in Counter(ids).items() if v > 1]
        raise SystemExit(f"duplicate topic ids: {dups[:10]}")
    if len(classes) != len(set(classes)):
        from collections import Counter

        dups = [k for k, v in Counter(classes).items() if v > 1]
        raise SystemExit(f"duplicate class names: {dups[:10]}")

    # clean previous layout (learn/ or learnj/)
    import shutil

    for stale in (JAVA_SRC / "learn", JAVA_SRC / "topics", JAVA_SRC / "learnj"):
        if stale.exists():
            shutil.rmtree(stale)
    LEARN_DIR.mkdir(parents=True, exist_ok=True)
    TOPICS_DIR.mkdir(parents=True, exist_ok=True)

    write_core()
    write_loader(all_topics)

    by_stage: dict[str, list[Topic]] = defaultdict(list)
    for t in all_topics:
        by_stage[t.stage_dir].append(t)

    for stage_dir, ts in by_stage.items():
        d = TOPICS_DIR / stage_dir
        d.mkdir(parents=True, exist_ok=True)
        (d / "README.md").write_text(
            README_STAGE.format(
                stage_dir=stage_dir,
                doc=ts[0].doc,
                stage_id=ts[0].stage_id,
            ),
            encoding="utf-8",
        )
        for t in ts:
            body = JAVA_TOPIC_TEMPLATE.format(
                doc=t.doc,
                stage_dir=t.stage_dir,
                step_label=t.step_num_label,
                slug=t.slug,
                topic_id=t.topic_id,
                heading=t.heading.replace("*/", "* /"),
                class_name=t.class_name,
            )
            (TOPICS_DIR / t.file_name).write_text(body, encoding="utf-8")

    # TOPIC_INDEX
    idx = ["# LearnJava topic index", "", f"Total topics: {len(all_topics)}", ""]
    for t in all_topics:
        idx.append(f"- `{t.topic_id}` → `topics/{t.file_name}`")
    idx.append("")
    (JAVA_SRC / "TOPIC_INDEX.md").write_text("\n".join(idx), encoding="utf-8")

    (JAVA_SRC / "README.md").write_text(
        """\
# LearnJava sources (`src/java`)

Source dir is set in root `build.gradle.kts` (`sourceSets.main.java.srcDirs`).
Parallel to `src/cpp/` and `src/kotlin/`. Open **repo root** in IDEA.

- Run config: `.run/LearnJava_Main.run.xml` (direct main) or `.run/LearnJava.run.xml` (Gradle `runJava`)
- Entry: `learnj.Main` (package `learnj` — isolated from Kotlin `learn.*`)
- Registry: `learnj.TopicRegistry` + `learnj.GeneratedTopicLoader`
- Topics: `topics/**` — one class per JavaStudy practice step (`learnj.topics`)

```bat
gradlew.bat runJava
gradlew.bat runJava --args="--list"
gradlew.bat runJava --args="part2/stage01/s01/jdk_jre_jvm"
```
""",
        encoding="utf-8",
    )

    print(f"\nGenerated {len(all_topics)} topics under {TOPICS_DIR}")


if __name__ == "__main__":
    main()
