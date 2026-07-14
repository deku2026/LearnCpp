#pragma once

/*
 * C ABI surface for LearnJava FFM (JEP 454) labs.
 * Loaded by Java via SymbolLookup.libraryLookup / loaderLookup.
 */

#include <stdint.h>

#if defined(_WIN32)
#if defined(LEARN_FFM_BUILD)
#define LEARN_FFM_API __declspec(dllexport)
#else
#define LEARN_FFM_API __declspec(dllimport)
#endif
#else
#define LEARN_FFM_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** ABI stamp for Java-side smoke checks. */
LEARN_FFM_API int32_t learn_ffm_abi_version(void);

LEARN_FFM_API int32_t learn_ffm_add_i32(int32_t a, int32_t b);
LEARN_FFM_API int64_t learn_ffm_add_i64(int64_t a, int64_t b);
LEARN_FFM_API double learn_ffm_add_f64(double a, double b);

/** Three-arg add — handy for multi-arg downcall debugging. */
LEARN_FFM_API int32_t learn_ffm_add3_i32(int32_t a, int32_t b, int32_t c);

/** Fused mul-add: a * b + c (easy breakpoint target). */
LEARN_FFM_API int32_t learn_ffm_mul_add_i32(int32_t a, int32_t b, int32_t c);

#ifdef __cplusplus
} /* extern "C" */
#endif
