#!/usr/bin/env bash
#
# Compile every translation unit across the supported compiler/standard-library
# combinations.  Each entry uses a clean, uncached CMake build, warnings as
# errors, UBSan, and Ninja -k 0 so one diagnostic does not hide later failures.

set -uo pipefail

script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_root=$(cd -- "$script_dir/.." && pwd)
build_root=${LEARNCPP_STRICT_BUILD_ROOT:-"$repo_root/build/strict"}
expected_topics=${LEARNCPP_EXPECTED_TOPICS:-874}
clang_major=${LEARNCPP_EXPECTED_CLANG_MAJOR:-22}
gcc_major=${LEARNCPP_EXPECTED_GCC_MAJOR:-14}

read -r -a build_types <<< "${LEARNCPP_STRICT_BUILD_TYPES:-Debug RelWithDebInfo}"

declare -a failures=()
declare -a successes=()

record_failure() {
    failures+=("$1")
    printf 'FAILED: %s\n' "$1" >&2
}

find_compiler() {
    local override=$1
    shift
    if [ -n "$override" ]; then
        command -v -- "$override" 2>/dev/null || printf '%s\n' "$override"
        return
    fi

    local candidate
    for candidate in "$@"; do
        if command -v -- "$candidate" >/dev/null 2>&1; then
            command -v -- "$candidate"
            return
        fi
    done
    return 1
}

verify_compiler() {
    local compiler=$1
    local expected_major=$2
    local label=$3

    if [ ! -x "$compiler" ]; then
        record_failure "$label: compiler is not executable: $compiler"
        return 1
    fi

    local version
    local actual_major
    version=$("$compiler" -dumpversion 2>/dev/null) || {
        record_failure "$label: could not query compiler version"
        return 1
    }
    actual_major=${version%%.*}
    if [ "$actual_major" != "$expected_major" ]; then
        record_failure "$label: expected compiler major $expected_major, got $version ($compiler)"
        return 1
    fi

    "$compiler" --version | sed -n '1p'
}

verify_executable() {
    local executable=$1
    local label=$2
    local output
    if ! output=$(UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 "$executable" --list); then
        record_failure "$label: learn_cpp --list failed"
        return 1
    fi

    local header=${output%%$'\n'*}
    local listed
    listed=$(printf '%s\n' "$output" | sed -n 's/^  //p' | wc -l | tr -d ' ')
    if [ "$header" != "learn_cpp: $expected_topics topics registered" ] ||
        [ "$listed" != "$expected_topics" ]; then
        record_failure "$label: expected $expected_topics registered topics; header='$header', listed=$listed"
        return 1
    fi
}

run_entry() {
    local label=$1
    local compiler=$2
    local build_type=$3
    local stdlib_flags=$4
    local build_dir="$build_root/$label-$build_type"

    printf '\n=== %s / %s ===\n' "$label" "$build_type"

    local -a configure_args=(
        --fresh
        -S "$repo_root"
        -B "$build_dir"
        -G Ninja
        "-DCMAKE_BUILD_TYPE=$build_type"
        "-DCMAKE_CXX_COMPILER=$compiler"
        -DLEARNCPP_WERROR=ON
        -DLEARNCPP_ENABLE_UBSAN=ON
        -DLEARNCPP_USE_SCCACHE=OFF
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    )
    if [ -n "$stdlib_flags" ]; then
        configure_args+=(
            "-DCMAKE_CXX_FLAGS=$stdlib_flags"
            "-DCMAKE_EXE_LINKER_FLAGS=$stdlib_flags"
        )
    fi

    if ! cmake "${configure_args[@]}"; then
        record_failure "$label/$build_type: configure"
        return
    fi

    if ! cmake --build "$build_dir" --parallel --clean-first -- -k 0; then
        record_failure "$label/$build_type: full build"
        return
    fi

    if ! verify_executable "$build_dir/bin/learn_cpp" "$label/$build_type"; then
        return
    fi

    successes+=("$label/$build_type")
    printf 'PASSED: %s / %s\n' "$label" "$build_type"
}

if ! command -v cmake >/dev/null 2>&1; then
    record_failure "cmake is not available"
fi
if ! command -v ninja >/dev/null 2>&1; then
    record_failure "ninja is not available"
fi

host=$(uname -s)
case "$host" in
    Linux)
        clangxx=$(find_compiler "${LEARNCPP_CLANGXX:-}" "clang++-$clang_major" clang++ || true)
        gxx=$(find_compiler "${LEARNCPP_GXX:-}" "g++-$gcc_major" g++ || true)

        clang_ok=0
        gcc_ok=0
        if [ -n "$clangxx" ] && verify_compiler "$clangxx" "$clang_major" "Clang"; then
            clang_ok=1
        elif [ -z "$clangxx" ]; then
            record_failure "Clang $clang_major was not found"
        fi
        if [ -n "$gxx" ] && verify_compiler "$gxx" "$gcc_major" "GCC"; then
            gcc_ok=1
        elif [ -z "$gxx" ]; then
            record_failure "GCC $gcc_major was not found"
        fi

        for build_type in "${build_types[@]}"; do
            if [ "$clang_ok" -eq 1 ]; then
                run_entry clang-libstdcxx "$clangxx" "$build_type" "-stdlib=libstdc++"
                run_entry clang-libcxx "$clangxx" "$build_type" "-stdlib=libc++"
            fi
            if [ "$gcc_ok" -eq 1 ]; then
                run_entry gcc-libstdcxx "$gxx" "$build_type" ""
            fi
        done
        ;;
    Darwin)
        clangxx=$(find_compiler \
            "${LEARNCPP_CLANGXX:-}" \
            "/opt/homebrew/opt/llvm@$clang_major/bin/clang++" \
            "/usr/local/opt/llvm@$clang_major/bin/clang++" \
            "clang++-$clang_major" \
            clang++ || true)
        if [ -n "$clangxx" ] && verify_compiler "$clangxx" "$clang_major" "Clang"; then
            for build_type in "${build_types[@]}"; do
                run_entry clang-libcxx "$clangxx" "$build_type" ""
            done
        elif [ -z "$clangxx" ]; then
            record_failure "Clang $clang_major was not found"
        fi
        ;;
    *)
        record_failure "unsupported host '$host'; use the Windows presets on Windows"
        ;;
esac

printf '\nStrict build matrix summary: %d passed, %d failed\n' "${#successes[@]}" "${#failures[@]}"
if [ "${#successes[@]}" -gt 0 ]; then
    printf '  PASS %s\n' "${successes[@]}"
fi
if [ "${#failures[@]}" -gt 0 ]; then
    printf '  FAIL %s\n' "${failures[@]}" >&2
    exit 1
fi
