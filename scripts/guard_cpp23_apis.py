#!/usr/bin/env python3
"""Gate C++23-only APIs for older libstdc++/libc++ (Linux/macOS CI).

Wraps entire topic body when a feature is required and unguarded.
Re-runnable: skips already-guarded files.
"""
from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1] / "src" / "cpp"

# (regex to detect use, feature macro name, display name)
FEATURES = [
    (r"\bstd::bind_back\b", "__cpp_lib_bind_back", "std::bind_back"),
    (r"\bstd::move_only_function\b", "__cpp_lib_move_only_function", "std::move_only_function"),
    (r"\bstd::generator\b", "__cpp_lib_generator", "std::generator"),
    (r"\bstd::flat_map\b", "__cpp_lib_flat_map", "std::flat_map"),
    (r"\bstd::flat_set\b", "__cpp_lib_flat_set", "std::flat_set"),
    (r"\bstd::flat_multimap\b", "__cpp_lib_flat_map", "std::flat_multimap"),
    (r"\bstd::flat_multiset\b", "__cpp_lib_flat_set", "std::flat_multiset"),
    (r"\bstd::println\b", "__cpp_lib_print", "std::println"),
    (r"\bstd::print\s*\(", "__cpp_lib_print", "std::print"),
    (r"\bstd::stacktrace\b", "__cpp_lib_stacktrace", "std::stacktrace"),
    (r"\bstd::expected\b", "__cpp_lib_expected", "std::expected"),
    (r"\bstd::out_ptr\b", "__cpp_lib_out_ptr", "std::out_ptr"),
    (r"\bstd::inout_ptr\b", "__cpp_lib_out_ptr", "std::inout_ptr"),
    (r"\bstd::byteswap\b", "__cpp_lib_byteswap", "std::byteswap"),
    (r"\bstd::to_underlying\b", "__cpp_lib_to_underlying", "std::to_underlying"),
    (r"\bstd::start_lifetime_as(?:_array)?\b", "__cpp_lib_start_lifetime_as", "std::start_lifetime_as"),
    (r"\bstd::mdspan\b", None, "std::mdspan"),
]


def already_gated(text: str, macro: str | None, name: str) -> bool:
    if macro and (
        f"#if !defined({macro})" in text
        or f"#if defined({macro})" in text
        or f"#if {macro}" in text
    ):
        return True
    if name == "std::mdspan" and "LEARNCPP_HAS_MDSPAN" in text:
        return True
    if f"[skip] {name}" in text:
        return True
    return False


def ensure_includes(head: str) -> str:
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


def wrap(path: Path, macro: str, name: str) -> bool:
    text = path.read_text(encoding="utf-8")
    if already_gated(text, macro, name):
        return False
    m = re.search(r'::learn::topic<"([^"]+)"', text)
    topic = m.group(1) if m else "unknown/topic"

    # find end of leading includes / preprocessor
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
        ):
            insert_at = i + 1
        elif not s or s.startswith("//"):
            if insert_at:
                insert_at = i + 1
        elif insert_at:
            break

    head = ensure_includes("".join(lines[:insert_at]))
    rest = "".join(lines[insert_at:])
    if f"#if !defined({macro})" in rest[:300]:
        return False

    stub = f"""
#if !defined({macro}) || !({macro})
namespace {{
int run(int /*argc*/, char** /*argv*/) {{
    std::cout << "[skip] {name} not available ({macro})\\n";
    return 0;
}}
[[maybe_unused]] const auto& _ = ::learn::topic<"{topic}", run>;
}}  // namespace
#else
"""
    out = head + stub + rest
    if not re.search(rf"#endif\s*//\s*{re.escape(macro)}\s*$", out.rstrip()):
        out = out.rstrip() + f"\n#endif  // {macro}\n"
    path.write_text(out, encoding="utf-8", newline="\n")
    return True


n = 0
# multiple passes: a file may need only the first missing feature
for _ in range(3):
    for path in sorted(ROOT.rglob("*.cpp")):
        text = path.read_text(encoding="utf-8", errors="replace")
        for pat, macro, name in FEATURES:
            if macro is None:
                continue
            if not re.search(pat, text):
                continue
            if already_gated(text, macro, name):
                continue
            if wrap(path, macro, name):
                print("guarded", name, path.relative_to(ROOT.parent.parent))
                n += 1
                break
print("total_guarded", n)
