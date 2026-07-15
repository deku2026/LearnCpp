#!/usr/bin/env python3
"""One-shot CI portability fixes for LearnCpp topic demos.

1) Keep NDEBUG for main() (list topics), but make assert() always-live via
   topic_registry.hpp. Strip re-#include <cassert> which would reinstall the
   NDEBUG no-op assert and revive -Wunused-* under -Werror.

2) Gate C++23 library APIs / maybe-missing headers so older libstdc++/libc++
   (Linux/macOS CI) compile. Feature-specific includes move inside #else so
   missing <expected>/<stacktrace>/... never hard-fail the #include line.
"""
from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CPP = ROOT / "src" / "cpp"

# API use -> (feature macro, optional maybe-missing header name)
FEATURES: list[tuple[str, str, str | None]] = [
    (r"\bstd::bind_back\b", "__cpp_lib_bind_back", None),
    (r"\bstd::move_only_function\b", "__cpp_lib_move_only_function", None),
    (r"\bstd::generator\b", "__cpp_lib_generator", "generator"),
    (r"\bstd::flat_map\b", "__cpp_lib_flat_map", "flat_map"),
    (r"\bstd::flat_set\b", "__cpp_lib_flat_set", "flat_set"),
    (r"\bstd::flat_multimap\b", "__cpp_lib_flat_map", "flat_map"),
    (r"\bstd::flat_multiset\b", "__cpp_lib_flat_set", "flat_set"),
    (r"\bstd::println\b", "__cpp_lib_print", "print"),
    (r"\bstd::print\s*\(", "__cpp_lib_print", "print"),
    (r"\bstd::stacktrace\b", "__cpp_lib_stacktrace", "stacktrace"),
    (r"\bstd::expected\b", "__cpp_lib_expected", "expected"),
    (r"\bstd::unexpected\b", "__cpp_lib_expected", "expected"),
    (r"\bstd::out_ptr\b", "__cpp_lib_out_ptr", None),
    (r"\bstd::inout_ptr\b", "__cpp_lib_out_ptr", None),
    (r"\bstd::byteswap\b", "__cpp_lib_byteswap", None),
    (r"\bstd::to_underlying\b", "__cpp_lib_to_underlying", None),
    (r"\bstd::start_lifetime_as(?:_array)?\b", "__cpp_lib_start_lifetime_as", None),
    (r"\bstd::unreachable\s*\(", "__cpp_lib_unreachable", None),
    (r"\bstd::is_scoped_enum_v\b", "__cpp_lib_is_scoped_enum", None),
    (r"\bstd::ispanstream\b", "__cpp_lib_spanstream", "spanstream"),
    (r"\bstd::ospanstream\b", "__cpp_lib_spanstream", "spanstream"),
    (r"\bstd::spanstream\b", "__cpp_lib_spanstream", "spanstream"),
    (r"\bstd::mdspan\b", "__cpp_lib_mdspan", "mdspan"),
]

# Headers that must not appear at global depth without a feature gate.
MISSING_HEADERS = {
    "expected": "__cpp_lib_expected",
    "stacktrace": "__cpp_lib_stacktrace",
    "print": "__cpp_lib_print",
    "generator": "__cpp_lib_generator",
    "flat_map": "__cpp_lib_flat_map",
    "flat_set": "__cpp_lib_flat_set",
    "mdspan": "__cpp_lib_mdspan",
    "spanstream": "__cpp_lib_spanstream",
}


def strip_cassert(text: str) -> str:
    """Remove #include <cassert>; topic_registry provides always-on assert."""
    return re.sub(r"^[ \t]*#include\s*<cassert>\s*\n", "", text, flags=re.M)


def topic_id(text: str) -> str:
    m = re.search(r'::learn::topic<"([^"]+)"', text)
    return m.group(1) if m else "unknown/topic"


def has_macro_gate(text: str, macro: str) -> bool:
    return (
        f"!defined({macro})" in text
        or f"defined({macro})" in text
        or re.search(rf"#if\s+{re.escape(macro)}\b", text) is not None
    )


def find_needed_feature(text: str) -> tuple[str, str, str | None] | None:
    """First unguarded feature used in this file."""
    for pat, macro, header in FEATURES:
        if not re.search(pat, text):
            continue
        if has_macro_gate(text, macro):
            continue
        if f"[skip]" in text and macro.split("_")[-1] in text:
            # weak signal; still prefer explicit gate
            pass
        return pat, macro, header
    return None


def needs_header_gate_fix(text: str) -> str | None:
    """If a maybe-missing header is included before any feature gate, return macro."""
    # Prefer first missing header that has a matching use / include
    for header, macro in MISSING_HEADERS.items():
        if f"#include <{header}>" not in text:
            continue
        # Already has_include-protected?
        if f"__has_include(<{header}>)" in text:
            continue
        # If whole file already gated with this macro AND include is only in #else — OK
        # Detect include at preprocessor depth 0
        depth = 0
        for ln in text.splitlines():
            s = ln.lstrip()
            if s.startswith("#if"):
                depth += 1
            elif s.startswith("#endif"):
                depth = max(0, depth - 1)
            elif f"#include <{header}>" in ln and depth == 0:
                return macro
    return None


def split_head_rest(text: str) -> tuple[str, str]:
    lines = text.splitlines(keepends=True)
    insert_at = 0
    for i, ln in enumerate(lines):
        s = ln.lstrip()
        if (
            s.startswith("#include")
            or s.startswith("#if")
            or s.startswith("#endif")
            or s.startswith("#define")
            or s.startswith("#ifndef")
            or s.startswith("#pragma")
            or s.startswith("#  ")
            or s.startswith("#else")
            or s.startswith("#elif")
            or s.startswith("#undef")
        ):
            insert_at = i + 1
        elif not s or s.startswith("//"):
            if insert_at:
                insert_at = i + 1
        elif insert_at:
            break
    return "".join(lines[:insert_at]), "".join(lines[insert_at:])


def ensure_safe_head(head: str) -> str:
    if '#include "learn/topic_registry.hpp"' not in head:
        return head
    if "#include <version>" not in head:
        head = head.replace(
            '#include "learn/topic_registry.hpp"\n',
            '#include "learn/topic_registry.hpp"\n\n#include <version>\n',
            1,
        )
    if "#include <iostream>" not in head:
        head = head.replace(
            '#include "learn/topic_registry.hpp"\n',
            '#include "learn/topic_registry.hpp"\n\n#include <iostream>\n',
            1,
        )
    return head


def strip_missing_headers_from_head(head: str) -> tuple[str, list[str]]:
    """Remove maybe-missing includes from head; return them for #else."""
    moved: list[str] = []
    lines = head.splitlines(keepends=True)
    out: list[str] = []
    for ln in lines:
        m = re.match(r'^[ \t]*#include\s*<([a-zA-Z0-9_]+)>\s*$', ln)
        if m and m.group(1) in MISSING_HEADERS:
            moved.append(m.group(1))
            continue
        out.append(ln)
    return "".join(out), moved


def unwrap_existing_feature_gate(text: str) -> str:
    """If file already has our skip/#else/#endif gate, unwrap to raw content for re-wrap."""
    # Pattern produced by older/newer scripts
    m = re.search(
        r"\n#if\s+!defined\((\w+)\)\s*\|\|\s*!\(\1\)[^\n]*\n"
        r"namespace\s*\{\s*\n"
        r"int run\(int[^\n]*\n"
        r"\s*std::cout\s*<<\s*\"\[skip\].*?\n"
        r"\s*return 0;\s*\n"
        r"\}\s*\n"
        r"\[\[maybe_unused\]\][^\n]*\n"
        r"\}\s*//\s*namespace\s*\n"
        r"#else\s*\n",
        text,
        flags=re.S,
    )
    if not m:
        # try looser
        m = re.search(
            r"\n#if\s+!defined\((\w+)\)\s*\|\|\s*!\(\1\)[^\n]*\n.*?\[skip\].*?\n#else\s*\n",
            text,
            flags=re.S,
        )
    if not m:
        return text
    macro = m.group(1)
    # remove gate start through #else
    text2 = text[: m.start()] + "\n" + text[m.end() :]
    # remove trailing #endif // MACRO
    text2 = re.sub(
        rf"\n#endif\s*//\s*{re.escape(macro)}\s*\n?\s*$",
        "\n",
        text2,
    )
    return text2


def wrap_feature(text: str, macro: str, name: str, header: str | None) -> str:
    text = unwrap_existing_feature_gate(text)
    tid = topic_id(text)
    head, rest = split_head_rest(text)
    head = ensure_safe_head(head)
    head, moved = strip_missing_headers_from_head(head)

    has_inc = ""
    if header:
        has_inc = f" || !__has_include(<{header}>)"
        if header not in moved:
            moved.append(header)

    # Also strip any remaining missing headers that appear at start of rest
    rest_lines = rest.splitlines(keepends=True)
    rest_out: list[str] = []
    i = 0
    while i < len(rest_lines):
        ln = rest_lines[i]
        m = re.match(r'^[ \t]*#include\s*<([a-zA-Z0-9_]+)>\s*$', ln)
        if m and m.group(1) in MISSING_HEADERS:
            if m.group(1) not in moved:
                moved.append(m.group(1))
            i += 1
            continue
        break
    rest_out = rest_lines[i:]
    rest = "".join(rest_out)

    else_includes = "".join(f"#include <{h}>\n" for h in moved)

    stub = f"""
#if !defined({macro}) || !({macro}){has_inc}
namespace {{
int run(int /*argc*/, char** /*argv*/) {{
    std::cout << "[skip] {name} not available ({macro})\\n";
    return 0;
}}
[[maybe_unused]] const auto& _ = ::learn::topic<"{tid}", run>;
}}  // namespace
#else
{else_includes}"""

    out = head.rstrip() + "\n" + stub + rest
    if not re.search(rf"#endif\s*//\s*{re.escape(macro)}\s*$", out.rstrip()):
        out = out.rstrip() + f"\n#endif  // {macro}\n"
    return out


def fix_scoped_enum_only(text: str) -> str:
    """Local-guard is_scoped_enum_v uses without whole-file skip (often mixed demos)."""
    if "is_scoped_enum_v" not in text:
        return text
    if has_macro_gate(text, "__cpp_lib_is_scoped_enum"):
        return text
    # Wrap each static_assert / assert line containing is_scoped_enum_v
    def repl(m: re.Match[str]) -> str:
        line = m.group(0)
        indent = re.match(r"[ \t]*", line).group(0)
        return (
            f"{indent}#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum\n"
            f"{line}"
            f"{indent}#endif\n"
        )

    return re.sub(
        r"^[ \t]*(?:static_)?assert\([^;]*is_scoped_enum_v[^;]*\);\s*\n",
        repl,
        text,
        flags=re.M,
    )


def fix_file(path: Path) -> list[str]:
    actions: list[str] = []
    orig = path.read_text(encoding="utf-8", errors="replace")
    text = orig

    # 1) strip cassert reinclude
    new = strip_cassert(text)
    if new != text:
        actions.append("strip_cassert")
        text = new

    # 2) local is_scoped_enum when file is not primarily about that feature
    # Prefer whole-file gate only if that's the only C++23 API; else local.
    if re.search(r"\bis_scoped_enum_v\b", text) and not has_macro_gate(
        text, "__cpp_lib_is_scoped_enum"
    ):
        # If file also needs other gates, still local-wrap is_scoped_enum
        other = find_needed_feature(
            re.sub(r"\bis_scoped_enum_v\b", "IS_SCOPED_ENUM_PLACEHOLDER", text)
        )
        if other is None and not has_macro_gate(text, "__cpp_lib_is_scoped_enum"):
            # whole file could be gated — but local is safer for mixed enum demos
            pass
        new = fix_scoped_enum_only(text)
        if new != text:
            actions.append("local_is_scoped_enum")
            text = new

    # 3) feature gates (may loop for multi-feature files — one primary skip)
    for _ in range(4):
        # Prefer fixing missing-header placement for already-gated files
        hdr_macro = needs_header_gate_fix(text)
        if hdr_macro:
            # Find feature name for macro
            name = hdr_macro
            header = None
            for _pat, macro, hdr in FEATURES:
                if macro == hdr_macro:
                    name = hdr or macro
                    header = hdr
                    break
            for h, m in MISSING_HEADERS.items():
                if m == hdr_macro:
                    header = h
                    name = h
                    break
            text = wrap_feature(text, hdr_macro, f"std::{name}" if not name.startswith("std") else name, header)
            actions.append(f"rewrap_header:{hdr_macro}")
            continue

        needed = find_needed_feature(text)
        if not needed:
            break
        _pat, macro, header = needed
        # human name
        name = macro.replace("__cpp_lib_", "std::")
        for pat, m, h in FEATURES:
            if m == macro:
                # better name from first matching feature
                if "bind_back" in pat:
                    name = "std::bind_back"
                elif "move_only" in pat:
                    name = "std::move_only_function"
                elif "expected" in pat or "unexpected" in pat:
                    name = "std::expected"
                elif "println" in pat:
                    name = "std::println"
                elif "print" in pat:
                    name = "std::print"
                elif "stacktrace" in pat:
                    name = "std::stacktrace"
                elif "generator" in pat:
                    name = "std::generator"
                elif "flat_map" in pat:
                    name = "std::flat_map"
                elif "flat_set" in pat:
                    name = "std::flat_set"
                elif "to_underlying" in pat:
                    name = "std::to_underlying"
                elif "byteswap" in pat:
                    name = "std::byteswap"
                elif "out_ptr" in pat:
                    name = "std::out_ptr"
                elif "unreachable" in pat:
                    name = "std::unreachable"
                elif "spanstream" in pat or "ispanstream" in pat:
                    name = "std::spanstream"
                elif "mdspan" in pat:
                    name = "std::mdspan"
                header = h
                break
        text = wrap_feature(text, macro, name, header)
        actions.append(f"gate:{macro}")

    # 4) For already-gated files where includes still sit before #if, move them
    hdr_macro = needs_header_gate_fix(text)
    if hdr_macro:
        header = None
        name = "feature"
        for h, m in MISSING_HEADERS.items():
            if m == hdr_macro:
                header = h
                name = f"std::{h}"
                break
        text = wrap_feature(text, hdr_macro, name, header)
        actions.append(f"final_rewrap:{hdr_macro}")

    if text != orig:
        # normalize newlines
        if not text.endswith("\n"):
            text += "\n"
        path.write_text(text, encoding="utf-8", newline="\n")
    return actions


def main() -> None:
    stats: dict[str, int] = {}
    changed = 0
    for path in sorted(CPP.rglob("*.cpp")):
        actions = fix_file(path)
        if actions:
            changed += 1
            for a in actions:
                stats[a.split(":")[0]] = stats.get(a.split(":")[0], 0) + 1
            rel = path.relative_to(ROOT)
            print(f"{rel}: {', '.join(actions)}")
    print(f"changed_files={changed}")
    print("stats", stats)


if __name__ == "__main__":
    main()
