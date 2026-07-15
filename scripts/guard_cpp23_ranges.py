#!/usr/bin/env python3
"""Gate C++23 ranges views and related APIs that often lag on libstdc++/libc++."""
from __future__ import annotations
import re
from pathlib import Path

ROOT = Path("src/cpp")

FEATURES = [
    (r"\bviews::zip\b|\branges::zip_view\b", "__cpp_lib_ranges_zip", "std::views::zip"),
    (r"\bviews::enumerate\b", "__cpp_lib_ranges_enumerate", "std::views::enumerate"),
    (r"\bviews::stride\b", "__cpp_lib_ranges_stride", "std::views::stride"),
    (r"\bviews::slide\b", "__cpp_lib_ranges_slide", "std::views::slide"),
    (r"\bviews::chunk(?:_by)?\b", "__cpp_lib_ranges_chunk", "std::views::chunk"),
    (r"\bviews::join_with\b", "__cpp_lib_ranges_join_with", "std::views::join_with"),
    (r"\bviews::cartesian_product\b", "__cpp_lib_ranges_cartesian_product", "std::views::cartesian_product"),
    (r"\bviews::repeat\b", "__cpp_lib_ranges_repeat", "std::views::repeat"),
    (r"\bviews::adjacent\b", "__cpp_lib_ranges_zip", "std::views::adjacent"),
    (r"\bstd::ranges::to\b", "__cpp_lib_ranges_to_container", "std::ranges::to"),
    (r"\bstd::spanstream\b|\bstd::ispanstream\b|\bstd::ospanstream\b", "__cpp_lib_spanstream", "std::spanstream"),
]

def topic_id(text: str) -> str:
    m = re.search(r'::learn::topic<"([^"]+)"', text)
    return m.group(1) if m else "unknown/topic"

def has_gate(text: str, macro: str) -> bool:
    return f"!defined({macro})" in text or f"defined({macro})" in text

def wrap(path: Path, macro: str, name: str) -> bool:
    text = path.read_text(encoding="utf-8")
    if has_gate(text, macro):
        return False
    tid = topic_id(text)
    lines = text.splitlines(keepends=True)
    insert_at = 0
    for i, ln in enumerate(lines):
        s = ln.lstrip()
        if s.startswith("#include") or s.startswith("#if") or s.startswith("#endif") or s.startswith("#define") or s.startswith("#pragma") or s.startswith("#else") or s.startswith("#elif") or s.startswith("#undef") or s.startswith("#ifndef"):
            insert_at = i + 1
        elif not s or s.startswith("//"):
            if insert_at:
                insert_at = i + 1
        elif insert_at:
            break
    head = "".join(lines[:insert_at])
    rest = "".join(lines[insert_at:])
    if "#include <version>" not in head:
        head = head.replace('#include "learn/topic_registry.hpp"\n', '#include "learn/topic_registry.hpp"\n\n#include <version>\n', 1)
    if "#include <iostream>" not in head:
        head = head.replace('#include "learn/topic_registry.hpp"\n', '#include "learn/topic_registry.hpp"\n\n#include <iostream>\n', 1)
    stub = f"""
#if !defined({macro}) || !({macro})
namespace {{
int run(int /*argc*/, char** /*argv*/) {{
    std::cout << "[skip] {name} not available ({macro})\\n";
    return 0;
}}
[[maybe_unused]] const auto& _ = ::learn::topic<"{tid}", run>;
}}  // namespace
#else
"""
    out = head.rstrip() + "\n" + stub + rest
    if not re.search(rf"#endif\s*//\s*{re.escape(macro)}\s*$", out.rstrip()):
        out = out.rstrip() + f"\n#endif  // {macro}\n"
    path.write_text(out, encoding="utf-8", newline="\n")
    return True

n = 0
for path in sorted(ROOT.rglob("*.cpp")):
    text = path.read_text(encoding="utf-8", errors="replace")
    for pat, macro, name in FEATURES:
        if not re.search(pat, text):
            continue
        if has_gate(text, macro):
            continue
        if wrap(path, macro, name):
            print("gated", name, path.relative_to(ROOT.parent.parent))
            n += 1
            break
print("total", n)
