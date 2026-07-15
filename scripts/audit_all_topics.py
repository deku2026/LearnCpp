#!/usr/bin/env python3
"""Audit every topic .cpp with clang -Wshadow -Werror -fsyntax-only.

Collects ALL failures (does not stop at first). Use before claiming CI-green:

  python scripts/audit_all_topics.py

Skips src/cpp/ffm (shared lib; needs LEARN_FFM_EXPORTS, not a topic TU).
"""
from __future__ import annotations

from collections import Counter
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
CPP = ROOT / "src" / "cpp"
CLANG_CANDIDATES = [
    Path(r"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\Llvm\x64\bin\clang++.exe"),
    Path(r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang++.exe"),
    Path(r"C:\Program Files\LLVM\bin\clang++.exe"),
]


def find_clang() -> str:
    for p in CLANG_CANDIDATES:
        if p.is_file():
            return str(p)
    # PATH
    from shutil import which
    w = which("clang++")
    if w:
        return w
    sys.exit("clang++ not found; install LLVM or VS clang tools")


def main() -> int:
    clang = find_clang()
    flags = [
        "-std=c++23",
        "-O0",
        "-DNDEBUG",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wshadow",
        "-Wuninitialized",
        "-Wno-unused-parameter",
        "-Werror",
        f"-I{ROOT / 'include'}",
        "-fsyntax-only",
    ]
    files = [
        p
        for p in sorted(CPP.rglob("*.cpp"))
        if "src/cpp/ffm" not in p.as_posix() and r"src\cpp\ffm" not in str(p)
    ]
    print(f"clang={clang}")
    print(f"auditing {len(files)} files", flush=True)

    def check(path: Path):
        r = subprocess.run(
            [clang, *flags, str(path)],
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        )
        if r.returncode == 0:
            return None
        errs = [ln.strip() for ln in (r.stderr + "\n" + r.stdout).splitlines() if "error:" in ln]
        return path.relative_to(ROOT).as_posix(), errs[:30]

    results = []
    with ThreadPoolExecutor(max_workers=6) as ex:
        futs = {ex.submit(check, p): p for p in files}
        done = 0
        for fut in as_completed(futs):
            done += 1
            if done % 100 == 0:
                print(f"progress {done}/{len(files)} fails={len(results)}", flush=True)
            res = fut.result()
            if res is not None:
                results.append(res)

    print(f"DONE fails={len(results)} / {len(files)}")
    kinds: Counter[str] = Counter()
    for _rel, errs in results:
        for e in errs:
            m = re.search(r"error:\s*(.*?)(?:\s*\[|$)", e)
            kinds[(m.group(1) if m else e)[:120]] += 1
    print("TOP ERROR KINDS:")
    for k, c in kinds.most_common(40):
        print(f"  [{c}] {k}")
    print("\nFILES:")
    for rel, errs in sorted(results):
        print(f"--- {rel} ---")
        for e in errs[:12]:
            print(" ", e)
    out = ROOT / "audit_errors.txt"
    out.write_text(
        "\n\n".join(f"{rel}\n" + "\n".join(errs) for rel, errs in sorted(results)),
        encoding="utf-8",
    )
    print(f"wrote {out}")
    return 1 if results else 0


if __name__ == "__main__":
    raise SystemExit(main())
