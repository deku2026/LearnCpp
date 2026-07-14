#include "learn_ffm.h"

int32_t learn_ffm_abi_version(void) {
    return 1;
}

int32_t learn_ffm_add_i32(int32_t a, int32_t b) {
    return a + b;
}

int64_t learn_ffm_add_i64(int64_t a, int64_t b) {
    return a + b;
}

double learn_ffm_add_f64(double a, double b) {
    return a + b;
}

int32_t learn_ffm_add3_i32(int32_t a, int32_t b, int32_t c) {
    return a + b + c;
}

int32_t learn_ffm_mul_add_i32(int32_t a, int32_t b, int32_t c) {
    // Good place for a native breakpoint when attaching a C++ debugger to java.exe.
    return a * b + c;
}
